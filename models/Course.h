#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct Course
    {
        int id;
        char name[MAX_COURSE_NAME_LENGTH];
        int creatorTeacherId;
        int totalLessons;
        int completedLessons;

        int themeIds[MAX_COURSE_THEMES];
        int themeCount;

        int testIds[MAX_COURSE_TESTS];
        int testCount;

        int enrolledStudentIds[MAX_COURSE_STUDENTS];
        int enrolledStudentCount;

        int assignedGroupIds[MAX_COURSE_GROUPS];
        int assignedGroupCount;
    };
}
