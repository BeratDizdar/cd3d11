#include "cd3d11_internal.h"

void cd3d11_bind_resource_view(cd3d11 *d3d, void *view, int slot, int bind_vs, int bind_ps) {
    ID3D11ShaderResourceView *srv = view;
    if (bind_vs) {
        d3d->context->lpVtbl->VSSetShaderResources(d3d->context, slot, 1, &srv);
    }
    if (bind_ps) {
        d3d->context->lpVtbl->PSSetShaderResources(d3d->context, slot, 1, &srv);
    }
}

void cd3d11_bind_cbuffer(cd3d11 *d3d, void *cbuffer, int slot, int bind_vs, int bind_ps) {
    ID3D11Buffer *cb = cbuffer;
    if (bind_vs) {
        d3d->context->lpVtbl->VSSetConstantBuffers(d3d->context, slot, 1, &cb);
    }
    if (bind_ps) {
        d3d->context->lpVtbl->PSSetConstantBuffers(d3d->context, slot, 1, &cb);
    }
}

void cd3d11_bind_vertex_shader(cd3d11 *d3d, void *shader) {
    d3d->context->lpVtbl->VSSetShader(d3d->context, (ID3D11VertexShader*)shader, NULL, 0);
}

void cd3d11_bind_pixel_shader(cd3d11 *d3d, void *shader) {
    d3d->context->lpVtbl->PSSetShader(d3d->context, (ID3D11PixelShader*)shader, NULL, 0);
}