#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct HomeworkSubmission
    {
        int id;
        int studentId;
        int lessonId;
        char studentName[MAX_NAME_LENGTH];
        char homeworkLink[MAX_LINK_LENGTH];
    };
}
