#pragma once
#include "crow.h"
#include "JsonHelpers.h"
#include "../services/CourseService.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include <cstdio>
#include <vector>

static crow::response groupErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    inline crow::json::wvalue groupToJson(const Group &g)
    {
        crow::json::wvalue j;
        j["id"] = g.id;
        j["teacherId"] = g.teacherId;
        j["name"] = g.name;
        j["studentCount"] = g.studentCount;

        std::vector<crow::json::wvalue> ids;
        for (int i = 0; i < g.studentCount; i++)
            ids.push_back(crow::json::wvalue(g.studentIds[i]));
        j["studentIds"] = std::move(ids);
        return j;
    }

    template <typename App>
    inline void setupGroupRoutes(App &app, CourseService &courseService)
    {

        CROW_ROUTE(app, "/api/teachers/<int>/groups")
            .methods("POST"_method)([&courseService](const crow::request &req, int teacherId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return groupErrorResponse(400, tc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body) return groupErrorResponse(400, "Некоректний JSON");

                if (!body.has("name") || !body.has("studentIds"))
                    return groupErrorResponse(400, "Поля name та studentIds є обов'язковими");

                char name[MAX_NAME_LENGTH];
                fromJson(name, sizeof(name), body["name"]);

                auto nc = Validator::validateGroupName(name);
                if (!nc.isValid) return groupErrorResponse(400, nc.errorMessage);

                auto& idsJson = body["studentIds"];
                int count = static_cast<int>(idsJson.size());

                if (count == 0)
                    return groupErrorResponse(400, "Група повинна містити хоча б одного студента");
                if (count > MAX_GROUP_STUDENTS)
                {
                    char limitMsg[64];
                    snprintf(limitMsg, sizeof(limitMsg),
                             "Перевищено максимальну кількість студентів у групі (%d)", MAX_GROUP_STUDENTS);
                    return groupErrorResponse(400, limitMsg);
                }

                int studentIds[MAX_GROUP_STUDENTS];
                for (int i = 0; i < count; i++)
                    studentIds[i] = static_cast<int>(idsJson[i].i());

                CreateGroupResult result = courseService.createGroup(teacherId, name, studentIds, count);

                switch (result.code) {
                    case CREATE_GROUP_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Групу створено";
                        resp["group"]   = groupToJson(result.group);
                        return crow::response(201, resp);
                    }
                    case CREATE_GROUP_INVALID_TEACHER:
                        return groupErrorResponse(404, "Викладача не знайдено");
                    case CREATE_GROUP_INVALID_NAME:
                        return groupErrorResponse(400, "Назва групи некоректна");
                    case CREATE_GROUP_INVALID_STUDENTS:
                        return groupErrorResponse(400, "Один або кілька студентів не знайдено або не у вашому списку");
                    case CREATE_GROUP_LIMIT_REACHED:
                        return groupErrorResponse(409, "Досягнуто максимальну кількість груп");
                    default:
                        return groupErrorResponse(500, "Помилка сервера");
                } });

        CROW_ROUTE(app, "/api/teachers/<int>/groups/<int>")
            .methods("DELETE"_method)([&courseService](int teacherId, int groupId)
                                      {
                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return groupErrorResponse(400, tc.errorMessage);
                auto gc = Validator::validatePositiveId(groupId);
                if (!gc.isValid) return groupErrorResponse(400, gc.errorMessage);

                if (!courseService.deleteGroup(teacherId, groupId))
                    return groupErrorResponse(404, "Групу не знайдено або немає прав на видалення");

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["message"] = "Групу видалено";
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/groups")
            .methods("GET"_method)([&courseService](int teacherId)
                                   {

                auto idCheck = Validator::validatePositiveId(teacherId);
                if (!idCheck.isValid) return groupErrorResponse(400, idCheck.errorMessage);

                Group buffer[MAX_STORAGE_RECORDS];
                int count = courseService.getTeacherGroups(teacherId, buffer, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < count; i++)
                    list.push_back(groupToJson(buffer[i]));

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = count;
                resp["groups"]  = std::move(list);
                return crow::response(200, resp); });
    }

}
