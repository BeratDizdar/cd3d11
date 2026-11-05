#include "cd3d11_internal.h"
#include "cd3d11.h"

void cd3d11_release_managed(cd3d11 *d3d) {
    for (int i = 0; i < STACK_MEMORY_MAX; i++) {
        cd3d11__safe_release_obj(d3d->stack.cell[i].ptr);
    }
    d3d->stack.top = 0;
}

void *cd3d11_new_vertex_shader_managed(cd3d11 *d3d, const void *data, long long size) {
    ID3D11VertexShader *vs = NULL;
    d3d->device->lpVtbl->CreateVertexShader(d3d->device, data, size, NULL, &vs);
    cd3d11__push_stack(d3d, vs, 0);
    return (void*)vs;
}

void *cd3d11_new_pixel_shader_managed(cd3d11 *d3d, const void *data, long long size) {
    ID3D11PixelShader *ps = NULL;
    d3d->device->lpVtbl->CreatePixelShader(d3d->device, data, size, NULL, &ps);
    cd3d11__push_stack(d3d, ps, 0);
    return (void*)ps;
}

void *cd3d11_new_cbuffer_managed(cd3d11 *d3d, void *data, int size, int is_mut) {
    D3D11_BUFFER_DESC bd = {0};
    bd.ByteWidth      = size;
    bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = (is_mut) ? D3D11_CPU_ACCESS_WRITE : 0;
    bd.Usage          = (is_mut) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
    D3D11_SUBRESOURCE_DATA srd = {.pSysMem = data};
    ID3D11Buffer *cb = NULL;
    d3d->device->lpVtbl->CreateBuffer(d3d->device, &bd, &srd, &cb);
    cd3d11__push_stack(d3d, cb, (is_mut) ? 1 : 0);
    return (void*)cb;
}

void *cd3d11_new_buffer_managed(cd3d11 *d3d, void *data, int size, int stride, int is_mut, int is_raw) {
    D3D11_BUFFER_DESC bd = {0};
    bd.ByteWidth      = size;
    bd.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
    bd.CPUAccessFlags = (is_mut) ? D3D11_CPU_ACCESS_WRITE : 0;
    bd.Usage          = (is_mut) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
    bd.MiscFlags      = (is_raw) ? D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS : D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bd.StructureByteStride = stride;
    D3D11_SUBRESOURCE_DATA srd = {.pSysMem = data};
    ID3D11Buffer *b = NULL;
    d3d->device->lpVtbl->CreateBuffer(d3d->device, &bd, &srd, &b);
    cd3d11__push_stack(d3d, b, (is_mut) ? 1 : 0);
    return (void*)b;
}

void *cd3d11_new_sampler_managed(cd3d11 *d3d, int is_linear) {
    D3D11_SAMPLER_DESC sd = {0};
    sd.Filter = (is_linear) ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    ID3D11SamplerState *s = NULL;
    d3d->device->lpVtbl->CreateSamplerState(d3d->device, &sd, &s);
    cd3d11__push_stack(d3d, s, 0);
    return (void*)s;
}

void *cd3d11_new_tex2d_managed(cd3d11 *d3d, const void *data, int w, int h, int pitch, int is_mut) {
    D3D11_TEXTURE2D_DESC td = {0};
    td.Width              = w;
    td.Height             = h;
    td.MipLevels          = 1;
    td.ArraySize          = 1;
    td.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count   = 1;
    td.SampleDesc.Quality = 0;
    td.Usage              = (is_mut) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
    td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags     = (is_mut) ? D3D11_CPU_ACCESS_WRITE : 0;
    td.MiscFlags          = 0;

    D3D11_SUBRESOURCE_DATA srd = {0};
    srd.pSysMem          = data;
    srd.SysMemPitch      = pitch;
    srd.SysMemSlicePitch = 0;
    ID3D11Texture2D *t = NULL;
    d3d->device->lpVtbl->CreateTexture2D(d3d->device, &td, (data ? &srd : NULL), &t);
    cd3d11__push_stack(d3d, t, (is_mut) ? 1 : 0);
    return (void*)t;
}