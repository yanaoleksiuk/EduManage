#pragma once
#include <cstring>
#include "../common/ModelLimits.h"
#include "Course.h"

namespace EduManage
{

    struct CourseDto
    {
        int id;
        char name[MAX_COURSE_NAME_LENGTH];
        int creatorTeacherId;
        int totalLessons;
        int completedLessons;
        int enrolledStudentCount;
    };

    inline CourseDto toCourseDto(const Course &course)
    {
        CourseDto dto;
        dto.id = course.id;
        strncpy(dto.name, course.name, MAX_COURSE_NAME_LENGTH - 1);
        dto.name[MAX_COURSE_NAME_LENGTH - 1] = '\0';
        dto.creatorTeacherId = course.creatorTeacherId;
        dto.totalLessons = course.totalLessons;
        dto.completedLessons = course.completedLessons;
        dto.enrolledStudentCount = course.enrolledStudentCount;
        return dto;
    }

}
