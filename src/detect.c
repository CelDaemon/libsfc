#include "detect.h"


enum sfc_copier sfc_detect_copier(const size_t size)
{
    return (size & 512) > 0 ? SFC_CPY_TRUE : SFC_CPY_FALSE;
}
