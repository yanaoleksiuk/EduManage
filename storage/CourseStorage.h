#pragma once

#include "BinaryStorage.h"
#include "../models/Course.h"

namespace EduManage
{
    class CourseStorage : public BinaryStorage<Course>
    {
    public:
        CourseStorage() : BinaryStorage<Course>("data/courses.dat") {}
        explicit CourseStorage(const char *path) : BinaryStorage<Course>(path) {}
    };
}
