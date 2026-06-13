#pragma once

#include "BinaryStorage.h"
#include "../models/HomeworkSubmission.h"
#include "../common/ModelLimits.h"

namespace EduManage
{
    class HomeworkStorage : public BinaryStorage<HomeworkSubmission>
    {
    public:
        HomeworkStorage() : BinaryStorage<HomeworkSubmission>("data/homework_submissions.dat") {}

        int getByLessonId(int lessonId, HomeworkSubmission *buf, int maxCount) const
        {
            HomeworkSubmission all[MAX_STORAGE_RECORDS];
            int total = getAll(all, MAX_STORAGE_RECORDS);
            int count = 0;
            for (int i = 0; i < total && count < maxCount; i++)
            {
                if (all[i].lessonId == lessonId)
                    buf[count++] = all[i];
            }
            return count;
        }

        bool findByStudentAndLesson(int studentId, int lessonId, HomeworkSubmission &out) const
        {
            HomeworkSubmission all[MAX_STORAGE_RECORDS];
            int total = getAll(all, MAX_STORAGE_RECORDS);
            for (int i = 0; i < total; i++)
            {
                if (all[i].studentId == studentId && all[i].lessonId == lessonId)
                {
                    out = all[i];
                    return true;
                }
            }
            return false;
        }

        bool upsert(HomeworkSubmission sub)
        {
            HomeworkSubmission existing{};
            if (findByStudentAndLesson(sub.studentId, sub.lessonId, existing))
            {
                sub.id = existing.id;
                return update(sub);
            }
            sub.id = getNextId();
            return add(sub);
        }
    };
}
