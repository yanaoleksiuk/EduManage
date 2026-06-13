#include "StudentStorage.h"

namespace EduManage
{
    bool StudentStorage::getByEmail(const char *email, Student &student) const
    {
        ensureFileExists();

        std::ifstream input(filePath, std::ios::binary);
        if (!input.is_open())
        {
            return false;
        }

        Student current = {};
        while (input.read(reinterpret_cast<char *>(&current), sizeof(Student)))
        {
            if (textsEqual(current.email, email))
            {
                student = current;
                return true;
            }
        }

        return false;
    }

    bool StudentStorage::emailExists(const char *email) const
    {
        Student student = {};
        return getByEmail(email, student);
    }
}
