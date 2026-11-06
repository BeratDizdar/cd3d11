#include "cd3d11_internal.h"
#include "cd3d11.h"
#include <stdio.h>

void cd3d11_bind_srv(cd3d11 *d3d, void *view, int slot, int bind_vs, int bind_ps) {
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

void cd3d11_bind_sampler(cd3d11 *d3d, void *sampler, int slot, int bind_vs, int bind_ps) {
    ID3D11SamplerState *ss = sampler;
    if (bind_vs) {
        d3d->context->lpVtbl->VSSetSamplers(d3d->context, slot, 1, &ss);
    }
    if (bind_ps) {
        d3d->context->lpVtbl->PSSetSamplers(d3d->context, slot, 1, &ss);
    }
}

void cd3d11_shader(cd3d11 *d3d, void *shader, shader_stage_t stage) {
    if (stage & STAGE_VERTEX) d3d->context->lpVtbl->VSSetShader(d3d->context, (ID3D11VertexShader*)shader, NULL, 0);
    else if (stage & STAGE_PIXEL) d3d->context->lpVtbl->PSSetShader(d3d->context, (ID3D11PixelShader*)shader, NULL, 0);
    else if (stage & STAGE_COMPUTE) d3d->context->lpVtbl->CSSetShader(d3d->context, (ID3D11ComputeShader*)shader, NULL, 0);
    else fprintf(stderr, "[CD3D11]: Unknown stage (%d)\n", stage);
}

void cd3d11_reg(cd3d11 *d3d, void *ptr, shader_reg_t reg, int slot, shader_stage_t stage) {
    if (stage & STAGE_VERTEX) {
        if      (stage & REG_B) d3d->context->lpVtbl->VSSetConstantBuffers(d3d->context, slot, 1, (ID3D11Buffer**)&ptr);
        else if (stage & REG_T) d3d->context->lpVtbl->VSSetShaderResources(d3d->context, slot, 1, (ID3D11ShaderResourceView**)&ptr);
        else if (stage & REG_S) d3d->context->lpVtbl->VSSetSamplers(d3d->context, slot, 1, (ID3D11SamplerState**)&ptr);
        else if (stage & REG_U) fprintf(stderr, "[CD3D11]: Invalid usage of register (REG_U X VertexStage)\n");
        else fprintf(stderr, "[CD3D11]: Unknown register (%d)\n", reg);
    }
    if (stage & STAGE_PIXEL) {
        if      (stage & REG_B) d3d->context->lpVtbl->PSSetConstantBuffers(d3d->context, slot, 1, (ID3D11Buffer**)&ptr);
        else if (stage & REG_T) d3d->context->lpVtbl->PSSetShaderResources(d3d->context, slot, 1, (ID3D11ShaderResourceView**)&ptr);
        else if (stage & REG_S) d3d->context->lpVtbl->PSSetSamplers(d3d->context, slot, 1, (ID3D11SamplerState**)&ptr);
        else if (stage & REG_U) fprintf(stderr, "[CD3D11]: Invalid usage of register (REG_U X PixelStage)\n");
        else fprintf(stderr, "[CD3D11]: Unknown register (%d)\n", reg);
    }
    if (stage & STAGE_COMPUTE) {
        if      (stage & REG_B) d3d->context->lpVtbl->CSSetConstantBuffers(d3d->context, slot, 1, (ID3D11Buffer**)&ptr);
        else if (stage & REG_T) d3d->context->lpVtbl->CSSetShaderResources(d3d->context, slot, 1, (ID3D11ShaderResourceView**)&ptr);
        else if (stage & REG_S) d3d->context->lpVtbl->CSSetSamplers(d3d->context, slot, 1, (ID3D11SamplerState**)&ptr);
        else if (stage & REG_U) d3d->context->lpVtbl->CSSetUnorderedAccessViews(d3d->context, slot, 1, (ID3D11UnorderedAccessView**)&ptr, 1);
        else fprintf(stderr, "[CD3D11]: Unknown register (%d)\n", reg);
    }
}