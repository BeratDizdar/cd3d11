typedef HRESULT (__stdcall *PFN_D3D11Init)(void*, int, void*, UINT, const void*, UINT, UINT, const void*, IDXGISwapChain**, ID3D11Device**, void*, ID3D11DeviceContext**);

__declspec(dllexport) void *d11_open(void *hwnd, int w, int h) {
    d11 *p = calloc(1, sizeof(d11));

    p->lib = LoadLibraryA("d3d11.dll");
    PFN_D3D11Init D3D11Init = (PFN_D3D11Init)GetProcAddress(p->lib, "D3D11CreateDeviceAndSwapChain");
    if (!D3D11Init) {
        fprintf(stderr, "[D11]: where is library!\n");
        return NULL;
    }

    DXGI_SWAP_CHAIN_DESC sD = {0};
    sD.BufferCount = 1;
    sD.BufferDesc.Width = w;
    sD.BufferDesc.Height = h;
    sD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sD.BufferDesc.RefreshRate.Numerator = 60;
    sD.BufferDesc.RefreshRate.Denominator = 1;
    sD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sD.OutputWindow = hwnd;
    sD.SampleDesc.Count = 1;
    sD.SampleDesc.Quality = 0;
    sD.Windowed = TRUE;
    D3D_FEATURE_LEVEL fReq = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL fSup;
    if (D3D11Init(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fReq, 1, D3D11_SDK_VERSION, &sD, &p->swp, &p->dev, &fSup, &p->ctx) != S_OK) {
        fprintf(stderr, "[D11]: device creation failed!\n");
        return NULL;
    }

    ID3D11Resource *tmpRTVres = NULL;
    p->swp->lpVtbl->GetBuffer(p->swp, 0, &IID_ID3D11Resource, (void**)&tmpRTVres);
    p->dev->lpVtbl->CreateRenderTargetView(p->dev, tmpRTVres, NULL, &p->rtv0);
    p->ctx->lpVtbl->OMSetRenderTargets(p->ctx, 1, &p->rtv0, NULL);
    tmpRTVres->lpVtbl->Release(tmpRTVres);

    D3D11_VIEWPORT vp = {0};
    vp.Width    = w;
    vp.Height   = h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    p->ctx->lpVtbl->RSSetViewports(p->ctx, 1, &vp);

    D3D11_QUERY_DESC qDesc = {D3D11_QUERY_TIMESTAMP, 0};
    p->dev->lpVtbl->CreateQuery(p->dev, &qDesc, &p->t0);
    p->dev->lpVtbl->CreateQuery(p->dev, &qDesc, &p->t1);

    return (void*)p;
}

__declspec(dllexport) void d11_make(void *d11_ptr) {
    if (d11_ptr) _d = (d11*)d11_ptr;
}

__declspec(dllexport) void d11_close(void *d11_ptr) {
    d11 *p = (d11*)d11_ptr;
    if (!p) return;
    for (int i = 0; i < D11_MAX_SHADER_SLOT; i++) {
        d11__release_if_exists(p->vs[i]);
        d11__release_if_exists(p->ps[i]);
    }
    for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT; i++) {
        d11__release_if_exists(p->resource[i]);
        d11__release_if_exists(p->reg_t[i]);
    }
    for (int i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_COUNT; i++) {
        d11__release_if_exists(p->reg_b[i]);
    }
    for (int i = 0; i < D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT; i++) {
        d11__release_if_exists(p->reg_s[i]);
    }

    d11__release_if_exists(p->t0);
    d11__release_if_exists(p->t1);
    d11__release_if_exists(p->rtv0);
    d11__release_if_exists(p->dsv0);
    d11__release_if_exists(p->ctx);
    d11__release_if_exists(p->dev);
    d11__release_if_exists(p->swp);
    if (p->lib) FreeLibrary((HMODULE)p->lib);
    free(p);
}