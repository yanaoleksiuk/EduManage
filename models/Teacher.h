#pragma once

#include "../common/ModelLimits.h"
#include "User.h"

namespace EduManage
{
    struct Teacher : public User
    {
        int createdCourseIds[MAX_USER_CREATED_COURSES];
        int createdCourseCount;
        int managedStudentIds[MAX_TEACHER_STUDENTS];
        int managedStudentCount;
        int groupIds[MAX_TEACHER_GROUPS];
        int groupCount;
    };
}
