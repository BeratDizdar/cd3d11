#include "d11_internal.h"

static d11 *_d; // bu isim sıkıntı yaratacak gibi hissediyorum

void d11__release_if_exists(void *obj) {
    if (obj) ((IUnknown*)obj)->lpVtbl->Release(obj);
}

// belki biraz daha detaylı yaparım
void d11__check_result(long code, const char *ok_msg) {
    switch(code) {
        case S_OK: if (ok_msg) printf(ok_msg); break;
        case E_OUTOFMEMORY: printf("[D11]: E_OUTOFMEMORY!\n"); break;
        case E_INVALIDARG: printf("[D11]: E_INVALIDARG!\n"); break;
        case E_NOTIMPL: printf("[D11]: E_NOTIMPL!\n"); break;
        case DXGI_ERROR_WAS_STILL_DRAWING: printf("[D11]: DXGI_ERROR_WAS_STILL_DRAWING!\n"); break;
        case DXGI_ERROR_INVALID_CALL: printf("[D11]: DXGI_ERROR_INVALID_CALL!\n"); break;
        case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS: printf("[D11]: D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS!\n"); break;
        case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS: printf("[D11]: D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS!\n"); break;
        case D3D11_ERROR_FILE_NOT_FOUND: printf("[D11]: D3D11_ERROR_FILE_NOT_FOUND!\n"); break;
    }
}

#include "d11_open.c"
#include "d11_object.c"

// #############################################################################
//
// EXECUTE
//
// #############################################################################

__declspec(dllexport) void d11_draw(int vertex_count, int base_vertex) {
    _d->ctx->lpVtbl->Draw(_d->ctx, vertex_count, base_vertex);
}

__declspec(dllexport) void d11_clear(float r, float g, float b) {
    _d->ctx->lpVtbl->ClearRenderTargetView(_d->ctx, _d->rtv0, (const float[4]){r, g, b, 1});
}

__declspec(dllexport) void d11_present() {
    _d->swp->lpVtbl->Present(_d->swp, 0, 0);
}

// #############################################################################
//
// MEMORY
//
// #############################################################################

__declspec(dllexport) void d11_memcpy(int _Type, int _Copyflag, int _Slot, void *_Src, long long _Size) {
    D3D11_MAPPED_SUBRESOURCE m = {0};

    ID3D11Resource *target_res = (_Type == D11_TYPE_BUFFER) ? _d->resource[_Slot] : (ID3D11Resource*)_d->reg_b[_Slot];

    _d->ctx->lpVtbl->Map(_d->ctx, target_res, 0, _Copyflag, 0, &m);
    memcpy(m.pData, _Src, _Size);
    _d->ctx->lpVtbl->Unmap(_d->ctx, target_res, 0);
}

// #############################################################################
//
// PIPELINE
//
// #############################################################################

__declspec(dllexport) void d11_shader(int _Slot) {
    _d->ctx->lpVtbl->VSSetShader(_d->ctx, _d->vs[_Slot], NULL, 0);
    _d->ctx->lpVtbl->PSSetShader(_d->ctx, _d->ps[_Slot], NULL, 0);
}

__declspec(dllexport) void d11_topology(int _Type) {
    _d->ctx->lpVtbl->IASetPrimitiveTopology(_d->ctx, _Type);
}

// #############################################################################
//
// SHADER
//
// #############################################################################

__declspec(dllexport) void d11_new_shader(int _Slot, const void *_VData, long long _VSize, const void *_PData, long long _PSize) {
    if (_Slot < 0 || _Slot >= D11_MAX_SHADER_SLOT) {
        fprintf(stderr, "[D11]: invalid shader slot (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->vs[_Slot]);
    d11__release_if_exists(_d->ps[_Slot]);

    ID3D11VertexShader **v = (ID3D11VertexShader**)&_d->vs[_Slot];
    ID3D11PixelShader **p = (ID3D11PixelShader**)&_d->vs[_Slot];

    _d->result = _d->dev->lpVtbl->CreateVertexShader(_d->dev, _VData, _VSize, NULL, v);
    d11__check_result(_d->result, "[D11]: Vertex shader created\n");
    _d->result = _d->dev->lpVtbl->CreatePixelShader(_d->dev, _PData, _PSize, NULL, p);
    d11__check_result(_d->result, "[D11]: Pixel shader created\n");
}

__declspec(dllexport) void d11_new_shader_from_file(int _Slot, const char *_VPath, const char *_PPath) {
    if (_Slot < 0 || _Slot >= D11_MAX_SHADER_SLOT) {
        fprintf(stderr, "[D11]: invalid shader slot (%d)\n", _Slot);
        return;
    }
    d11__release_if_exists(_d->vs[_Slot]);
    d11__release_if_exists(_d->ps[_Slot]);

    FILE *_v = NULL, *_p = NULL;
    _v = fopen(_VPath, "rb");
    fseek(_v, 0, SEEK_END);
    long long _vsize = ftell(_v);
    rewind(_v);
    char *_vbuffer = malloc(_vsize+1);
    fread(_vbuffer, 1, _vsize, _v);
    _vbuffer[_vsize] = '\0';
    fclose(_v);

    _p = fopen(_PPath, "rb");
    fseek(_p, 0, SEEK_END);
    long long _psize = ftell(_p);
    rewind(_p);
    char *_pbuffer = malloc(_psize+1);
    fread(_pbuffer, 1, _psize, _p);
    _pbuffer[_psize] = '\0';
    fclose(_p);

    printf("[D11]: trying to load vs_5_0(%s) + ps_5_0(%s)...\n", _VPath, _PPath);
    d11_new_shader(_Slot, _vbuffer, _vsize, _pbuffer, _psize);
}

// #############################################################################
//
// TIME
//
// #############################################################################

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