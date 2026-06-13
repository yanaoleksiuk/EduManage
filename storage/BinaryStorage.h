#pragma once

#include <cstdio>
#include <fstream>

#include "../common/TextUtils.h"

namespace EduManage
{
    template <typename T>
    class BinaryStorage
    {
    public:
        explicit BinaryStorage(const char *path)
        {
            copyText(filePath, sizeof(filePath), path);
        }

        bool add(const T &record) const
        {
            ensureFileExists();

            T existing = {};
            if (getById(record.id, existing))
            {
                return false;
            }

            std::ofstream output(filePath, std::ios::binary | std::ios::app);
            if (!output.is_open())
            {
                return false;
            }

            output.write(reinterpret_cast<const char *>(&record), sizeof(T));
            return output.good();
        }

        int getAll(T *buffer, int maxCount) const
        {
            ensureFileExists();

            std::ifstream input(filePath, std::ios::binary);
            if (!input.is_open() || buffer == nullptr || maxCount <= 0)
            {
                return 0;
            }

            int count = 0;
            T current = {};
            while (input.read(reinterpret_cast<char *>(&current), sizeof(T)))
            {
                if (count >= maxCount)
                {
                    break;
                }

                buffer[count] = current;
                ++count;
            }

            return count;
        }

        bool getById(int id, T &record) const
        {
            ensureFileExists();

            std::ifstream input(filePath, std::ios::binary);
            if (!input.is_open())
            {
                return false;
            }

            T current = {};
            while (input.read(reinterpret_cast<char *>(&current), sizeof(T)))
            {
                if (current.id == id)
                {
                    record = current;
                    return true;
                }
            }

            return false;
        }

        bool update(const T &record) const
        {
            ensureFileExists();

            std::ifstream input(filePath, std::ios::binary);
            if (!input.is_open())
            {
                return false;
            }

            char tempPath[320] = {};
            copyText(tempPath, sizeof(tempPath), filePath);
            if (!appendText(tempPath, sizeof(tempPath), ".tmp"))
            {
                return false;
            }

            std::ofstream output(tempPath, std::ios::binary | std::ios::trunc);
            if (!output.is_open())
            {
                return false;
            }

            bool replaced = false;
            T current = {};
            while (input.read(reinterpret_cast<char *>(&current), sizeof(T)))
            {
                if (current.id == record.id)
                {
                    output.write(reinterpret_cast<const char *>(&record), sizeof(T));
                    replaced = true;
                }
                else
                {
                    output.write(reinterpret_cast<const char *>(&current), sizeof(T));
                }
            }

            input.close();
            output.close();

            if (!replaced)
            {
                std::remove(tempPath);
                return false;
            }

            std::remove(filePath);
            std::rename(tempPath, filePath);
            return true;
        }

        bool removeById(int id) const
        {
            ensureFileExists();

            std::ifstream input(filePath, std::ios::binary);
            if (!input.is_open())
            {
                return false;
            }

            char tempPath[320] = {};
            copyText(tempPath, sizeof(tempPath), filePath);
            if (!appendText(tempPath, sizeof(tempPath), ".tmp"))
            {
                return false;
            }

            std::ofstream output(tempPath, std::ios::binary | std::ios::trunc);
            if (!output.is_open())
            {
                return false;
            }

            bool removed = false;
            T current = {};
            while (input.read(reinterpret_cast<char *>(&current), sizeof(T)))
            {
                if (current.id == id)
                {
                    removed = true;
                    continue;
                }

                output.write(reinterpret_cast<const char *>(&current), sizeof(T));
            }

            input.close();
            output.close();

            if (!removed)
            {
                std::remove(tempPath);
                return false;
            }

            std::remove(filePath);
            std::rename(tempPath, filePath);
            return true;
        }

        int getNextId() const
        {
            ensureFileExists();

            std::ifstream input(filePath, std::ios::binary);
            if (!input.is_open())
            {
                return 1;
            }

            int maxId = 0;
            T current = {};
            while (input.read(reinterpret_cast<char *>(&current), sizeof(T)))
            {
                if (current.id > maxId)
                {
                    maxId = current.id;
                }
            }

            return maxId + 1;
        }

    protected:
        void ensureFileExists() const
        {
            std::ifstream input(filePath, std::ios::binary);
            if (input.good())
            {
                return;
            }

            std::ofstream output(filePath, std::ios::binary | std::ios::app);
        }

        char filePath[260];
    };
}
