#include "cd3d11_internal.h"
#include "cd3d11.h"
#include <stdio.h>

void cd3d11_bind_shader(cd3d11 *d3d, void *shader, shader_stage_t stage) {
    if      (stage & STAGE_VERTEX) d3d->context->lpVtbl->VSSetShader(d3d->context, (ID3D11VertexShader*)shader, NULL, 0);
    else if (stage & STAGE_PIXEL) d3d->context->lpVtbl->PSSetShader(d3d->context, (ID3D11PixelShader*)shader, NULL, 0);
    else if (stage & STAGE_COMPUTE) d3d->context->lpVtbl->CSSetShader(d3d->context, (ID3D11ComputeShader*)shader, NULL, 0);
    else fprintf(stderr, "[CD3D11]: Unknown stage (%d)\n", stage);
}

void cd3d11_bind_reg(cd3d11 *d3d, void *ptr, shader_reg_t reg, int slot, shader_stage_t stage_bit) {
    if (stage_bit & STAGE_VERTEX) {
        if      (stage_bit & REG_B) d3d->context->lpVtbl->VSSetConstantBuffers(d3d->context, slot, 1, (ID3D11Buffer**)&ptr);
        else if (stage_bit & REG_T) d3d->context->lpVtbl->VSSetShaderResources(d3d->context, slot, 1, (ID3D11ShaderResourceView**)&ptr);
        else if (stage_bit & REG_S) d3d->context->lpVtbl->VSSetSamplers(d3d->context, slot, 1, (ID3D11SamplerState**)&ptr);
        else if (stage_bit & REG_U) fprintf(stderr, "[CD3D11]: Invalid usage of register (REG_U X VertexStage)\n");
        else fprintf(stderr, "[CD3D11]: Unknown register (%d)\n", reg);
    }
    if (stage_bit & STAGE_PIXEL) {
        if      (stage_bit & REG_B) d3d->context->lpVtbl->PSSetConstantBuffers(d3d->context, slot, 1, (ID3D11Buffer**)&ptr);
        else if (stage_bit & REG_T) d3d->context->lpVtbl->PSSetShaderResources(d3d->context, slot, 1, (ID3D11ShaderResourceView**)&ptr);
        else if (stage_bit & REG_S) d3d->context->lpVtbl->PSSetSamplers(d3d->context, slot, 1, (ID3D11SamplerState**)&ptr);
        else if (stage_bit & REG_U) fprintf(stderr, "[CD3D11]: Invalid usage of register (REG_U X PixelStage)\n");
        else fprintf(stderr, "[CD3D11]: Unknown register (%d)\n", reg);
    }
    if (stage_bit & STAGE_COMPUTE) {
        if      (stage_bit & REG_B) d3d->context->lpVtbl->CSSetConstantBuffers(d3d->context, slot, 1, (ID3D11Buffer**)&ptr);
        else if (stage_bit & REG_T) d3d->context->lpVtbl->CSSetShaderResources(d3d->context, slot, 1, (ID3D11ShaderResourceView**)&ptr);
        else if (stage_bit & REG_S) d3d->context->lpVtbl->CSSetSamplers(d3d->context, slot, 1, (ID3D11SamplerState**)&ptr);
        else if (stage_bit & REG_U) d3d->context->lpVtbl->CSSetUnorderedAccessViews(d3d->context, slot, 1, (ID3D11UnorderedAccessView**)&ptr, NULL);
        else fprintf(stderr, "[CD3D11]: Unknown register (%d)\n", reg);
    }
}