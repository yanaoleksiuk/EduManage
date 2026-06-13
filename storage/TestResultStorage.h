#pragma once
#include "BinaryStorage.h"
#include "../models/TestResult.h"
#include "../common/ModelLimits.h"

namespace EduManage
{
    class TestResultStorage : public BinaryStorage<TestResult>
    {
    public:
        TestResultStorage()
            : BinaryStorage<TestResult>("data/test_results.dat") {}
        explicit TestResultStorage(const char *path)
            : BinaryStorage<TestResult>(path) {}

        bool findByStudentAndTest(int studentId, int testId, TestResult &out) const
        {
            TestResult buf[MAX_STORAGE_RECORDS];
            int count = getAll(buf, MAX_STORAGE_RECORDS);
            for (int i = 0; i < count; ++i)
            {
                if (buf[i].studentId == studentId && buf[i].testId == testId)
                {
                    out = buf[i];
                    return true;
                }
            }
            return false;
        }

        int findByStudent(int studentId, TestResult *out, int maxCount) const
        {
            TestResult buf[MAX_STORAGE_RECORDS];
            int count = getAll(buf, MAX_STORAGE_RECORDS);
            int found = 0;
            for (int i = 0; i < count && found < maxCount; ++i)
            {
                if (buf[i].studentId == studentId)
                    out[found++] = buf[i];
            }
            return found;
        }
    };
}
