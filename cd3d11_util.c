#include "cd3d11_internal.h"
#include <stdio.h>

void cd3d11_memcpy(cd3d11 *d3d, void *obj, void *data, long long size, int discard) {
    ID3D11Resource *r = obj;
    D3D11_MAPPED_SUBRESOURCE m = {0};
    d3d->context->lpVtbl->Map(d3d->context, r, 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &m);
    memcpy(&m, data, size);
    d3d->context->lpVtbl->Unmap(d3d->context, r, 0);
}