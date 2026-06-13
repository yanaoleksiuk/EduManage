#pragma once

#include "BinaryStorage.h"
#include "../models/Question.h"

namespace EduManage
{
    class QuestionStorage : public BinaryStorage<Question>
    {
    public:
        QuestionStorage() : BinaryStorage<Question>("data/questions.dat") {}
        explicit QuestionStorage(const char *path) : BinaryStorage<Question>(path) {}
    };
}
