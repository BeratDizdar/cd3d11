#include "d11_internal.h"

static d11 *_d; // bu isim sıkıntı yaratacak gibi hissediyorum

void d11__release_if_exists(void *obj) {
    if (obj) ((IUnknown*)obj)->lpVtbl->Release(obj);
}

#include "d11_open.c"
#include "d11_object.c"
#include "d11_memory.c"
#include "d11_pipeline.c"
#include "d11_execute.c"
#include "d11_time.c"