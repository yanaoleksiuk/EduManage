#pragma once

#include "BinaryStorage.h"
#include "../models/Theme.h"

namespace EduManage
{
    class ThemeStorage : public BinaryStorage<Theme>
    {
    public:
        ThemeStorage() : BinaryStorage<Theme>("data/themes.dat") {}
        explicit ThemeStorage(const char *path) : BinaryStorage<Theme>(path) {}
    };
}
