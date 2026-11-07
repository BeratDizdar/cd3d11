#pragma once
#include <initguid.h>
#include <d3d11.h>

/* UNUTMA:
- Texture için de view (büyük ihtimaller d3d türlerini kullanacaksın)
- sampler binding
- blending zartu zurt
- dinamik r/w işi
*/

#define STACK_MEMORY_MAX 256

typedef enum {
    REG_B = 1 << 1,
    REG_T = 1 << 2,
    REG_S = 1 << 3,
    REG_U = 1 << 4,
} shader_reg_t;

typedef enum {
    STAGE_VERTEX  = 1 << 1,
    STAGE_PIXEL   = 1 << 2,
    STAGE_COMPUTE = 1 << 3,
} shader_stage_t;

typedef struct _cd3d11 {
    void                   *lib;
    IDXGISwapChain         *swapchain;
    ID3D11Device           *device;
    ID3D11DeviceContext    *context;
    ID3D11RenderTargetView *default_rtv;
    struct _debug_queries {
        ID3D11Query *time0;
        ID3D11Query *time1;
    } debug;
    struct _managed_memory {
        void *ptr[STACK_MEMORY_MAX];
        int top;
        int active; // bunu kullanmayı unutma
    } stack;
} cd3d11;

int cd3d11_open(void *hwnd, int w, int h, cd3d11 *d3d); // *_open.c
void cd3d11_close(cd3d11 *d3d);                         // *_close.c
void cd3d11_rs_viewport(cd3d11 *d3d, int w, int h);              // *_rs.c
void cd3d11_rs_state(cd3d11 *d3d, void *rasterstate);            // *_rs.c
void cd3d11_rs_scissor(cd3d11 *d3d, int l, int u, int r, int b); // *_rs.c

// void cd3d11_memcpy();

void cd3d11_topology(cd3d11 *d3d, D3D_PRIMITIVE_TOPOLOGY topology); // *_ia.c
// vertex, index, layout ???

void cd3d11_cmd_draw(cd3d11 *d3d, int vertex_count, int base_vertex);                                         // *_cmd.c
void cd3d11_cmd_drawindexed(cd3d11 *d3d, int index_count, int base_index, int base_vertex);                   // *_cmd.c
void cd3d11_cmd_drawinstanced(cd3d11 *d3d, int inst_v_count, int inst_count, int base_vertex, int base_inst); // *_cmd.c
void cd3d11_cmd_dispatch(cd3d11 *d3d, int x, int y, int z);                                                   // *_cmd.c
void cd3d11_cmd_present(cd3d11 *d3d, int sync_interval);                                                      // *_cmd.c
void cd3d11_cmd_clear_default(cd3d11 *d3d);                                                                   // *_cmd.c

void cd3d11_release_managed_memory(cd3d11 *d3d);                                                  // *_objlife.c
void *cd3d11_new_shader_m(cd3d11 *d3d, const void *data, long long size, shader_stage_t stage);   // *_objlife.c
void *cd3d11_new_cbuffer_m(cd3d11 *d3d, void *data, int size, int is_mut);                        // *_objlife.c
void *cd3d11_new_buffer_m(cd3d11 *d3d, void *data, int size, int stride, int is_mut, int is_raw); // *_objlife.c
void *cd3d11_new_sampler_m(cd3d11 *d3d, int is_linear);                                           // *_objlife.c
void *cd3d11_new_tex2d_m(cd3d11 *d3d, const void *data, int w, int h, int pitch, int is_mut);     // *_objlife.c
void *cd3d11_new_rasterstate_m(cd3d11 *d3d, int bSolid, D3D11_CULL_MODE cull, int bCcw);          // *_objlife.c
//TODO: void *cd3d11_new_blendstate_m(cd3d11 *d3d);
//TODO: void *cd3d11_new_depthstencilstate_m(cd3d11 *d3d);

void *cd3d11_get_srv_from_buffer_m(cd3d11 *d3d, void *buffer, int element_count, int is_raw); // *_view.c

void cd3d11_bind_shader(cd3d11 *d3d, void *shader, shader_stage_t stage);                           // *_bind.c
void cd3d11_bind_reg(cd3d11 *d3d, void *ptr, shader_reg_t reg, int slot, shader_stage_t stage_bit); // *_bind.c

void cd3d11_mark_time(cd3d11 *d3d);                             // *_time.c
unsigned long long cd3d11_wait_and_get_time_value(cd3d11 *d3d); // *_time.c
