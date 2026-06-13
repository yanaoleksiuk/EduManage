#pragma once
#include "crow.h"
#include "JsonHelpers.h"
#include "../storage/StudentStorage.h"
#include "../storage/TeacherStorage.h"
#include "../services/AuthService.h"
#include "../models/UserDto.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include <cstring>

static crow::response userErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    inline crow::json::wvalue dtoToJson(const UserDto &dto)
    {
        crow::json::wvalue j;
        j["id"] = dto.id;
        j["fullName"] = dto.fullName;
        j["email"] = dto.email;
        j["role"] = dto.role;
        j["totalProgress"] = dto.totalProgress;
        j["enrolledCourseCount"] = dto.enrolledCourseCount;
        j["createdCourseCount"] = dto.createdCourseCount;
        return j;
    }

    template <typename App>
    inline void setupUserRoutes(App &app,
                                StudentStorage &studentStorage,
                                TeacherStorage &teacherStorage,
                                AuthService &authService)
    {

        CROW_ROUTE(app, "/api/users")
            .methods("GET"_method)([&studentStorage, &teacherStorage]()
                                   {
                Student sBuffer[MAX_STORAGE_RECORDS];
                Teacher tBuffer[MAX_STORAGE_RECORDS];
                int sCount = studentStorage.getAll(sBuffer, MAX_STORAGE_RECORDS);
                int tCount = teacherStorage.getAll(tBuffer, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < sCount; i++) list.push_back(dtoToJson(toUserDto(sBuffer[i])));
                for (int i = 0; i < tCount; i++) list.push_back(dtoToJson(toTeacherDto(tBuffer[i])));

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = sCount + tCount;
                resp["users"]   = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/users/<int>")
            .methods("GET"_method, "PUT"_method, "DELETE"_method)([&studentStorage, &teacherStorage, &authService](const crow::request &req, int id)
                                                                  {

                auto idCheck = Validator::validatePositiveId(id);
                if (!idCheck.isValid)
                    return userErrorResponse(400, idCheck.errorMessage);

                Student student{};
                Teacher teacher{};
                bool isStudent = studentStorage.getById(id, student);
                bool isTeacher = !isStudent && teacherStorage.getById(id, teacher);

                if (!isStudent && !isTeacher)
                    return userErrorResponse(404, "Користувача не знайдено");

                
                if (req.method == crow::HTTPMethod::Get) {
                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["user"]    = isStudent ? dtoToJson(toUserDto(student))
                                                : dtoToJson(toTeacherDto(teacher));
                    return crow::response(200, resp);
                }

                
                if (req.method == crow::HTTPMethod::Put) {
                    auto body = crow::json::load(req.body);
                    if (!body)
                        return userErrorResponse(400, "Некоректний JSON");

                    
                    const User& base = isStudent
                        ? static_cast<const User&>(student)
                        : static_cast<const User&>(teacher);

                    char newName[MAX_NAME_LENGTH];
                    char newEmail[MAX_EMAIL_LENGTH];
                    char newPass[MAX_PASSWORD_LENGTH];

                    if (body.has("fullName"))
                        fromJson(newName, sizeof(newName), body["fullName"]);
                    else {
                        strncpy(newName, base.fullName, sizeof(newName) - 1);
                        newName[sizeof(newName) - 1] = '\0';
                    }

                    if (body.has("email"))
                        fromJson(newEmail, sizeof(newEmail), body["email"]);
                    else {
                        strncpy(newEmail, base.email, sizeof(newEmail) - 1);
                        newEmail[sizeof(newEmail) - 1] = '\0';
                    }

                    if (body.has("password"))
                        fromJson(newPass, sizeof(newPass), body["password"]);
                    else {
                        strncpy(newPass, base.password, sizeof(newPass) - 1);
                        newPass[sizeof(newPass) - 1] = '\0';
                    }

                    auto check = Validator::validateFullName(newName);
                    if (!check.isValid) return userErrorResponse(400, check.errorMessage);
                    check = Validator::validateEmail(newEmail);
                    if (!check.isValid) return userErrorResponse(400, check.errorMessage);
                    check = Validator::validatePassword(newPass);
                    if (!check.isValid) return userErrorResponse(400, check.errorMessage);

                    auto result = isStudent
                        ? authService.updateStudentProfile(id, newName, newEmail, newPass)
                        : authService.updateTeacherProfile(id, newName, newEmail, newPass);

                    if (result.code != UPDATE_ACCOUNT_OK)
                        return userErrorResponse(409, "Помилка оновлення (можливо, email вже зайнятий)");

                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["message"] = "Дані оновлено";
                    return crow::response(200, resp);
                }

                
                if (req.method == crow::HTTPMethod::Delete) {
                    bool removed = isStudent ? studentStorage.removeById(id)
                                             : teacherStorage.removeById(id);
                    if (!removed)
                        return userErrorResponse(500, "Помилка видалення");

                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["message"] = "Користувача видалено";
                    return crow::response(200, resp);
                }

                return userErrorResponse(405, "Метод не підтримується"); });
    }

}
