#pragma once
#include "crow.h"
#include "JsonHelpers.h"
#include "../services/CourseService.h"
#include "../models/TestResult.h"
#include "../common/Validator.h"
#include "../common/ModelLimits.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>

static crow::response testErrorResponse(int status, const char *msg)
{
    crow::json::wvalue body;
    body["success"] = false;
    body["message"] = msg;
    return crow::response(status, body);
}

namespace EduManage
{

    inline crow::json::wvalue questionToJson(const Question &q)
    {
        crow::json::wvalue j;
        j["id"] = q.id;
        j["text"] = q.text;
        j["score"] = q.score;

        std::vector<crow::json::wvalue> opts;
        std::vector<crow::json::wvalue> flags;
        for (int i = 0; i < q.optionCount; i++)
        {
            opts.push_back(crow::json::wvalue(q.options[i]));
            flags.push_back(crow::json::wvalue(q.correctOptionFlags[i]));
        }
        j["options"] = std::move(opts);
        j["correctFlags"] = std::move(flags);
        return j;
    }

    inline crow::json::wvalue testToJson(const Test &t)
    {
        crow::json::wvalue j;
        j["id"] = t.id;
        j["courseId"] = t.courseId;
        j["title"] = t.title;
        j["maxScore"] = t.maxScore;
        j["questionCount"] = t.questionCount;
        return j;
    }

