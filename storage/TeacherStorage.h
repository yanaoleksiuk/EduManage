#pragma once

#include "BinaryStorage.h"
#include "../models/Teacher.h"

namespace EduManage
{
    class TeacherStorage : public BinaryStorage<Teacher>
    {
    public:
        TeacherStorage() : BinaryStorage<Teacher>("data/teachers.dat") {}
        explicit TeacherStorage(const char *path) : BinaryStorage<Teacher>(path) {}

        bool getByEmail(const char *email, Teacher &teacher) const;
        bool emailExists(const char *email) const;
    };
}
