#pragma once
#define D11_TOPOLOGY_TRIANGLELIST  4
#define D11_TOPOLOGY_TRIANGLESTRIP 5
#define D11_TOPOLOGY_LINELIST      2
#define D11_TOPOLOGY_LINESTRIP     3
#define D11_TOPOLOGY_POINTLIST     1
#define D11_COPYFLAG_DISCARD     4
#define D11_COPYFLAG_NOOVERWRITE 5
#define D11_TYPE_CBUFFER 1
#define D11_TYPE_BUFFER  2

// *_open.c

void *d11_open(void *hwnd, int w, int h);
void d11_make(void *d11_ptr);
void d11_close(void *d11_ptr);

// *_memory.c (şimdilik sadece input_resource için)

void d11_memcpy(int _Type, int _Copyflag, int _Slot, void *_Src, long long _Size);

// *_object.c

void d11_new_cbuffer(int _Slot, const void *_Data, long long _Size);
void d11_new_structured_buffer(int _Slot, const void *_Data, long long _Size, int _Stride);
void d11_new_byte_address_buffer(int _Slot, const void *_Data, long long _Size);
void d11_new_texture2d(int _Slot, const void *_Data, int _Width, int _Height);
void d11_new_sampler(int _Slot, int _IsLinear);
void d11_new_shader(int _Slot, const void *_VData, long long _VSize, const void *_PData, long long _PSize);
void d11_new_shader_from_file(int _Slot, const char *_VShader, const char *_PShader);

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