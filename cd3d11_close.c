#include "cd3d11_internal.h"
#include <libloaderapi.h>

void cd3d11_close(cd3d11 *d3d) {
    for (int i = 0; i < STACK_MEMORY_MAX; i++) {
        cd3d11__safe_release_obj(d3d->stack.cell[i].ptr);
    }
    cd3d11__safe_release_obj(d3d->debug.time1);
    cd3d11__safe_release_obj(d3d->debug.time0);
    cd3d11__safe_release_obj(d3d->default_rtv);
    cd3d11__safe_release_obj(d3d->context);
    cd3d11__safe_release_obj(d3d->device);
    cd3d11__safe_release_obj(d3d->swapchain);
    FreeLibrary(d3d->lib);
}