#include "UserStorage.h"

namespace EduManage
{
    bool UserStorage::getByEmail(const char *email, User &user) const
    {
        ensureFileExists();

        std::ifstream input(filePath, std::ios::binary);
        if (!input.is_open())
        {
            return false;
        }

        User current = {};
        while (input.read(reinterpret_cast<char *>(&current), sizeof(User)))
        {
            if (textsEqual(current.email, email))
            {
                user = current;
                return true;
            }
        }

        return false;
    }

    bool UserStorage::emailExists(const char *email) const
    {
        User user = {};
        return getByEmail(email, user);
    }
}
