#include "d11_internal.h"

d11 *_d; // bu isim sıkıntı yaratacak gibi hissediyorum

void d11__release_if_exists(void *obj) {
    if (obj) ((IUnknown*)obj)->lpVtbl->Release(obj);
}