#include "cd3d11_internal.h"
#include "cd3d11.h"
#include <stdio.h>

void cd3d11_release_managed_memory(cd3d11 *d3d) {
    for (int i = 0; i < STACK_MEMORY_MAX; i++) {
        cd3d11__safe_release_obj(d3d->stack.cell[i].ptr);
    }
    d3d->stack.top = 0;
}

void *cd3d11_new_shader_m(cd3d11 *d3d, const void *data, long long size, shader_stage_t stage) {
    void *shader = NULL;
    if      (stage & STAGE_VERTEX) d3d->device->lpVtbl->CreateVertexShader(d3d->device, data, size, NULL, (ID3D11VertexShader**)&shader);
    else if (stage & STAGE_PIXEL) d3d->device->lpVtbl->CreatePixelShader(d3d->device, data, size, NULL, (ID3D11PixelShader**)&shader);
    else if (stage & STAGE_COMPUTE) d3d->device->lpVtbl->CreateComputeShader(d3d->device, data, size, NULL, (ID3D11ComputeShader**)&shader);
    else fprintf(stderr, "[CD3D11]: Unknown stage (%d)\n", stage);
    cd3d11__push_stack(d3d, shader, 0);
    return (void*)shader;
}

void *cd3d11_new_cbuffer_m(cd3d11 *d3d, void *data, int size, int is_mut) {
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

void *cd3d11_new_buffer_m(cd3d11 *d3d, void *data, int size, int stride, int is_mut, int is_raw) {
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

void *cd3d11_new_sampler_m(cd3d11 *d3d, int is_linear) {
    D3D11_SAMPLER_DESC sd = {0};
    sd.Filter = (is_linear) ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    ID3D11SamplerState *s = NULL;
    d3d->device->lpVtbl->CreateSamplerState(d3d->device, &sd, &s);
    cd3d11__push_stack(d3d, s, 0);
    return (void*)s;
}

void *cd3d11_new_tex2d_m(cd3d11 *d3d, const void *data, int w, int h, int pitch, int is_mut) {
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

void *cd3d11_new_rasterstate_m(cd3d11 *d3d, int bSolid, D3D11_CULL_MODE cull, int bCcw) {
    D3D11_RASTERIZER_DESC rd = {0};
    rd.CullMode = cull;
    rd.FillMode = (bSolid == 0) ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    rd.FrontCounterClockwise = (bCcw) ? 1 : 0;
    ID3D11RasterizerState *rs = NULL;
    d3d->device->lpVtbl->CreateRasterizerState(d3d->device, &rd, &rs);
    cd3d11__push_stack(d3d, rs, 0);
    return (void*)rs;
}

/* TODO
void *cd3d11_new_blendstate_m(cd3d11 *d3d) {
    D3D11_BLEND_DESC bd = {0};
    ID3D11BlendState *bs = NULL;
    d3d->device->lpVtbl->CreateBlendState(d3d->device, &bd, &bs);
    cd3d11__push_stack(d3d, bs, 0);
    return (void*)bs;
}


void *cd3d11_new_depthstencilstate_m(cd3d11 *d3d) {
    D3D11_DEPTH_STENCIL_DESC dsd = {0};
    ID3D11DepthStencilState *dss = NULL;
    d3d->device->lpVtbl->CreateDepthStencilState(d3d->device, &dsd, &dss);
    cd3d11__push_stack(d3d, dss, 0);
    return (void*)dss;
}
*/