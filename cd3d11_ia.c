#include "cd3d11_internal.h"

void cd3d11_topology(cd3d11 *d3d, D3D_PRIMITIVE_TOPOLOGY topology) {
    d3d->context->lpVtbl->IASetPrimitiveTopology(d3d->context, topology);
}