    template <typename App>
    inline void setupTestRoutes(App &app, CourseService &courseService)
    {

        CROW_ROUTE(app, "/api/courses/<int>/tests")
            .methods("GET"_method)([&courseService](const crow::request &req, int courseId)
                                   {

                auto idCheck = Validator::validatePositiveId(courseId);
                if (!idCheck.isValid) return testErrorResponse(400, idCheck.errorMessage);

                Course course{};
                if (!courseService.getCourseById(courseId, course))
                    return testErrorResponse(404, "Курс не знайдено");

                
                int studentId = 0;
                const char* sidParam = req.url_params.get("studentId");
                if (sidParam) studentId = std::atoi(sidParam);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < course.testCount; i++) {
                    Test test{};
                    if (!courseService.getTestById(course.testIds[i], test)) continue;

                    crow::json::wvalue tj = testToJson(test);

                    if (studentId > 0) {
                        TestResult tr{};
                        bool taken = courseService.getTestResult(studentId, test.id, tr);
                        tj["taken"]    = taken;
                        tj["score"]    = taken ? tr.score    : 0.0;
                        tj["maxScore"] = taken ? tr.maxScore : test.maxScore;
                        tj["passed"]   = taken ? tr.passed   : false;
                    }

                    list.push_back(std::move(tj));
                }

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = (int)list.size();
                resp["tests"]   = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/courses/<int>/tests")
            .methods("POST"_method)([&courseService](const crow::request &req, int teacherId, int courseId)
                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return testErrorResponse(400, tc.errorMessage);

                auto cc = Validator::validatePositiveId(courseId);
                if (!cc.isValid) return testErrorResponse(400, cc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body) return testErrorResponse(400, "Некоректний JSON");

                if (!body.has("title") || !body.has("questions"))
                    return testErrorResponse(400, "Поля title та questions є обов'язковими");

                char title[MAX_TEST_TITLE_LENGTH];
                fromJson(title, sizeof(title), body["title"]);

                auto ttc = Validator::validateTestTitle(title);
                if (!ttc.isValid) return testErrorResponse(400, ttc.errorMessage);

                auto& questionsJson = body["questions"];
                int qCount = static_cast<int>(questionsJson.size());

                if (qCount == 0)
                    return testErrorResponse(400, "Тест повинен мати хоча б одне запитання");
                if (qCount > MAX_TEST_QUESTIONS) {
                    char limitMsg[64];
                    snprintf(limitMsg, sizeof(limitMsg),
                             "Перевищено максимальну кількість запитань (%d)", MAX_TEST_QUESTIONS);
                    return testErrorResponse(400, limitMsg);
                }

                Question questions[MAX_TEST_QUESTIONS];
                for (int i = 0; i < qCount; i++) {
                    questions[i] = Question{};
                    auto& qj = questionsJson[i];

                    if (!qj.has("text") || !qj.has("options") || !qj.has("correctFlags"))
                        return testErrorResponse(400, "Кожне запитання повинно мати поля text, options та correctFlags");

                    fromJson(questions[i].text, MAX_QUESTION_TEXT_LENGTH, qj["text"]);
                    questions[i].text[MAX_QUESTION_TEXT_LENGTH - 1] = '\0';

                    auto& opts  = qj["options"];
                    auto& flags = qj["correctFlags"];

                    questions[i].optionCount = static_cast<int>(opts.size());
                    if (questions[i].optionCount > MAX_QUESTION_OPTIONS)
                        questions[i].optionCount = MAX_QUESTION_OPTIONS;

                    for (int k = 0; k < questions[i].optionCount; k++) {
                        fromJson(questions[i].options[k], MAX_OPTION_TEXT_LENGTH, opts[k]);
                        questions[i].options[k][MAX_OPTION_TEXT_LENGTH - 1] = '\0';
                        questions[i].correctOptionFlags[k] = static_cast<int>(flags[k].i());
                    }

                    questions[i].score = qj.has("score") ? qj["score"].d() : 1.0;
                }

                CreateTestResult result = courseService.createTest(teacherId, courseId, title, questions, qCount);

                switch (result.code) {
                    case CREATE_TEST_OK: {
                        crow::json::wvalue resp;
                        resp["success"] = true;
                        resp["message"] = "Тест створено";
                        resp["test"]    = testToJson(result.test);
                        return crow::response(201, resp);
                    }
                    case CREATE_TEST_INVALID_TEACHER:
                        return testErrorResponse(403, "Викладача не знайдено або немає прав на цей курс");
                    case CREATE_TEST_INVALID_COURSE:
                        return testErrorResponse(404, "Курс не знайдено");
                    case CREATE_TEST_INVALID_TITLE:
                        return testErrorResponse(400, "Назва тесту некоректна");
                    case CREATE_TEST_INVALID_QUESTIONS:
                        return testErrorResponse(400, "Запитання некоректні");
                    case CREATE_TEST_LIMIT_REACHED:
                        return testErrorResponse(409, "Досягнуто максимальну кількість тестів на курсі");
                    default:
                        return testErrorResponse(500, "Помилка сервера");
                } });

        CROW_ROUTE(app, "/api/tests/<int>")
            .methods("GET"_method)([&courseService](int testId)
                                   {

                auto idCheck = Validator::validatePositiveId(testId);
                if (!idCheck.isValid) return testErrorResponse(400, idCheck.errorMessage);

                Test test{};
                if (!courseService.getTestById(testId, test))
                    return testErrorResponse(404, "Тест не знайдено");

                std::vector<crow::json::wvalue> qList;
                for (int i = 0; i < test.questionCount; i++) {
                    Question q{};
                    if (courseService.getQuestionById(test.questionIds[i], q))
                        qList.push_back(questionToJson(q));
                }

                crow::json::wvalue tj = testToJson(test);
                tj["questions"] = std::move(qList);

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["test"]    = std::move(tj);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/teachers/<int>/tests/<int>")
            .methods("PUT"_method, "DELETE"_method)([&courseService](const crow::request &req, int teacherId, int testId)
                                                    {

                auto tc = Validator::validatePositiveId(teacherId);
                if (!tc.isValid) return testErrorResponse(400, tc.errorMessage);

                auto ic = Validator::validatePositiveId(testId);
                if (!ic.isValid) return testErrorResponse(400, ic.errorMessage);

                
                if (req.method == crow::HTTPMethod::Delete) {
                    bool deleted = courseService.deleteTest(teacherId, testId);
                    if (!deleted)
                        return testErrorResponse(404, "Тест не знайдено або немає прав");
                    crow::json::wvalue resp;
                    resp["success"] = true;
                    resp["message"] = "Тест видалено";
                    return crow::response(200, resp);
                }

                
                auto body = crow::json::load(req.body);
                if (!body) return testErrorResponse(400, "Некоректний JSON");

                if (!body.has("title") || !body.has("questions"))
                    return testErrorResponse(400, "Поля title та questions є обов'язковими");

                char title[MAX_TEST_TITLE_LENGTH];
                fromJson(title, sizeof(title), body["title"]);

                auto& questionsJson = body["questions"];
                int qCount = static_cast<int>(questionsJson.size());
                if (qCount == 0)
                    return testErrorResponse(400, "Тест повинен мати хоча б одне запитання");
                if (qCount > MAX_TEST_QUESTIONS)
                    return testErrorResponse(400, "Перевищено максимальну кількість запитань");

                Question questions[MAX_TEST_QUESTIONS];
                for (int i = 0; i < qCount; i++) {
                    questions[i] = Question{};
                    auto& qj = questionsJson[i];
                    if (!qj.has("text") || !qj.has("options") || !qj.has("correctFlags"))
                        return testErrorResponse(400, "Кожне запитання повинно мати text, options та correctFlags");

                    fromJson(questions[i].text, MAX_QUESTION_TEXT_LENGTH, qj["text"]);
                    questions[i].text[MAX_QUESTION_TEXT_LENGTH - 1] = '\0';

                    auto& opts  = qj["options"];
                    auto& flags = qj["correctFlags"];
                    questions[i].optionCount = static_cast<int>(opts.size());
                    if (questions[i].optionCount > MAX_QUESTION_OPTIONS)
                        questions[i].optionCount = MAX_QUESTION_OPTIONS;

                    for (int k = 0; k < questions[i].optionCount; k++) {
                        fromJson(questions[i].options[k], MAX_OPTION_TEXT_LENGTH, opts[k]);
                        questions[i].options[k][MAX_OPTION_TEXT_LENGTH - 1] = '\0';
                        questions[i].correctOptionFlags[k] = static_cast<int>(flags[k].i());
                    }
                    questions[i].score = qj.has("score") ? qj["score"].d() : 1.0;
                }

                bool ok = courseService.updateTest(teacherId, testId, title, questions, qCount);
                if (!ok)
                    return testErrorResponse(403, "Тест не знайдено або немає прав на редагування");

                Test updated{};
                courseService.getTestById(testId, updated);
                crow::json::wvalue resp;
                resp["success"] = true;
                resp["message"] = "Тест оновлено";
                resp["test"]    = testToJson(updated);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/students/<int>/tests/<int>/result")
            .methods("GET"_method)([&courseService](int studentId, int testId)
                                   {

                auto sc = Validator::validatePositiveId(studentId);
                if (!sc.isValid) return testErrorResponse(400, sc.errorMessage);
                auto tc = Validator::validatePositiveId(testId);
                if (!tc.isValid) return testErrorResponse(400, tc.errorMessage);

                TestResult tr{};
                if (!courseService.getTestResult(studentId, testId, tr))
                    return testErrorResponse(404, "Результат не знайдено — студент ще не проходив цей тест");

                Test test{};
                if (!courseService.getTestById(testId, test))
                    return testErrorResponse(404, "Тест не знайдено");

                std::vector<crow::json::wvalue> qList;
                for (int i = 0; i < test.questionCount; i++) {
                    Question q{};
                    if (courseService.getQuestionById(test.questionIds[i], q))
                        qList.push_back(questionToJson(q));
                }

                crow::json::wvalue tj = testToJson(test);
                tj["questions"] = std::move(qList);

                std::vector<crow::json::wvalue> masks;
                for (int i = 0; i < tr.answerCount; i++)
                    masks.push_back(crow::json::wvalue(tr.answerMasks[i]));

                crow::json::wvalue res;
                res["success"]     = true;
                res["studentId"]   = tr.studentId;
                res["testId"]      = tr.testId;
                res["score"]       = tr.score;
                res["maxScore"]    = tr.maxScore;
                res["passed"]      = tr.passed;
                res["answerMasks"] = std::move(masks);
                res["test"]        = std::move(tj);
                return crow::response(200, res); });

        CROW_ROUTE(app, "/api/students/<int>/test-results")
            .methods("GET"_method)([&courseService](int studentId)
                                   {

                auto sc = Validator::validatePositiveId(studentId);
                if (!sc.isValid) return testErrorResponse(400, sc.errorMessage);

                TestResult buf[MAX_STORAGE_RECORDS];
                int count = courseService.getTestResultsForStudent(studentId, buf, MAX_STORAGE_RECORDS);

                std::vector<crow::json::wvalue> list;
                for (int i = 0; i < count; i++) {
                    Test test{};
                    crow::json::wvalue item;
                    item["testId"]   = buf[i].testId;
                    item["score"]    = buf[i].score;
                    item["maxScore"] = buf[i].maxScore;
                    item["passed"]   = buf[i].passed;
                    if (courseService.getTestById(buf[i].testId, test))
                        item["testTitle"] = test.title;
                    else
                        item["testTitle"] = "—";
                    list.push_back(std::move(item));
                }

                crow::json::wvalue resp;
                resp["success"] = true;
                resp["count"]   = count;
                resp["results"] = std::move(list);
                return crow::response(200, resp); });

        CROW_ROUTE(app, "/api/students/<int>/tests/<int>/submit")
            .methods("POST"_method)([&courseService](const crow::request &req, int studentId, int testId)
                                    {

                auto sc = Validator::validatePositiveId(studentId);
                if (!sc.isValid) return testErrorResponse(400, sc.errorMessage);

                auto tc = Validator::validatePositiveId(testId);
                if (!tc.isValid) return testErrorResponse(400, tc.errorMessage);

                auto body = crow::json::load(req.body);
                if (!body) return testErrorResponse(400, "Некоректний JSON");

                if (!body.has("answers"))
                    return testErrorResponse(400, "Поле answers є обов'язковим");

                auto& answersJson = body["answers"];
                int ansCount = static_cast<int>(answersJson.size());

                if (ansCount == 0)
                    return testErrorResponse(400, "Масив відповідей не може бути порожнім");
                if (ansCount > MAX_TEST_QUESTIONS)
                    return testErrorResponse(400, "Забагато відповідей");

                int answerMasks[MAX_TEST_QUESTIONS];
                for (int i = 0; i < ansCount; i++)
                    answerMasks[i] = static_cast<int>(answersJson[i].i());

                TakeTestResult result = courseService.takeTest(studentId, testId, answerMasks, ansCount);

                switch (result.code) {
                    case TAKE_TEST_OK: {
                        crow::json::wvalue resp;
                        resp["success"]  = true;
                        resp["message"]  = result.passed ? "Тест пройдено успішно!" : "Тест здано";
                        resp["score"]    = result.score;
                        resp["maxScore"] = result.maxScore;
                        resp["passed"]   = result.passed;
                        return crow::response(200, resp);
                    }
                    case TAKE_TEST_INVALID_STUDENT:
                        return testErrorResponse(404, "Студента не знайдено");
                    case TAKE_TEST_INVALID_TEST:
                        return testErrorResponse(404, "Тест не знайдено");
                    case TAKE_TEST_NOT_ENROLLED:
                        return testErrorResponse(403, "Студент не записаний на цей курс");
                    case TAKE_TEST_ALREADY_PASSED:
                        return testErrorResponse(409, "Студент вже проходив цей тест");
                    case TAKE_TEST_INVALID_ANSWERS:
                        return testErrorResponse(400, "Відповіді некоректні");
                    case TAKE_TEST_LIMIT_REACHED:
                        return testErrorResponse(409, "Досягнуто ліміт спроб");
                    default:
                        return testErrorResponse(500, "Помилка сервера");
                } });
    }
}
