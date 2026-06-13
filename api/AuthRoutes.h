#pragma once
#include "crow.h"
#include "JsonHelpers.h"
#include "../services/AuthService.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include "../models/AuthDto.h"

static crow::response errorResponse(int status, const char *message)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = message;
    return crow::response(status, body);
}

static crow::response successResponse(crow::json::wvalue &body)
{
    body["success"] = true;
    return crow::response(200, body);
}

namespace EduManage
{

    template <typename App>
    inline void setupAuthRoutes(App &app, AuthService &authService)
    {

        CROW_ROUTE(app, "/api/auth/register")
            .methods("POST"_method)([&authService](const crow::request &req)
                                    {

                auto body = crow::json::load(req.body);
                if (!body)
                    return errorResponse(400, "Некоректний JSON");

                if (!body.has("fullName") || !body.has("email") ||
                    !body.has("password") || !body.has("role"))
                    return errorResponse(400, "Поля fullName, email, password та role є обов'язковими");

                char fullName[MAX_NAME_LENGTH];
                char email[MAX_EMAIL_LENGTH];
                char password[MAX_PASSWORD_LENGTH];
                char role[16];

                fromJson(fullName, sizeof(fullName), body["fullName"]);
                fromJson(email,    sizeof(email),    body["email"]);
                fromJson(password, sizeof(password), body["password"]);
                fromJson(role,     sizeof(role),     body["role"]);

                
                bool isStudent = (strcmp(role, "student") == 0);
                bool isTeacher = (strcmp(role, "teacher") == 0);
                if (!isStudent && !isTeacher)
                    return errorResponse(400, "Поле role повинно бути \"student\" або \"teacher\"");

                auto nameCheck = Validator::validateFullName(fullName);
                if (!nameCheck.isValid) return errorResponse(400, nameCheck.errorMessage);

                auto emailCheck = Validator::validateEmail(email);
                if (!emailCheck.isValid) return errorResponse(400, emailCheck.errorMessage);

                auto passCheck = Validator::validatePassword(password);
                if (!passCheck.isValid) return errorResponse(400, passCheck.errorMessage);

                RegisterResult result = isStudent
                    ? authService.registerStudent(fullName, email, password)
                    : authService.registerTeacher(fullName, email, password);

                switch (result.code) {
                    case REGISTER_OK: {
                        crow::json::wvalue resp;
                        resp["userId"]   = result.user.id;
                        resp["fullName"] = result.user.fullName;
                        resp["role"]     = role;
                        resp["message"]  = "Реєстрацію успішно завершено";
                        return successResponse(resp);
                    }
                    case REGISTER_DUPLICATE_EMAIL:
                        return errorResponse(409, "Користувач з таким email вже існує");
                    case REGISTER_INVALID_NAME:
                        return errorResponse(400, "Некоректне ім'я");
                    case REGISTER_INVALID_EMAIL:
                        return errorResponse(400, "Некоректний email");
                    case REGISTER_INVALID_PASSWORD:
                        return errorResponse(400, "Некоректний пароль");
                    default:
                        return errorResponse(500, "Помилка сервера при реєстрації");
                } });

        CROW_ROUTE(app, "/api/auth/register/student")
            .methods("POST"_method)([&authService](const crow::request &req)
                                    {

                auto body = crow::json::load(req.body);
                if (!body)
                    return errorResponse(400, "Некоректний JSON");

                if (!body.has("fullName") || !body.has("email") || !body.has("password"))
                    return errorResponse(400, "Поля fullName, email та password є обов'язковими");

                char fullName[MAX_NAME_LENGTH];
                char email[MAX_EMAIL_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                fromJson(fullName, sizeof(fullName), body["fullName"]);
                fromJson(email,    sizeof(email),    body["email"]);
                fromJson(password, sizeof(password), body["password"]);

                auto nameCheck = Validator::validateFullName(fullName);
                if (!nameCheck.isValid) return errorResponse(400, nameCheck.errorMessage);

                auto emailCheck = Validator::validateEmail(email);
                if (!emailCheck.isValid) return errorResponse(400, emailCheck.errorMessage);

                auto passCheck = Validator::validatePassword(password);
                if (!passCheck.isValid) return errorResponse(400, passCheck.errorMessage);

                RegisterResult result = authService.registerStudent(fullName, email, password);

                switch (result.code) {
                    case REGISTER_OK: {
                        crow::json::wvalue resp;
                        resp["userId"]   = result.user.id;
                        resp["fullName"] = result.user.fullName;
                        resp["role"]     = "student";
                        resp["message"]  = "Реєстрацію успішно завершено";
                        return successResponse(resp);
                    }
                    case REGISTER_DUPLICATE_EMAIL:
                        return errorResponse(409, "Користувач з таким email вже існує");
                    case REGISTER_INVALID_NAME:
                        return errorResponse(400, "Некоректне ім'я");
                    case REGISTER_INVALID_EMAIL:
                        return errorResponse(400, "Некоректний email");
                    case REGISTER_INVALID_PASSWORD:
                        return errorResponse(400, "Некоректний пароль");
                    default:
                        return errorResponse(500, "Помилка сервера при реєстрації");
                } });

        CROW_ROUTE(app, "/api/auth/register/teacher")
            .methods("POST"_method)([&authService](const crow::request &req)
                                    {

                auto body = crow::json::load(req.body);
                if (!body)
                    return errorResponse(400, "Некоректний JSON");

                if (!body.has("fullName") || !body.has("email") || !body.has("password"))
                    return errorResponse(400, "Поля fullName, email та password є обов'язковими");

                char fullName[MAX_NAME_LENGTH];
                char email[MAX_EMAIL_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                fromJson(fullName, sizeof(fullName), body["fullName"]);
                fromJson(email,    sizeof(email),    body["email"]);
                fromJson(password, sizeof(password), body["password"]);

                auto nameCheck = Validator::validateFullName(fullName);
                if (!nameCheck.isValid) return errorResponse(400, nameCheck.errorMessage);

                auto emailCheck = Validator::validateEmail(email);
                if (!emailCheck.isValid) return errorResponse(400, emailCheck.errorMessage);

                auto passCheck = Validator::validatePassword(password);
                if (!passCheck.isValid) return errorResponse(400, passCheck.errorMessage);

                RegisterResult result = authService.registerTeacher(fullName, email, password);

                switch (result.code) {
                    case REGISTER_OK: {
                        crow::json::wvalue resp;
                        resp["userId"]   = result.user.id;
                        resp["fullName"] = result.user.fullName;
                        resp["role"]     = "teacher";
                        resp["message"]  = "Реєстрацію успішно завершено";
                        return successResponse(resp);
                    }
                    case REGISTER_DUPLICATE_EMAIL:
                        return errorResponse(409, "Користувач з таким email вже існує");
                    case REGISTER_INVALID_NAME:
                        return errorResponse(400, "Некоректне ім'я");
                    case REGISTER_INVALID_EMAIL:
                        return errorResponse(400, "Некоректний email");
                    case REGISTER_INVALID_PASSWORD:
                        return errorResponse(400, "Некоректний пароль");
                    default:
                        return errorResponse(500, "Помилка сервера при реєстрації");
                } });

        CROW_ROUTE(app, "/api/auth/login")
            .methods("POST"_method)([&authService](const crow::request &req)
                                    {

                auto body = crow::json::load(req.body);
                if (!body)
                    return errorResponse(400, "Некоректний JSON");

                if (!body.has("email") || !body.has("password"))
                    return errorResponse(400, "Поля email та password є обов'язковими");

                char email[MAX_EMAIL_LENGTH];
                char password[MAX_PASSWORD_LENGTH];

                fromJson(email,    sizeof(email),    body["email"]);
                fromJson(password, sizeof(password), body["password"]);

                auto emailCheck = Validator::validateEmail(email);
                if (!emailCheck.isValid) return errorResponse(400, emailCheck.errorMessage);

                LoginResult result = authService.login(email, password);

                switch (result.code) {
                    case LOGIN_OK: {
                        crow::json::wvalue resp;
                        resp["userId"]   = result.user.id;
                        resp["fullName"] = result.user.fullName;
                        resp["role"]     = (result.role == ACCOUNT_ROLE_TEACHER) ? "teacher" : "student";
                        resp["message"]  = "Вхід успішний";
                        return successResponse(resp);
                    }
                    case LOGIN_USER_NOT_FOUND:
                        return errorResponse(404, "Користувача з таким email не знайдено");
                    case LOGIN_WRONG_PASSWORD:
                        return errorResponse(401, "Неправильний пароль");
                    default:
                        return errorResponse(500, "Помилка сервера при вході");
                } });
    }

}
