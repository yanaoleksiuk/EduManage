#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct Lesson
    {
        int id;
        int themeId;
        char title[MAX_LESSON_TITLE_LENGTH];
        char material[MAX_MATERIAL_LENGTH];
        char date[MAX_DATE_LENGTH];
        char time[MAX_TIME_LENGTH];
        char homeworkLink[MAX_LINK_LENGTH];
    };
}
