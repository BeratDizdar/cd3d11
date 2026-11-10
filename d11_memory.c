__declspec(dllexport) void d11_memcpy_discard(int _Slot, void *_Src, long long _Size) {
    D3D11_MAPPED_SUBRESOURCE m = {0};
    _d->ctx->lpVtbl->Map(_d->ctx, _d->resource[_Slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &m);
    memcpy(m.pData, _Src, _Size);
    _d->ctx->lpVtbl->Unmap(_d->ctx, _d->resource[_Slot], 0);
}

__declspec(dllexport) void d11_memcpy_nooverwrite(int _Slot, void *_Src, long long _Size) {
    D3D11_MAPPED_SUBRESOURCE m = {0};
    _d->ctx->lpVtbl->Map(_d->ctx, _d->resource[_Slot], 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &m);
    memcpy(m.pData, _Src, _Size);
    _d->ctx->lpVtbl->Unmap(_d->ctx, _d->resource[_Slot], 0);
}