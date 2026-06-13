#pragma once
#include <cstring>
#include "../common/ModelLimits.h"
#include "Student.h"
#include "Teacher.h"

namespace EduManage
{

    struct UserDto
    {
        int id;
        char fullName[MAX_NAME_LENGTH];
        char email[MAX_EMAIL_LENGTH];
        char role[10];
        double totalProgress;
        int enrolledCourseCount;
        int createdCourseCount;
    };

    inline UserDto toUserDto(const Student &s)
    {
        UserDto dto;
        dto.id = s.id;
        strncpy(dto.fullName, s.fullName, MAX_NAME_LENGTH - 1);
        dto.fullName[MAX_NAME_LENGTH - 1] = '\0';
        strncpy(dto.email, s.email, MAX_EMAIL_LENGTH - 1);
        dto.email[MAX_EMAIL_LENGTH - 1] = '\0';
        strncpy(dto.role, "student", sizeof(dto.role) - 1);
        dto.role[sizeof(dto.role) - 1] = '\0';
        dto.totalProgress = s.totalProgress;
        dto.enrolledCourseCount = s.enrolledCourseCount;
        dto.createdCourseCount = 0;
        return dto;
    }

    inline UserDto toTeacherDto(const Teacher &t)
    {
        UserDto dto;
        dto.id = t.id;
        strncpy(dto.fullName, t.fullName, MAX_NAME_LENGTH - 1);
        dto.fullName[MAX_NAME_LENGTH - 1] = '\0';
        strncpy(dto.email, t.email, MAX_EMAIL_LENGTH - 1);
        dto.email[MAX_EMAIL_LENGTH - 1] = '\0';
        strncpy(dto.role, "teacher", sizeof(dto.role) - 1);
        dto.role[sizeof(dto.role) - 1] = '\0';
        dto.totalProgress = 0.0;
        dto.enrolledCourseCount = 0;
        dto.createdCourseCount = t.createdCourseCount;
        return dto;
    }

}
