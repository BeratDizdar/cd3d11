#include "cd3d11_internal.h"
#include <stdio.h>
#include <libloaderapi.h>

typedef HRESULT (__stdcall *PFN_D3D11CreateDeviceAndSwapChain)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

int cd3d11_open(void *hwnd, int w, int h, cd3d11 *context) {
    IDXGISwapChain **s = &context->swapchain;
    ID3D11Device **d = &context->device;
    ID3D11DeviceContext **c = &context->context;
    ID3D11Query **qS = (ID3D11Query**)&context->debug.time0;
    ID3D11Query **qE = (ID3D11Query**)&context->debug.time1;

    context->lib = LoadLibraryA("d3d11.dll");
    PFN_D3D11CreateDeviceAndSwapChain D3D11CreateDeviceAndSwapChain = (PFN_D3D11CreateDeviceAndSwapChain)GetProcAddress(context->lib, "D3D11CreateDeviceAndSwapChain");

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
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fReq, 1, D3D11_SDK_VERSION, &sD, s, d, &fSup, c) != S_OK) {
        fprintf(stderr, "[CD3D11]: Device creation failed!\n");
        return 1;
    }

    ID3D11Resource *tmpRTVres = NULL;
    (*s)->lpVtbl->GetBuffer(*s, 0, &IID_ID3D11Resource, (void**)&tmpRTVres);
    (*d)->lpVtbl->CreateRenderTargetView(*d, tmpRTVres, NULL, &context->default_rtv);
    (*c)->lpVtbl->OMSetRenderTargets(*c, 1, &context->default_rtv, NULL);
    tmpRTVres->lpVtbl->Release(tmpRTVres);

    D3D11_VIEWPORT vp = {0};
    vp.Width    = w;
    vp.Height   = h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    (*c)->lpVtbl->RSSetViewports(*c, 1, &vp);

    D3D11_QUERY_DESC qDesc = {D3D11_QUERY_TIMESTAMP, 0};
    (*d)->lpVtbl->CreateQuery(*d, &qDesc, qS);
    (*d)->lpVtbl->CreateQuery(*d, &qDesc, qE);

    printf("[CD3D11]: Device created\n");
    return 0;
}