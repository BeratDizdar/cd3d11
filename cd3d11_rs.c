#include "cd3d11_internal.h"

void cd3d11_rs_viewport(cd3d11 *d3d, int w, int h) {
    D3D11_VIEWPORT vp = {0};
    vp.Width    = w;
    vp.Height   = h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    d3d->context->lpVtbl->RSSetViewports(d3d->context, 1, &vp);
}

void cd3d11_rs_state(cd3d11 *d3d, void *rasterstate) {
    d3d->context->lpVtbl->RSSetState(d3d->context, rasterstate);
}

void cd3d11_rs_scissor(cd3d11 *d3d, int l, int u, int r, int b) {
    D3D11_RECT rect = {l, u, r, b};
    d3d->context->lpVtbl->RSSetScissorRects(d3d->context, 1, &rect);
}