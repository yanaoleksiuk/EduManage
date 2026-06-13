#pragma once

#include <cctype>
#include <cstddef>
#include <cstring>

namespace EduManage
{
    inline void copyText(char *destination, std::size_t destinationSize, const char *source)
    {
        if (destination == nullptr || destinationSize == 0)
        {
            return;
        }

        if (source == nullptr)
        {
            destination[0] = '\0';
            return;
        }

        std::strncpy(destination, source, destinationSize - 1);
        destination[destinationSize - 1] = '\0';
    }

    inline bool isEmptyText(const char *text)
    {
        return text == nullptr || text[0] == '\0';
    }

    inline bool textsEqual(const char *left, const char *right)
    {
        if (left == nullptr || right == nullptr)
        {
            return left == right;
        }

        return std::strcmp(left, right) == 0;
    }

    inline bool appendText(char *destination, std::size_t destinationSize, const char *suffix)
    {
        if (destination == nullptr || suffix == nullptr || destinationSize == 0)
        {
            return false;
        }

        const std::size_t currentLength = std::strlen(destination);
        const std::size_t suffixLength = std::strlen(suffix);

        if (currentLength + suffixLength >= destinationSize)
        {
            return false;
        }

        std::memcpy(destination + currentLength, suffix, suffixLength + 1);
        return true;
    }

    inline char toLowerChar(char value)
    {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(value)));
    }

    inline bool containsTextIgnoreCase(const char *text, const char *pattern)
    {
        if (isEmptyText(pattern))
        {
            return true;
        }

        if (isEmptyText(text))
        {
            return false;
        }

        const std::size_t textLength = std::strlen(text);
        const std::size_t patternLength = std::strlen(pattern);
        if (patternLength > textLength)
        {
            return false;
        }

        for (std::size_t start = 0; start + patternLength <= textLength; ++start)
        {
            bool matches = true;
            for (std::size_t offset = 0; offset < patternLength; ++offset)
            {
                if (toLowerChar(text[start + offset]) != toLowerChar(pattern[offset]))
                {
                    matches = false;
                    break;
                }
            }

            if (matches)
            {
                return true;
            }
        }

        return false;
    }
}
