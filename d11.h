#pragma once
#define D11_TOPOLOGY_TRIANGLELIST  4
#define D11_TOPOLOGY_TRIANGLESTRIP 5
#define D11_TOPOLOGY_LINELIST      2
#define D11_TOPOLOGY_LINESTRIP     3
#define D11_TOPOLOGY_POINTLIST     1

// *_open.c

int *d11_open(void *hwnd, int w, int h);
void d11_make(void *d11_ptr);
void d11_close(void *d11_ptr);

// *_memory.c (şimdilik sadece input_resource için)

void d11_memcpy_discard(int _Slot, void *_Src, long long _Size);
void d11_memcpy_nooverwrite(int _Slot, void *_Src, long long _Size);

// *_object.c

void d11_new_cbuffer(int _Slot, const void *_Data, long long _Size);
void d11_new_structured_buffer(int _Slot, const void *_Data, long long _Size, int _Stride);
void d11_new_byte_address_buffer(int _Slot, const void *_Data, long long _Size);
void d11_new_texture2d(int _Slot, const void *_Data, int _Width, int _Height);
void d11_new_sampler(int _Slot, int _IsLinear);

// *_pipeline.c

void d11_shader(int _Slot); // şimdilik sadece 1 tane ekliyorum
void d11_topology(int _Type);

// *_execute.c

void d11_clear(float r, float g, float b);
void d11_draw(int vertex_count, int base_vertex);
void d11_present();

// *_time.c

void d11_time_start();
long long unsigned d11_time_elapsed();