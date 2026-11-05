#pragma once
#include <d3d11.h>

/* UNUTMA:
- Texture için de view (büyük ihtimaller d3d türlerini kullanacaksın)
- sampler binding
- blending zartu zurt
*/

#define STACK_MEMORY_MAX 64

typedef struct _cd3d11 {
    IDXGISwapChain         *swapchain;
    ID3D11Device           *device;
    ID3D11DeviceContext    *context;
    ID3D11RenderTargetView *default_rtv;
    struct {
        ID3D11Query *time0;
        ID3D11Query *time1;
    } debug;
    struct {
        struct {
            void *ptr;
            int is_mutable;
        } cell[STACK_MEMORY_MAX];
        int top;
    } stack;
} cd3d11;

void cd3d11_open(void *hwnd, int w, int h, cd3d11 *d3d);            // *_open.c
void cd3d11_close(cd3d11 *d3d);                                     // *_close.c
void cd3d11_viewport(cd3d11 *d3d, int w, int h);                    // *_rs.c
void cd3d11_topology(cd3d11 *d3d, D3D_PRIMITIVE_TOPOLOGY topology); // *_ia.c
void cd3d11_draw(cd3d11 *d3d, int vertex_count, int base_vertex);   // *_cmd.c
void cd3d11_present(cd3d11 *d3d, int sync_interval);                // *_cmd.c
void cd3d11_clear_default(cd3d11 *d3d);                             // *_cmd.c

void cd3d11_release_managed(cd3d11 *d3d);                                                               // *_objlife.c
void *cd3d11_new_vertex_shader_managed(cd3d11 *d3d, const void *data, long long size);                  // *_objlife.c
void *cd3d11_new_pixel_shader_managed(cd3d11 *d3d, const void *data, long long size);                   // *_objlife.c
void *cd3d11_new_cbuffer_managed(cd3d11 *d3d, void *data, int size, int is_mut);                        // *_objlife.c
void *cd3d11_new_buffer_managed(cd3d11 *d3d, void *data, int size, int stride, int is_mut, int is_raw); // *_objlife.c
void *cd3d11_new_sampler_managed(cd3d11 *d3d, int is_linear);                                           // *_objlife.c
void *cd3d11_new_tex2d_managed(cd3d11 *d3d, const void *data, int w, int h, int pitch, int is_mut);     // *_objlife.c

void *cd3d11_get_view_from_buffer_managed(cd3d11 *d3d, void *buffer, int element_count, int is_raw); // *_view.c

void cd3d11_bind_resource_view(cd3d11 *d3d, void *view, int slot, int bind_vs, int bind_ps); // *_bind.c
void cd3d11_bind_cbuffer(cd3d11 *d3d, void *cbuffer, int slot, int bind_vs, int bind_ps);    // *_bind.c
void cd3d11_bind_vertex_shader(cd3d11 *d3d, void *shader);                                   // *_bind.c
void cd3d11_bind_pixel_shader(cd3d11 *d3d, void *shader);                                    // *_bind.c

void cd3d11_mark_time(cd3d11 *d3d);                             // *_time.c
unsigned long long cd3d11_wait_and_get_time_value(cd3d11 *d3d); // *_time.c
