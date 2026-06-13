#pragma once
#include "../common/ModelLimits.h"

namespace EduManage
{
    struct TestResult
    {
        int id;
        int studentId;
        int testId;
        double score;
        double maxScore;
        bool passed;
        int answerMasks[MAX_TEST_QUESTIONS];
        int answerCount;
    };
}
