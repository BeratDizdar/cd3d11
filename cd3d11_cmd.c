#include "cd3d11_internal.h"

void cd3d11_cmd_draw(cd3d11 *d3d, int vertex_count, int base_vertex) {
    d3d->context->lpVtbl->Draw(d3d->context, vertex_count, base_vertex);
}

void cd3d11_cmd_drawindexed(cd3d11 *d3d, int index_count, int base_index, int base_vertex) {
    d3d->context->lpVtbl->DrawIndexed(d3d->context, index_count, base_index, base_vertex);
}

void cd3d11_cmd_drawinstanced(cd3d11 *d3d, int inst_v_count, int inst_count, int base_vertex, int base_inst) {
    d3d->context->lpVtbl->DrawInstanced(d3d->context, inst_v_count, inst_count, base_vertex, base_inst);
}

void cd3d11_cmd_dispatch(cd3d11 *d3d, int x, int y, int z) {
    d3d->context->lpVtbl->Dispatch(d3d->context, x, y, z);
}

void cd3d11_cmd_present(cd3d11 *d3d, int sync_interval) {
    d3d->swapchain->lpVtbl->Present(d3d->swapchain, sync_interval, 0);
}

void cd3d11_cmd_clear_default(cd3d11 *d3d) {
    d3d->context->lpVtbl->ClearRenderTargetView(d3d->context, d3d->default_rtv, (const float[4]){0, 0, 0, 1});
}