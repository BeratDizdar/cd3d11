#include "cd3d11_internal.h"

void cd3d11_mark_time(cd3d11 *d3d) {
    d3d->context->lpVtbl->End(d3d->context, (ID3D11Asynchronous*)d3d->debug.time0);
}

unsigned long long cd3d11_wait_and_get_time_value(cd3d11 *d3d) {
    unsigned long long ts_begin = 0;
    unsigned long long ts_end   = 0;
    ID3D11DeviceContext *c = d3d->context;
    ID3D11Asynchronous *aS = (ID3D11Asynchronous*)d3d->debug.time0;
    ID3D11Asynchronous *aE = (ID3D11Asynchronous*)d3d->debug.time1;

    while (c->lpVtbl->GetData(c, aS, &ts_begin, sizeof(UINT64), 0) == S_FALSE);
    while (c->lpVtbl->GetData(c, aE, &ts_end, sizeof(UINT64), 0) == S_FALSE);

    return (ts_end - ts_begin);
}