#pragma once
#include "../common/ModelLimits.h"

namespace EduManage
{

    struct RegisterRequestDto
    {
        char fullName[MAX_NAME_LENGTH];
        char email[MAX_EMAIL_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
    };

    struct LoginRequestDto
    {
        char email[MAX_EMAIL_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
    };

}
