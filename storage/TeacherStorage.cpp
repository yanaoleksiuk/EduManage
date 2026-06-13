#include "TeacherStorage.h"

namespace EduManage
{
    bool TeacherStorage::getByEmail(const char *email, Teacher &teacher) const
    {
        ensureFileExists();

        std::ifstream input(filePath, std::ios::binary);
        if (!input.is_open())
        {
            return false;
        }

        Teacher current = {};
        while (input.read(reinterpret_cast<char *>(&current), sizeof(Teacher)))
        {
            if (textsEqual(current.email, email))
            {
                teacher = current;
                return true;
            }
        }

        return false;
    }

    bool TeacherStorage::emailExists(const char *email) const
    {
        Teacher teacher = {};
        return getByEmail(email, teacher);
    }
}
