#pragma once
#include "crow.h"
#include <cstring>

static void fromJson(char *dst, size_t dstSize, const crow::json::rvalue &val)
{
    auto rs = val.s();
    size_t len = rs.size() < (dstSize - 1) ? rs.size() : (dstSize - 1);
    memcpy(dst, rs.begin(), len);
    dst[len] = '\0';
}
