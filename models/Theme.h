#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct Theme
    {
        int id;
        int courseId;
        char title[MAX_THEME_NAME_LENGTH];
        int lessonIds[MAX_THEME_LESSONS];
        int lessonCount;
    };
}
