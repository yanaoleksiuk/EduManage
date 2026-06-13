#pragma once
#include "crow.h"
#include "JsonHelpers.h"
#include "../services/CourseService.h"
#include "../models/HomeworkSubmission.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include <cstring>
#include <cstdlib>
#include <vector>

static crow::response themeErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    inline crow::json::wvalue themeToJson(const Theme &t)
    {
        crow::json::wvalue j;
        j["id"] = t.id;
        j["courseId"] = t.courseId;
        j["title"] = t.title;
        j["lessonCount"] = t.lessonCount;
        return j;
    }

    inline crow::json::wvalue lessonToJson(const Lesson &l)
    {
        crow::json::wvalue j;
        j["id"] = l.id;
        j["themeId"] = l.themeId;
        j["title"] = l.title;
        j["material"] = l.material;
        j["date"] = l.date;
        j["time"] = l.time;
        j["homeworkLink"] = l.homeworkLink;
        return j;
    }

    template <typename App>
    inline void setupThemeRoutes(App &app, CourseService &courseService)
    {

        CROW_ROUTE(app, "/api/courses/<int>/themes")
            .methods("GET"_method)([&courseService](int courseId)
                                   {

                auto idCheck = Validator::validatePositiveId(courseId);
                if (!idCheck.isValid) return themeErrorResponse(400, idCheck.errorMessage);

                Course course{};
                if (!courseService.getCourseById(courseId, course))
                    return themeErrorResponse(404, "Курс не знайдено");

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < course.themeCount; i++) {
                    Theme theme{};
                    if (courseService.getThemeById(course.themeIds[i], theme))
                        list.push_back(themeToJson(theme));
                }

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = (int)list.size();
                resp["themes"]  = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/courses/<int>/themes")
            .methods("POST"_method)([&courseService](const crow::request &req, int teacherId, int courseId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return themeErrorResponse(400, tc.errorMessage);

                auto cc = Validator::validatePositiveId(courseId);
                if (!cc.isValid) return themeErrorResponse(400, cc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body) return themeErrorResponse(400, "Некоректний JSON");

                if (!body.has("title"))
                    return themeErrorResponse(400, "Поле title є обов'язковим");

                char title[MAX_THEME_NAME_LENGTH];
                fromJson(title, sizeof(title), body["title"]);

                auto nc = Validator::validateThemeName(title);
                if (!nc.isValid) return themeErrorResponse(400, nc.errorMessage);

                CreateThemeResult result = courseService.createTheme(teacherId, courseId, title);

                switch (result.code) {
                    case CREATE_THEME_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Тему створено";
                        resp["theme"]   = themeToJson(result.theme);
                        return crow::response(201, resp);
                    }
                    case CREATE_THEME_INVALID_TEACHER:
                        return themeErrorResponse(403, "Викладача не знайдено або немає прав на цей курс");
                    case CREATE_THEME_INVALID_COURSE:
                        return themeErrorResponse(404, "Курс не знайдено");
                    case CREATE_THEME_INVALID_NAME:
                        return themeErrorResponse(400, "Назва теми некоректна");
                    case CREATE_THEME_LIMIT_REACHED:
                        return themeErrorResponse(409, "Досягнуто максимальну кількість тем у курсі");
                    default:
                        return themeErrorResponse(500, "Помилка сервера");
                } });

        CROW_ROUTE(app, "/api/themes/<int>/lessons")
            .methods("GET"_method)([&courseService](const crow::request &req, int themeId)
                                   {

                auto idCheck = Validator::validatePositiveId(themeId);
                if (!idCheck.isValid) return themeErrorResponse(400, idCheck.errorMessage);

                Theme theme{};
                if (!courseService.getThemeById(themeId, theme))
                    return themeErrorResponse(404, "Тему не знайдено");

                const char* sidParam = req.url_params.get("studentId");
                int studentId = 0;
                if (sidParam) studentId = std::atoi(sidParam);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < theme.lessonCount; i++) {
                    Lesson lesson{};
                    if (courseService.getLessonById(theme.lessonIds[i], lesson)) {
                        crow::json::wvalue lj = lessonToJson(lesson);
                        if (studentId > 0)
                            lj["completed"] = courseService.isLessonCompletedByStudent(studentId, lesson.id);
                        list.push_back(std::move(lj));
                    }
                }

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = (int)list.size();
                resp["lessons"] = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/themes/<int>/lessons")
            .methods("POST"_method)([&courseService](const crow::request &req, int teacherId, int themeId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return themeErrorResponse(400, tc.errorMessage);

                auto tidCheck = Validator::validatePositiveId(themeId);
                if (!tidCheck.isValid) return themeErrorResponse(400, tidCheck.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body) return themeErrorResponse(400, "Некоректний JSON");

                if (!body.has("title") || !body.has("material"))
                    return themeErrorResponse(400, "Поля title та material є обов'язковими");

                char title[MAX_LESSON_TITLE_LENGTH];
                char material[MAX_MATERIAL_LENGTH];
                char date[MAX_DATE_LENGTH]         = {};
                char time[MAX_TIME_LENGTH]         = {};
                char homeworkLink[MAX_LINK_LENGTH] = {};

                fromJson(title,    sizeof(title),    body["title"]);
                fromJson(material, sizeof(material), body["material"]);
                if (body.has("date"))         fromJson(date,         sizeof(date),         body["date"]);
                if (body.has("time"))         fromJson(time,         sizeof(time),         body["time"]);
                if (body.has("homeworkLink")) fromJson(homeworkLink, sizeof(homeworkLink), body["homeworkLink"]);

                auto lc = Validator::validateLessonTitle(title);
                if (!lc.isValid) return themeErrorResponse(400, lc.errorMessage);

                CreateLessonResult result = courseService.createLesson(
                    teacherId, themeId,
                    title, material,
                    date, time,
                    homeworkLink
                );

                switch (result.code) {
                    case CREATE_LESSON_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Урок створено";
                        resp["lesson"]  = lessonToJson(result.lesson);
                        return crow::response(201, resp);
                    }
                    case CREATE_LESSON_INVALID_TEACHER:
                        return themeErrorResponse(403, "Викладача не знайдено або немає прав");
                    case CREATE_LESSON_INVALID_THEME:
                        return themeErrorResponse(404, "Тему не знайдено");
                    case CREATE_LESSON_INVALID_TITLE:
                        return themeErrorResponse(400, "Заголовок уроку некоректний");
                    case CREATE_LESSON_INVALID_MATERIAL:
                        return themeErrorResponse(400, "Матеріал уроку некоректний");
                    case CREATE_LESSON_LIMIT_REACHED:
                        return themeErrorResponse(409, "Досягнуто максимальну кількість уроків у темі");
                    default:
                        return themeErrorResponse(500, "Помилка сервера");
                } });

        CROW_ROUTE(app, "/api/lessons/<int>")
            .methods("GET"_method)([&courseService](const crow::request &req, int lessonId)
                                   {

                auto idCheck = Validator::validatePositiveId(lessonId);
                if (!idCheck.isValid) return themeErrorResponse(400, idCheck.errorMessage);

                Lesson lesson{};
                if (!courseService.getLessonById(lessonId, lesson))
                    return themeErrorResponse(404, "Урок не знайдено");

                crow::json::wvalue lj = lessonToJson(lesson);

                const char* sidParam = req.url_params.get("studentId");
                if (sidParam) {
                    int studentId = std::atoi(sidParam);
                    if (studentId > 0) {
                        HomeworkSubmission sub{};
                        bool submitted = courseService.getStudentSubmission(studentId, lessonId, sub);
                        lj["mySubmitted"]    = submitted;
                        lj["myHomeworkLink"] = submitted ? sub.homeworkLink : "";
                        lj["myCompleted"]    = courseService.isLessonCompletedByStudent(studentId, lessonId);
                    }
                }

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["lesson"]  = std::move(lj);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/lessons/<int>")
            .methods("PUT"_method, "DELETE"_method)([&courseService](const crow::request &req, int teacherId, int lessonId)
                                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return themeErrorResponse(400, tc.errorMessage);

                auto lc = Validator::validatePositiveId(lessonId);
                if (!lc.isValid) return themeErrorResponse(400, lc.errorMessage);

                
                if (req.method == crow::HTTPMethod::Put) {
                    auto body = crow::json::load(req.body);
                    if (!body) return themeErrorResponse(400, "Некоректний JSON");

                    Lesson current{};
                    if (!courseService.getLessonById(lessonId, current))
                        return themeErrorResponse(404, "Урок не знайдено");

                    char title[MAX_LESSON_TITLE_LENGTH];
                    char material[MAX_MATERIAL_LENGTH];
                    char date[MAX_DATE_LENGTH];
                    char time[MAX_TIME_LENGTH];
                    char homeworkLink[MAX_LINK_LENGTH];

                    if (body.has("title"))        fromJson(title,        sizeof(title),        body["title"]);
                    else { strncpy(title,        current.title,        sizeof(title) - 1);        title[sizeof(title) - 1]               = '\0'; }

                    if (body.has("material"))     fromJson(material,     sizeof(material),     body["material"]);
                    else { strncpy(material,     current.material,     sizeof(material) - 1);     material[sizeof(material) - 1]         = '\0'; }

                    if (body.has("date"))         
                    fromJson(date,         
                        sizeof(date),         
                        body["date"]);
                    else { 
                        strncpy(date,         
                            current.date,         
                            sizeof(date) - 1);         
                            date[sizeof(date) - 1]                 
                            = '\0'; }

                    if (body.has("time"))         
                    fromJson(time,         
                        sizeof(time),
                                 body["time"]);
                    else { 
                        strncpy(time, 
                        current.time,         
                        sizeof(time) - 1);         
                        time[sizeof(time) - 1]                 
                        = '\0'; }

                    if (body.has("homeworkLink")) fromJson(homeworkLink, sizeof(homeworkLink), body["homeworkLink"]);
                    else { strncpy(homeworkLink, current.homeworkLink, sizeof(homeworkLink) - 1); homeworkLink[sizeof(homeworkLink) - 1] = '\0'; }

                    bool ok = courseService.updateLesson(
                        teacherId, lessonId,
                        title, material,
                        date, time,
                        homeworkLink
                    );

                    if (!ok)
                        return themeErrorResponse(403, "Помилка оновлення (немає прав або урок не знайдено)");

                    Lesson updated{};
                    courseService.getLessonById(lessonId, updated);

                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["message"] = "Урок оновлено";
                    resp["lesson"]  = lessonToJson(updated);
                    return crow::response(200, resp);
                }

                
                bool ok = courseService.deleteLesson(teacherId, lessonId);
                if (!ok)
                    return themeErrorResponse(403, "Помилка видалення (немає прав або урок не знайдено)");

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["message"] = "Урок видалено";
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/students/<int>/lessons/<int>/complete")
            .methods("POST"_method)([&courseService](const crow::request &req, int studentId, int lessonId)
                                    {
            auto sc = Validator::validatePositiveId(studentId);
            if (!sc.isValid)
                return themeErrorResponse(400, sc.errorMessage);

            auto lc = Validator::validatePositiveId(lessonId);
            if (!lc.isValid)
                return themeErrorResponse(400, lc.errorMessage);

            bool ok = courseService.markLessonCompleted(studentId, lessonId);
            if (!ok)
                return themeErrorResponse(400, "Не вдалося позначити урок (вже пройдено або не знайдено студента/урок)");

            
            if (!req.body.empty()) {
                auto body = crow::json::load(req.body);
                if (body && body.has("homeworkLink")) {
                    char hwLink[MAX_LINK_LENGTH] = {};
                    fromJson(hwLink, sizeof(hwLink), body["homeworkLink"]);
                    if (hwLink[0] != '\0') {
                        
                        char studentName[MAX_NAME_LENGTH] = {};
                        courseService.getStudentName(studentId, studentName, sizeof(studentName));
                        courseService.saveHomeworkSubmission(studentId, lessonId, studentName, hwLink);
                    }
                }
            }

            crow::json::wvalue resp;
            resp["success"] = true;
            resp["message"] = "Урок позначено як пройдений";
            return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/themes/<int>")
            .methods("PUT"_method, "DELETE"_method)([&courseService](const crow::request &req, int teacherId, int themeId)
                                                    {
            auto tc = Validator::validatePositiveId(teacherId);
            if (!tc.isValid) return themeErrorResponse(400, tc.errorMessage);

            auto tmc = Validator::validatePositiveId(themeId);
            if (!tmc.isValid) return themeErrorResponse(400, tmc.errorMessage);

            if (req.method == crow::HTTPMethod::Delete) {
                bool ok = courseService.deleteTheme(teacherId, themeId);
                if (!ok)
                    return themeErrorResponse(403, "Помилка видалення (немає прав або тему не знайдено)");
                crow::json::wvalue resp;
                resp["success"] = true;
                resp["message"] = "Тему видалено";
                return crow::response(200, resp);
            }

            
            auto body = crow::json::load(req.body);
            if (!body) return themeErrorResponse(400, "Некоректний JSON");
            if (!body.has("title")) return themeErrorResponse(400, "Поле title є обов'язковим");

            char title[MAX_THEME_NAME_LENGTH];
            fromJson(title, sizeof(title), body["title"]);
            auto nc = Validator::validateThemeName(title);
            if (!nc.isValid) return themeErrorResponse(400, nc.errorMessage);

            bool ok = courseService.updateThemeTitle(teacherId, themeId, title);
            if (!ok)
                return themeErrorResponse(403, "Помилка оновлення (немає прав або тему не знайдено)");

            crow::json::wvalue resp;
            resp["success"] = true;
            resp["message"] = "Тему оновлено";
            return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/lessons/<int>/submissions")
            .methods("GET"_method)([&courseService](int teacherId, int lessonId)
                                   {
            auto tc = Validator::validatePositiveId(teacherId);
            if (!tc.isValid)
                return themeErrorResponse(400, tc.errorMessage);

            auto lc = Validator::validatePositiveId(lessonId);
            if (!lc.isValid)
                return themeErrorResponse(400, lc.errorMessage);

            HomeworkSubmission buf[MAX_STORAGE_RECORDS];
            int count = courseService.getSubmissionsForLesson(lessonId, buf, MAX_STORAGE_RECORDS);

            crow::json::wvalue resp;
            resp["success"] = true;
            std::vector<crow::json::wvalue> list;
            for (int i = 0; i < count; i++) {
                crow::json::wvalue s;
                s["studentId"]    = buf[i].studentId;
                s["studentName"]  = buf[i].studentName;
                s["homeworkLink"] = buf[i].homeworkLink;
                list.push_back(std::move(s));
            }
            resp["submissions"] = std::move(list);
            return crow::response(200, resp); });
    }

}
