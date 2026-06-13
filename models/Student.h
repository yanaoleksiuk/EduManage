#pragma once

#include "../common/ModelLimits.h"
#include "User.h"

namespace EduManage
{
    struct Student : public User
    {
        int enrolledCourseIds[MAX_USER_ENROLLED_COURSES];
        int enrolledCourseCount;
        int completedLessonIds[MAX_USER_COMPLETED_LESSONS];
        int completedLessonCount;
        int passedTestIds[MAX_USER_PASSED_TESTS];
        int passedTestCount;
        double passedTestScores[MAX_USER_PASSED_TESTS];
        double passedTestMaxScores[MAX_USER_PASSED_TESTS];
        double totalProgress;
    };
}
