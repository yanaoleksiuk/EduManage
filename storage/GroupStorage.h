#pragma once

#include "BinaryStorage.h"
#include "../models/Group.h"

namespace EduManage
{
    class GroupStorage : public BinaryStorage<Group>
    {
    public:
        GroupStorage() : BinaryStorage<Group>("data/groups.dat") {}
        explicit GroupStorage(const char *path) : BinaryStorage<Group>(path) {}
    };
}
