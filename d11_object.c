__declspec(dllexport) void d11_new_cbuffer(int _Slot, const void *_Data, long long _Size) {
    if (_Slot < 0 || _Slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_COUNT) {
        fprintf(stderr, "[D11]: invalid constant buffer register (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->reg_b[_Slot]);

    D3D11_BUFFER_DESC bd = {0};
    bd.ByteWidth      = _Size;
    bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.Usage          = D3D11_USAGE_DYNAMIC;
    D3D11_SUBRESOURCE_DATA srd = {.pSysMem = _Data};
    D3D11_SUBRESOURCE_DATA *p_srd = (_Data != NULL) ? &srd : NULL;
    _d->dev->lpVtbl->CreateBuffer(_d->dev, &bd, p_srd, &_d->reg_b[_Slot]);

    _d->ctx->lpVtbl->VSSetConstantBuffers(_d->ctx, _Slot, 1, &_d->reg_b[_Slot]);
    _d->ctx->lpVtbl->PSSetConstantBuffers(_d->ctx, _Slot, 1, &_d->reg_b[_Slot]);
}

__declspec(dllexport) void d11_new_structured_buffer(int _Slot, const void *_Data, long long _Size, int _Stride) {
    if (_Slot < 0 || _Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT) {
        fprintf(stderr, "[D11]: invalid input resource register (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->resource[_Slot]);
    d11__release_if_exists(_d->reg_t[_Slot]);

    D3D11_BUFFER_DESC bd = {0};
    bd.ByteWidth           = _Size;
    bd.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
    bd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    bd.Usage               = D3D11_USAGE_DYNAMIC;
    bd.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bd.StructureByteStride = _Stride;
    D3D11_SUBRESOURCE_DATA srd = {.pSysMem = _Data};
    D3D11_SUBRESOURCE_DATA *p_srd = (_Data != NULL) ? &srd : NULL;
    _d->dev->lpVtbl->CreateBuffer(_d->dev, &bd, p_srd, (ID3D11Buffer**)&_d->resource[_Slot]);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {0};
    srvd.Format              = DXGI_FORMAT_UNKNOWN;
    srvd.ViewDimension       = D3D11_SRV_DIMENSION_BUFFER;
    srvd.Buffer.FirstElement = 0;
    srvd.Buffer.NumElements  = (UINT)(_Size / _Stride);
    _d->dev->lpVtbl->CreateShaderResourceView(_d->dev, _d->resource[_Slot], &srvd, &_d->reg_t[_Slot]);

    _d->ctx->lpVtbl->VSSetShaderResources(_d->ctx, _Slot, 1, &_d->reg_t[_Slot]);
    _d->ctx->lpVtbl->PSSetShaderResources(_d->ctx, _Slot, 1, &_d->reg_t[_Slot]);
}

__declspec(dllexport) void d11_new_byte_address_buffer(int _Slot, const void *_Data, long long _Size) {
    if (_Slot < 0 || _Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT) {
        fprintf(stderr, "[D11]: invalid input resource register (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->resource[_Slot]);
    d11__release_if_exists(_d->reg_t[_Slot]);

    D3D11_BUFFER_DESC bd = {0};
    bd.ByteWidth      = _Size;
    bd.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.Usage          = D3D11_USAGE_DYNAMIC;
    bd.MiscFlags      = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    D3D11_SUBRESOURCE_DATA srd = {.pSysMem = _Data};
    D3D11_SUBRESOURCE_DATA *p_srd = (_Data != NULL) ? &srd : NULL;
    _d->dev->lpVtbl->CreateBuffer(_d->dev, &bd, p_srd, (ID3D11Buffer**)&_d->resource[_Slot]);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {0};
    srvd.Format                = DXGI_FORMAT_R32_TYPELESS;
    srvd.ViewDimension         = D3D11_SRV_DIMENSION_BUFFEREX;
    srvd.BufferEx.FirstElement = 0;
    srvd.BufferEx.NumElements  = (UINT)(_Size >> 2);
    srvd.BufferEx.Flags        = D3D11_BUFFEREX_SRV_FLAG_RAW;
    _d->dev->lpVtbl->CreateShaderResourceView(_d->dev, _d->resource[_Slot], &srvd, &_d->reg_t[_Slot]);

    _d->ctx->lpVtbl->VSSetShaderResources(_d->ctx, _Slot, 1, &_d->reg_t[_Slot]);
    _d->ctx->lpVtbl->PSSetShaderResources(_d->ctx, _Slot, 1, &_d->reg_t[_Slot]);
}

__declspec(dllexport) void d11_new_texture2d(int _Slot, const void *_Data, int _Width, int _Height) {
    if (_Slot < 0 || _Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT) {
        fprintf(stderr, "[D11]: invalid input resource register (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->resource[_Slot]);
    d11__release_if_exists(_d->reg_t[_Slot]);

    D3D11_TEXTURE2D_DESC td = {0};
    td.Width              = (UINT)_Width;
    td.Height             = (UINT)_Height;
    td.MipLevels          = 1;
    td.ArraySize          = 1;
    td.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count   = 1;
    td.SampleDesc.Quality = 0;
    td.Usage              = D3D11_USAGE_DEFAULT;
    td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags     = 0;
    td.MiscFlags          = 0;

    D3D11_SUBRESOURCE_DATA srd = {0};
    srd.pSysMem          = _Data;
    srd.SysMemPitch      = (UINT)_Width * 4;
    D3D11_SUBRESOURCE_DATA *p_srd = (_Data != NULL) ? &srd : NULL;

    _d->dev->lpVtbl->CreateTexture2D(_d->dev, &td, p_srd, (ID3D11Texture2D**)&_d->resource[_Slot]);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {0};
    srvd.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvd.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MostDetailedMip = 0;
    srvd.Texture2D.MipLevels       = 1;
    _d->dev->lpVtbl->CreateShaderResourceView(_d->dev, _d->resource[_Slot], &srvd, &_d->reg_t[_Slot]);

    _d->ctx->lpVtbl->VSSetShaderResources(_d->ctx, _Slot, 1, &_d->reg_t[_Slot]);
    _d->ctx->lpVtbl->PSSetShaderResources(_d->ctx, _Slot, 1, &_d->reg_t[_Slot]);
}

__declspec(dllexport) void d11_new_sampler(int _Slot, int _IsLinear) {
    if (_Slot < 0 || _Slot >= D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT) {
        fprintf(stderr, "[D11]: invalid sampler register (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->reg_s[_Slot]);

    D3D11_SAMPLER_DESC sd = {0};
    sd.Filter         = (_IsLinear) ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    sd.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sd.MinLOD         = 0;
    sd.MaxLOD         = D3D11_FLOAT32_MAX;
    _d->dev->lpVtbl->CreateSamplerState(_d->dev, &sd, (ID3D11SamplerState**)&_d->reg_s[_Slot]);
    
    _d->ctx->lpVtbl->VSSetSamplers(_d->ctx, _Slot, 1, &_d->reg_s[_Slot]);
    _d->ctx->lpVtbl->PSSetSamplers(_d->ctx, _Slot, 1, &_d->reg_s[_Slot]);
}

__declspec(dllexport) void d11_new_shader(int _Slot, const void *_VData, long long _VSize, const void *_PData, long long _PSize) {
    if (_Slot < 0 || _Slot >= D11_MAX_SHADER_SLOT) {
        fprintf(stderr, "[D11]: invalid shader slot (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->vs[_Slot]);
    d11__release_if_exists(_d->ps[_Slot]);

    _d->dev->lpVtbl->CreateVertexShader(_d->dev, _VData, _VSize, NULL, (ID3D11VertexShader**)&_d->vs[_Slot]);
    _d->dev->lpVtbl->CreatePixelShader(_d->dev, _PData, _PSize, NULL, (ID3D11PixelShader**)&_d->ps[_Slot]);
}