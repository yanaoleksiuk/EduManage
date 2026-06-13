#pragma once

#include "BinaryStorage.h"
#include "../models/User.h"

namespace EduManage
{
    class UserStorage : public BinaryStorage<User>
    {
    public:
        UserStorage() : BinaryStorage<User>("data/users.dat") {}
        explicit UserStorage(const char *path) : BinaryStorage<User>(path) {}

        bool getByEmail(const char *email, User &user) const;
        bool emailExists(const char *email) const;
    };
}
