#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct Group
    {
        int id;
        int teacherId;
        char name[MAX_NAME_LENGTH];
        int studentIds[MAX_GROUP_STUDENTS];
        int studentCount;
    };
}
