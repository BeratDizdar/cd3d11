#include "d11_internal.h"

void d11_draw(int vertex_count, int base_vertex) {
    _d->ctx->lpVtbl->Draw(_d->ctx, vertex_count, base_vertex);
}

void d11_clear(float r, float g, float b) {
    _d->ctx->lpVtbl->ClearRenderTargetView(_d->ctx, _d->rtv0, (const float[4]){r, g, b, 1});
}

void d11_present() {
    _d->swp->lpVtbl->Present(_d->swp, 0, 0);
}