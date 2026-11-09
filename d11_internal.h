#pragma once
#include <initguid.h>
#include <d3d11.h>
#include "d11.h"
#include <stdio.h>
#include <stdlib.h>

#define D11_MAX_SHADER_SLOT 16

typedef struct _d11 {
    void                   *lib;
    IDXGISwapChain         *swp;
    ID3D11Device           *dev;
    ID3D11DeviceContext    *ctx;

    ID3D11RenderTargetView *rtv0;
    ID3D11DepthStencilView *dsv0;

    ID3D11Query *t0;
    ID3D11Query *t1;

    ID3D11VertexShader *vs[D11_MAX_SHADER_SLOT];
    ID3D11PixelShader  *ps[D11_MAX_SHADER_SLOT];
    ID3D11Resource     *resource[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT];
    
    ID3D11Buffer             *reg_b[D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_COUNT];
    ID3D11ShaderResourceView *reg_t[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT];
    ID3D11SamplerState       *reg_s[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT];
} d11;
extern d11 *_d;

void d11__release_if_exists(void *obj);