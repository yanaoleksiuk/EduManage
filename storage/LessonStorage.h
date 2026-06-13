#pragma once

#include "BinaryStorage.h"
#include "../models/Lesson.h"

namespace EduManage
{
    class LessonStorage : public BinaryStorage<Lesson>
    {
    public:
        LessonStorage() : BinaryStorage<Lesson>("data/lessons.dat") {}
        explicit LessonStorage(const char *path) : BinaryStorage<Lesson>(path) {}
    };
}
