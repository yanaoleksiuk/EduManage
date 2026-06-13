#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct User
    {
        int id;
        char fullName[MAX_NAME_LENGTH];
        char email[MAX_EMAIL_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
    };
}
