#include "cd3d11_internal.h"

void cd3d11_draw(cd3d11 *d3d, int vertex_count, int base_vertex) {
    d3d->context->lpVtbl->Draw(d3d->context, vertex_count, base_vertex);
}

void cd3d11_present(cd3d11 *d3d, int sync_interval) {
    d3d->swapchain->lpVtbl->Present(d3d->swapchain, sync_interval, 0);
}

void cd3d11_clear_default(cd3d11 *d3d) {
    d3d->context->lpVtbl->ClearRenderTargetView(d3d->context, d3d->default_rtv, (const float[4]){0, 0, 0, 1});
}