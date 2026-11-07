#pragma once
#include "cd3d11.h"

static inline void cd3d11__safe_release_obj(void *o) {
    if (o) ((IUnknown*)o)->lpVtbl->Release(o);
}

static inline void cd3d11__push_stack(cd3d11 *d3d, void *obj) {
    if (d3d->stack.top >= STACK_MEMORY_MAX) return;

    d3d->stack.ptr[d3d->stack.top++] = obj;
}