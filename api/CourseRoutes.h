#pragma once
#include "crow.h"
#include "../services/CourseService.h"
#include "../storage/CourseStorage.h"
#include "../storage/StudentStorage.h"
#include "../models/CourseDto.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include "JsonHelpers.h"
#include <cstdlib>
#include <cstring>
#include <vector>

static crow::response courseErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    inline crow::json::wvalue courseDtoToJson(const CourseDto &dto)
    {
        crow::json::wvalue j;
        j["id"] = dto.id;
        j["name"] = dto.name;
        j["creatorTeacherId"] = dto.creatorTeacherId;
        j["totalLessons"] = dto.totalLessons;
        j["completedLessons"] = dto.completedLessons;
        j["enrolledStudentCount"] = dto.enrolledStudentCount;
        return j;
    }

    template <typename App>
    inline void setupCourseRoutes(App &app,
                                  CourseStorage &courseStorage,
                                  CourseService &courseService,
                                  StudentStorage &studentStorage)
    {

        CROW_ROUTE(app, "/api/courses")
            .methods("GET"_method)([&courseStorage](const crow::request &)
                                   {
                Course buffer[200];
                int count = courseStorage.getAll(buffer, 200);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < count; i++)
                    list.push_back(courseDtoToJson(toCourseDto(buffer[i])));

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = count;
                resp["courses"] = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/courses")
            .methods("POST"_method)([&courseService](const crow::request &req, int teacherId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return courseErrorResponse(400, tc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body)
                    return courseErrorResponse(400, "Некоректний JSON");

                if (!body.has("name"))
                    return courseErrorResponse(400, "Поле name є обов'язковим");

                char name[MAX_COURSE_NAME_LENGTH];
                fromJson(name, sizeof(name), body["name"]);

                auto nameCheck = Validator::validateCourseName(name);
                if (!nameCheck.isValid) return courseErrorResponse(400, nameCheck.errorMessage);

                CreateCourseResult result = courseService.createCourse(teacherId, name);

                switch (result.code) {
                    case CREATE_COURSE_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Курс створено";
                        resp["course"]  = courseDtoToJson(toCourseDto(result.course));
                        return crow::response(201, resp);
                    }
                    case CREATE_COURSE_INVALID_TEACHER:
                        return courseErrorResponse(404, "Викладача не знайдено або ID не належить викладачу");
                    case CREATE_COURSE_INVALID_NAME:
                        return courseErrorResponse(400, "Назва курсу некоректна");
                    case CREATE_COURSE_LIMIT_REACHED:
                        return courseErrorResponse(409, "Викладач досяг максимальної кількості курсів");
                    default:
                        return courseErrorResponse(500, "Помилка сервера");
                } });

        CROW_ROUTE(app, "/api/courses/<int>")
            .methods("GET"_method, "PUT"_method, "DELETE"_method)([&courseStorage](const crow::request &req, int id)
                                                                  {

                auto idCheck = Validator::validatePositiveId(id);
                if (!idCheck.isValid)
                    return courseErrorResponse(400, idCheck.errorMessage);

                if (req.method == crow::HTTPMethod::Get) {
                    Course course{};
                    if (!courseStorage.getById(id, course))
                        return courseErrorResponse(404, "Курс не знайдено");

                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["course"]  = courseDtoToJson(toCourseDto(course));
                    return crow::response(200, resp);
                }

                if (req.method == crow::HTTPMethod::Put) {
                    auto body = crow::json::load(req.body);
                    if (!body)
                        return courseErrorResponse(400, "Некоректний JSON");

                    if (!body.has("name"))
                        return courseErrorResponse(400, "Поле name є обов'язковим");

                    Course course{};
                    if (!courseStorage.getById(id, course))
                        return courseErrorResponse(404, "Курс не знайдено");

                    char newName[MAX_COURSE_NAME_LENGTH];
                    fromJson(newName, sizeof(newName), body["name"]);
                    auto nameCheck = Validator::validateCourseName(newName);
                    if (!nameCheck.isValid) return courseErrorResponse(400, nameCheck.errorMessage);

                    strncpy(course.name, newName, MAX_COURSE_NAME_LENGTH - 1);
                    course.name[MAX_COURSE_NAME_LENGTH - 1] = '\0';

                    if (!courseStorage.update(course))
                        return courseErrorResponse(500, "Помилка оновлення курсу");

                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["message"] = "Курс оновлено";
                    resp["course"]  = courseDtoToJson(toCourseDto(course));
                    return crow::response(200, resp);
                }

                if (req.method == crow::HTTPMethod::Delete) {
                    Course course{};
                    if (!courseStorage.getById(id, course))
                        return courseErrorResponse(404, "Курс не знайдено");

                    if (!courseStorage.removeById(id))
                        return courseErrorResponse(500, "Помилка видалення курсу");

                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["message"] = "Курс видалено";
                    return crow::response(200, resp);
                }

                return courseErrorResponse(405, "Метод не підтримується"); });

        CROW_ROUTE(app, "/api/courses/search")
            .methods("GET"_method)([&courseService](const crow::request &req)
                                   {

                const char* qParam = req.url_params.get("q");
                char query[MAX_SEARCH_TEXT_LENGTH] = {};
                if (qParam)
                    strncpy(query, qParam, sizeof(query) - 1);

                Course buffer[MAX_STORAGE_RECORDS];
                int count;
                if (query[0] == '\0')
                    count = courseService.getAllCourses(buffer, MAX_STORAGE_RECORDS);
                else
                    count = courseService.searchCourses(query, buffer, MAX_STORAGE_RECORDS);

                const char* tParam = req.url_params.get("teacherId");
                const char* sParam = req.url_params.get("studentId");
                int filterTeacher  = tParam ? std::atoi(tParam) : 0;
                int filterStudent  = sParam ? std::atoi(sParam) : 0;

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < count; i++) {
                    if (filterTeacher > 0 && buffer[i].creatorTeacherId != filterTeacher)
                        continue;
                    if (filterStudent > 0) {
                        bool enrolled = false;
                        for (int j = 0; j < buffer[i].enrolledStudentCount; j++) {
                            if (buffer[i].enrolledStudentIds[j] == filterStudent) {
                                enrolled = true; break;
                            }
                        }
                        if (!enrolled) continue;
                    }
                    list.push_back(courseDtoToJson(toCourseDto(buffer[i])));
                }

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = (int)list.size();
                resp["courses"] = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/courses/<int>/enroll-options")
            .methods("GET"_method)([&courseService](int teacherId, int courseId)
                                   {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return courseErrorResponse(400, tc.errorMessage);

                auto cc = Validator::validatePositiveId(courseId);
                if (!cc.isValid) return courseErrorResponse(400, cc.errorMessage);

                Course course{};
                if (!courseService.getCourseById(courseId, course))
                    return courseErrorResponse(404, "Курс не знайдено");

                Student sBuffer[MAX_STORAGE_RECORDS];
                int sCount = courseService.getTeacherStudents(teacherId, sBuffer, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> availStudents;
                for (int i = 0; i < sCount; i++) {
                    bool alreadyEnrolled = false;
                    for (int j = 0; j < course.enrolledStudentCount; j++) {
                        if (course.enrolledStudentIds[j] == sBuffer[i].id) {
                            alreadyEnrolled = true; break;
                        }
                    }
                    if (!alreadyEnrolled) {
                        crow::json::wvalue s;
                        s["id"]       = sBuffer[i].id;
                        s["fullName"] = sBuffer[i].fullName;
                        s["email"]    = sBuffer[i].email;
                        availStudents.push_back(std::move(s));
                    }
                }

                Group gBuffer[MAX_STORAGE_RECORDS];
                int gCount = courseService.getTeacherGroups(teacherId, gBuffer, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> availGroups;
                for (int i = 0; i < gCount; i++) {
                    bool alreadyAssigned = false;
                    for (int j = 0; j < course.assignedGroupCount; j++) {
                        if (course.assignedGroupIds[j] == gBuffer[i].id) {
                            alreadyAssigned = true; break;
                        }
                    }
                    if (!alreadyAssigned) {
                        crow::json::wvalue g;
                        g["id"]           = gBuffer[i].id;
                        g["name"]         = gBuffer[i].name;
                        g["studentCount"] = gBuffer[i].studentCount;
                        availGroups.push_back(std::move(g));
                    }
                }

                crow::json::wvalue resp;
                resp["success"]           = true;
                resp["courseId"]          = courseId;
                resp["availableStudents"] = std::move(availStudents);
                resp["availableGroups"]   = std::move(availGroups);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/courses/<int>/enroll")
            .methods("POST"_method)([&courseService, &studentStorage](const crow::request &req, int teacherId, int courseId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return courseErrorResponse(400, tc.errorMessage);

                auto cc = Validator::validatePositiveId(courseId);
                if (!cc.isValid) return courseErrorResponse(400, cc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body)
                    return courseErrorResponse(400, "Некоректний JSON");

                if (!body.has("studentId") && !body.has("studentEmail"))
                    return courseErrorResponse(400, "Потрібно вказати studentId або studentEmail");

                int studentId = 0;
                if (body.has("studentId")) {
                    studentId = static_cast<int>(body["studentId"].i());
                    auto sc = Validator::validatePositiveId(studentId);
                    if (!sc.isValid) return courseErrorResponse(400, sc.errorMessage);
                } else {
                    char studentEmail[MAX_EMAIL_LENGTH];
                    fromJson(studentEmail, sizeof(studentEmail), body["studentEmail"]);
                    auto ec = Validator::validateEmail(studentEmail);
                    if (!ec.isValid) return courseErrorResponse(400, ec.errorMessage);

                    Student found{};
                    if (!studentStorage.getByEmail(studentEmail, found))
                        return courseErrorResponse(404, "Студента з таким email не знайдено");
                    studentId = found.id;
                }

                GrantCourseAccessResult result =
                    courseService.grantCourseAccessToStudent(teacherId, courseId, studentId);

                switch (result.code) {
                    case GRANT_COURSE_ACCESS_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Студенту надано доступ до курсу";
                        return crow::response(200, resp);
                    }
                    case GRANT_COURSE_ACCESS_INVALID_TEACHER:
                        return courseErrorResponse(403, "Викладача не знайдено або немає прав");
                    case GRANT_COURSE_ACCESS_INVALID_COURSE:
                        return courseErrorResponse(404, "Курс не знайдено");
                    case GRANT_COURSE_ACCESS_INVALID_STUDENT:
                        return courseErrorResponse(404, "Студента не знайдено");
                    case GRANT_COURSE_ACCESS_ALREADY_GRANTED:
                        return courseErrorResponse(409, "Студент вже має доступ до цього курсу");
                    case GRANT_COURSE_ACCESS_LIMIT_REACHED:
                        return courseErrorResponse(409, "Досягнуто максимальну кількість студентів на курсі");
                    default:
                        return courseErrorResponse(500, "Помилка сервера при наданні доступу");
                } });

        CROW_ROUTE(app, "/api/teachers/<int>/courses/<int>/enroll-group")
            .methods("POST"_method)([&courseService](const crow::request &req, int teacherId, int courseId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return courseErrorResponse(400, tc.errorMessage);

                auto cc = Validator::validatePositiveId(courseId);
                if (!cc.isValid) return courseErrorResponse(400, cc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body)
                    return courseErrorResponse(400, "Некоректний JSON");

                if (!body.has("groupId"))
                    return courseErrorResponse(400, "Поле groupId є обов'язковим");

                int groupId = static_cast<int>(body["groupId"].i());
                auto gc = Validator::validatePositiveId(groupId);
                if (!gc.isValid) return courseErrorResponse(400, gc.errorMessage);

                GrantCourseAccessResult result =
                    courseService.grantCourseAccessToGroup(teacherId, courseId, groupId);

                switch (result.code) {
                    case GRANT_COURSE_ACCESS_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Групу записано на курс";
                        return crow::response(200, resp);
                    }
                    case GRANT_COURSE_ACCESS_INVALID_TEACHER:
                        return courseErrorResponse(403, "Викладача не знайдено або немає прав");
                    case GRANT_COURSE_ACCESS_INVALID_COURSE:
                        return courseErrorResponse(404, "Курс не знайдено");
                    case GRANT_COURSE_ACCESS_INVALID_GROUP:
                        return courseErrorResponse(404, "Групу не знайдено");
                    case GRANT_COURSE_ACCESS_ALREADY_GRANTED:
                        return courseErrorResponse(409, "Ця група вже має доступ до курсу");
                    case GRANT_COURSE_ACCESS_LIMIT_REACHED:
                        return courseErrorResponse(409, "Досягнуто максимальну кількість груп або студентів на курсі");
                    default:
                        return courseErrorResponse(500, "Помилка сервера");
                } });
    }

}
