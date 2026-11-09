#include "d11_internal.h"

void d11_topology(int _Type) {
    _d->ctx->lpVtbl->IASetPrimitiveTopology(_d->ctx, _Type);
}

void d11_shader(int _Slot) {
    _d->ctx->lpVtbl->VSSetShader(_d->ctx, _d->vs[_Slot], NULL, 0);
    _d->ctx->lpVtbl->PSSetShader(_d->ctx, _d->ps[_Slot], NULL, 0);
}