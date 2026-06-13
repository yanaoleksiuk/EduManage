#pragma once

#include "../common/ModelLimits.h"

namespace EduManage
{
    struct Test
    {
        int id;
        int courseId;
        char title[MAX_TEST_TITLE_LENGTH];
        double maxScore;
        int questionIds[MAX_TEST_QUESTIONS];
        int questionCount;
    };
}
