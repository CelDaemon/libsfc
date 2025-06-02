#include "detect.h"

bool sfc_detect_copier(const size_t size)
{
    return (size & 512) > 0 ? true : false;
}
