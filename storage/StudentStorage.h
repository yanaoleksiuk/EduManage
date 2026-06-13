#pragma once

#include "BinaryStorage.h"
#include "../models/Student.h"

namespace EduManage
{
    class StudentStorage : public BinaryStorage<Student>
    {
    public:
        StudentStorage() : BinaryStorage<Student>("data/students.dat") {}
        explicit StudentStorage(const char *path) : BinaryStorage<Student>(path) {}

        bool getByEmail(const char *email, Student &student) const;
        bool emailExists(const char *email) const;
    };
}
