#include "cd3d11_internal.h"

void cd3d11_viewport(cd3d11 *d3d, int w, int h) {
    D3D11_VIEWPORT vp = {0};
    vp.Width    = w;
    vp.Height   = h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    d3d->context->lpVtbl->RSSetViewports(d3d->context, 1, &vp);
}