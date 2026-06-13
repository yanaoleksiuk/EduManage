#pragma once

#include "BinaryStorage.h"
#include "../models/Test.h"

namespace EduManage
{
    class TestStorage : public BinaryStorage<Test>
    {
    public:
        TestStorage() : BinaryStorage<Test>("data/tests.dat") {}
        explicit TestStorage(const char *path) : BinaryStorage<Test>(path) {}
    };
}
