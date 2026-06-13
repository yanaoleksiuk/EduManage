#pragma once
#include "crow.h"
#include "JsonHelpers.h"
#include "../services/CourseService.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include <vector>

static crow::response teacherErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    template <typename App>
    inline void setupTeacherRoutes(App &app,
                                   CourseService &courseService)
    {

        CROW_ROUTE(app, "/api/teachers/<int>/students")
            .methods("GET"_method, "POST"_method)([&courseService](const crow::request &req, int teacherId)
                                                  {

                auto idCheck = Validator::validatePositiveId(teacherId);
                if (!idCheck.isValid)
                    return teacherErrorResponse(400, idCheck.errorMessage);

                
                if (req.method == crow::HTTPMethod::Get) {
                    Student buffer[MAX_STORAGE_RECORDS];
                    int count = courseService.getTeacherStudents(teacherId, buffer, MAX_STORAGE_RECORDS);

                    std::vector<crow::json::wvalue> list;
                    for (int i = 0; i < count; i++) {
                        crow::json::wvalue s;
                        s["id"]       = buffer[i].id;
                        s["fullName"] = buffer[i].fullName;
                        s["email"]    = buffer[i].email;
                        list.push_back(std::move(s));
                    }

                    crow::json::wvalue resp;
                    resp["success"]  = true;
                    resp["count"]    = count;
                    resp["students"] = std::move(list);
                    return crow::response(200, resp);
                }

                
                auto body = crow::json::load(req.body);
                if (!body)
                    return teacherErrorResponse(400, "Некоректний JSON");

                if (!body.has("studentEmail"))
                    return teacherErrorResponse(400, "Поле studentEmail є обов'язковим");

                char studentEmail[MAX_EMAIL_LENGTH];
                fromJson(studentEmail, sizeof(studentEmail), body["studentEmail"]);

                auto emailCheck = Validator::validateEmail(studentEmail);
                if (!emailCheck.isValid)
                    return teacherErrorResponse(400, emailCheck.errorMessage);

                AssignStudentResult result =
                    courseService.addTeacherStudentByEmail(teacherId, studentEmail);

                switch (result.code) {
                    case ASSIGN_STUDENT_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Студента додано до списку викладача";
                        return crow::response(200, resp);
                    }
                    case ASSIGN_STUDENT_INVALID_TEACHER:
                        return teacherErrorResponse(404, "Викладача не знайдено");
                    case ASSIGN_STUDENT_INVALID_STUDENT:
                        return teacherErrorResponse(404, "Студента з таким email не знайдено");
                    case ASSIGN_STUDENT_ALREADY_LINKED:
                        return teacherErrorResponse(409, "Студент вже є у списку цього викладача");
                    case ASSIGN_STUDENT_LIMIT_REACHED:
                        return teacherErrorResponse(409, "Досягнуто максимальну кількість студентів");
                    default:
                        return teacherErrorResponse(500, "Помилка сервера");
                } });

        CROW_ROUTE(app, "/api/teachers/<int>/students/<int>")
            .methods("DELETE"_method)([&courseService](int teacherId, int studentId)
                                      {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return teacherErrorResponse(400, tc.errorMessage);

                auto sc = Validator::validatePositiveId(studentId);
                if (!sc.isValid) return teacherErrorResponse(400, sc.errorMessage);

                bool ok = courseService.removeStudentFromTeacher(teacherId, studentId);
                if (!ok)
                    return teacherErrorResponse(404, "Студента не знайдено у списку цього викладача");

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["message"] = "Студента видалено зі списку";
                return crow::response(200, resp); });
    }

}
