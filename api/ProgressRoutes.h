#pragma once
#include "crow.h"
#include "../services/CourseService.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include <cstdlib>
#include <vector>

static crow::response progressErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    inline int countCompletedForCourse(const Student &student, const Course &course,
                                       CourseService &courseService)
    {
        int completed = 0;
        for (int ti = 0; ti < course.themeCount; ti++)
        {
            Theme theme{};
            if (!courseService.getThemeById(course.themeIds[ti], theme))
                continue;
            for (int li = 0; li < theme.lessonCount; li++)
            {
                int lid = theme.lessonIds[li];
                for (int ci = 0; ci < student.completedLessonCount; ci++)
                {
                    if (student.completedLessonIds[ci] == lid)
                    {
                        completed++;
                        break;
                    }
                }
            }
        }
        return completed;
    }

    inline crow::json::wvalue scheduleItemToJson(const ScheduleItem &item)
    {
        crow::json::wvalue j;
        j["lessonId"] = item.lessonId;
        j["courseId"] = item.courseId;
        j["courseName"] = item.courseName;
        j["lessonTitle"] = item.lessonTitle;
        j["date"] = item.date;
        j["time"] = item.time;
        return j;
    }

    template <typename App>
    inline void setupProgressRoutes(App &app, CourseService &courseService)
    {

        CROW_ROUTE(app, "/api/students/<int>/progress")
            .methods("GET"_method)([&courseService](int studentId)
                                   {

                auto idCheck = Validator::validatePositiveId(studentId);
                if (!idCheck.isValid) return progressErrorResponse(400, idCheck.errorMessage);

                Student student{};
                if (!courseService.getStudentById(studentId, student))
                    return progressErrorResponse(404, "Студента не знайдено");

                
                std::vector<crow::json::wvalue> courseList;
                for (int i = 0; i < student.enrolledCourseCount; i++) {
                    Course course{};
                    if (!courseService.getCourseById(student.enrolledCourseIds[i], course)) continue;

                    int    completed = countCompletedForCourse(student, course, courseService);
                    double percent   = (course.totalLessons > 0)
                                        ? completed * 100.0 / course.totalLessons
                                        : 0.0;

                    crow::json::wvalue cp;
                    cp["courseId"]         = course.id;
                    cp["courseName"]       = course.name;
                    cp["completedLessons"] = completed;
                    cp["totalLessons"]     = course.totalLessons;
                    cp["progressPercent"]  = percent;
                    courseList.push_back(std::move(cp));
                }

                crow::json::wvalue resp;
                resp["success"]       = true;
                resp["studentId"]     = student.id;
                resp["fullName"]      = student.fullName;
                resp["totalProgress"] = student.totalProgress;
                resp["courses"]       = std::move(courseList);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/courses/<int>/progress")
            .methods("GET"_method)([&courseService](const crow::request &req, int courseId)
                                   {

                auto idCheck = Validator::validatePositiveId(courseId);
                if (!idCheck.isValid) return progressErrorResponse(400, idCheck.errorMessage);

                const char* sidParam = req.url_params.get("studentId");
                if (!sidParam)
                    return progressErrorResponse(400, "Параметр studentId є обов'язковим (?studentId=X)");

                int studentId = std::atoi(sidParam);
                auto sc = Validator::validatePositiveId(studentId);
                if (!sc.isValid) return progressErrorResponse(400, "Некоректний studentId");

                Course course{};
                if (!courseService.getCourseById(courseId, course))
                    return progressErrorResponse(404, "Курс не знайдено");

                Student student{};
                if (!courseService.getStudentById(studentId, student))
                    return progressErrorResponse(404, "Студента не знайдено");

                int    completed = countCompletedForCourse(student, course, courseService);
                double percent   = (course.totalLessons > 0)
                                    ? completed * 100.0 / course.totalLessons
                                    : 0.0;

                crow::json::wvalue resp;
                resp["success"]          = true;
                resp["courseId"]         = course.id;
                resp["courseName"]       = course.name;
                resp["studentId"]        = student.id;
                resp["studentName"]      = student.fullName;
                resp["completedLessons"] = completed;
                resp["totalLessons"]     = course.totalLessons;
                resp["progressPercent"]  = percent;
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/students/<int>/schedule")
            .methods("GET"_method)([&courseService](int studentId)
                                   {

                auto idCheck = Validator::validatePositiveId(studentId);
                if (!idCheck.isValid) return progressErrorResponse(400, idCheck.errorMessage);

                ScheduleItem buffer[MAX_STORAGE_RECORDS];
                int count = courseService.getUpcomingScheduleForStudent(studentId, buffer, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < count; i++)
                    list.push_back(scheduleItemToJson(buffer[i]));

                crow::json::wvalue resp;
                resp["success"]  = true;
                resp["count"]    = count;
                resp["schedule"] = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/schedule")
            .methods("GET"_method)([&courseService](int teacherId)
                                   {

                auto idCheck = Validator::validatePositiveId(teacherId);
                if (!idCheck.isValid) return progressErrorResponse(400, idCheck.errorMessage);

                ScheduleItem buffer[MAX_STORAGE_RECORDS];
                int count = courseService.getUpcomingScheduleForTeacher(teacherId, buffer, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < count; i++)
                    list.push_back(scheduleItemToJson(buffer[i]));

                crow::json::wvalue resp;
                resp["success"]  = true;
                resp["count"]    = count;
                resp["schedule"] = std::move(list);
                return crow::response(200, resp); });
    }

}
