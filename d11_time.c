__declspec(dllexport) void d11_time_start() {
    _d->ctx->lpVtbl->End(_d->ctx, (ID3D11Asynchronous*)_d->t0);
}

__declspec(dllexport) unsigned long long d11_time_elapsed() {
    _d->ctx->lpVtbl->End(_d->ctx, (ID3D11Asynchronous*)_d->t1);
    unsigned long long ts_begin = 0;
    unsigned long long ts_end   = 0;
    ID3D11DeviceContext *c = _d->ctx;
    ID3D11Asynchronous *aS = (ID3D11Asynchronous*)_d->t0;
    ID3D11Asynchronous *aE = (ID3D11Asynchronous*)_d->t1;

    while (c->lpVtbl->GetData(c, aS, &ts_begin, sizeof(UINT64), 0) == S_FALSE);
    while (c->lpVtbl->GetData(c, aE, &ts_end, sizeof(UINT64), 0) == S_FALSE);

    return (ts_end - ts_begin);
}