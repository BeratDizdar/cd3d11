#pragma once
#include "cd3d11.h"

static inline void cd3d11__safe_release_obj(void *o) {
    if (o) ((IUnknown*)o)->lpVtbl->Release(o);
}

static inline void cd3d11__push_stack(cd3d11 *d3d, void *obj, int is_mut) {
    if (d3d->stack.top >= STACK_MEMORY_MAX) return;

    d3d->stack.cell[d3d->stack.top++].ptr = obj;
}