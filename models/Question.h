#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct Question
    {
        int id;
        char text[MAX_QUESTION_TEXT_LENGTH];
        char options[MAX_QUESTION_OPTIONS][MAX_OPTION_TEXT_LENGTH];
        int optionCount;
        int correctOptionFlags[MAX_QUESTION_OPTIONS];
        double score;
    };
}
