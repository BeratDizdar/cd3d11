#include "cd3d11_internal.h"

void *cd3d11_get_view_from_buffer_managed(cd3d11 *d3d, void *buffer, int element_count, int is_raw) {
    D3D11_SHADER_RESOURCE_VIEW_DESC sd = {0};
    sd.Format                = (is_raw) ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_UNKNOWN;
    sd.ViewDimension         = D3D11_SRV_DIMENSION_BUFFEREX;
    sd.BufferEx.FirstElement = 0;
    sd.BufferEx.NumElements  = element_count;
    sd.BufferEx.Flags        = (is_raw) ? D3D11_BUFFEREX_SRV_FLAG_RAW : 0;
    ID3D11ShaderResourceView *srv = NULL;
    d3d->device->lpVtbl->CreateShaderResourceView(d3d->device, (ID3D11Resource*)buffer, &sd, &srv);
    cd3d11__push_stack(d3d, srv, 0);
    return srv;
}