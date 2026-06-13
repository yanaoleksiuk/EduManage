#include "ApiServer.h"

#include <iostream>
#include <fstream>
#include <string>

static std::string staticReadFile(const std::string &path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open())
        return {};
    return std::string(std::istreambuf_iterator<char>(f),
                       std::istreambuf_iterator<char>());
}

static std::string staticMime(const std::string &path)
{
    auto ends = [&](const char *ext)
    {
        size_t el = std::strlen(ext);
        return path.size() >= el &&
               path.compare(path.size() - el, el, ext) == 0;
    };
    if (ends(".html"))
        return "text/html; charset=utf-8";
    if (ends(".css"))
        return "text/css; charset=utf-8";
    if (ends(".js"))
        return "application/javascript; charset=utf-8";
    if (ends(".svg"))
        return "image/svg+xml";
    if (ends(".ico"))
        return "image/x-icon";
    if (ends(".png"))
        return "image/png";
    return "application/octet-stream";
}

static crow::response serveStatic(const std::string &rel)
{

    if (rel.find("..") != std::string::npos)
        return crow::response(403);

    for (const char *base : {"frontend", "../frontend", "../../frontend"})
    {
        std::string full = std::string(base) + "/" + rel;
        std::string body = staticReadFile(full);
        if (!body.empty())
        {
            crow::response res(200, body);
            res.set_header("Content-Type", staticMime(full));
            return res;
        }
    }
    return crow::response(404);
}

namespace EduManage
{

    ApiServer::ApiServer()
        : studentStorage{}, teacherStorage{}, courseStorage{}, groupStorage{}, themeStorage{}, lessonStorage{}, testStorage{}, questionStorage{}, homeworkStorage{}, authService(studentStorage, teacherStorage), courseService(courseStorage, studentStorage, teacherStorage,
                                                                                                                                                                                                                                groupStorage, themeStorage, lessonStorage,
                                                                                                                                                                                                                                testStorage, questionStorage, homeworkStorage)
    {
    }

    void ApiServer::setupRoutes()
    {
        setupAuthRoutes(app, authService);
        setupUserRoutes(app, studentStorage, teacherStorage, authService);
        setupCourseRoutes(app, courseStorage, courseService, studentStorage);
        setupTeacherRoutes(app, courseService);
        setupThemeRoutes(app, courseService);
        setupTestRoutes(app, courseService);
        setupGroupRoutes(app, courseService);
        setupProgressRoutes(app, courseService);

        CROW_ROUTE(app, "/")([]()
                             { return serveStatic("index.html"); });
        CROW_ROUTE(app, "/index.html")([]()
                                       { return serveStatic("index.html"); });
        CROW_ROUTE(app, "/dashboard.html")([]()
                                           { return serveStatic("dashboard.html"); });
        CROW_ROUTE(app, "/css/<string>")([](const std::string &f)
                                         { return serveStatic("css/" + f); });
        CROW_ROUTE(app, "/js/<string>")([](const std::string &f)
                                        { return serveStatic("js/" + f); });
    }

    void ApiServer::run(int port)
    {
        setupRoutes();

        std::cout << "=== EduManage REST API ===\n";
        std::cout << "Server started on http://localhost:" << port << "\n";
        std::cout << "\n--- Authorization ---\n";
        std::cout << "  POST   /api/auth/register  { role: student|teacher }\n";
        std::cout << "  POST   /api/auth/login\n";
        std::cout << "\n--- Users ---\n";
        std::cout << "  GET    /api/users\n";
        std::cout << "  GET    /api/users/:id\n";
        std::cout << "  PUT    /api/users/:id\n";
        std::cout << "  DELETE /api/users/:id\n";
        std::cout << "\n--- Courses ---\n";
        std::cout << "  GET    /api/courses\n";
        std::cout << "  POST   /api/teachers/:tid/courses                    { name }\n";
        std::cout << "  GET    /api/courses/:id\n";
        std::cout << "  PUT    /api/courses/:id                              { name }\n";
        std::cout << "  DELETE /api/courses/:id\n";
        std::cout << "  GET    /api/courses/search?q=...&teacherId=...&studentId=...\n";
        std::cout << "  GET    /api/courses/:id/progress?studentId=X\n";
        std::cout << "\n--- Enrollment ---\n";
        std::cout << "  GET    /api/teachers/:tid/courses/:cid/enroll-options\n";
        std::cout << "  POST   /api/teachers/:tid/courses/:cid/enroll        { studentId } or { studentEmail }\n";
        std::cout << "  POST   /api/teachers/:tid/courses/:cid/enroll-group  { groupId }\n";
        std::cout << "\n--- Themes and Lessons ---\n";
        std::cout << "  GET    /api/courses/:id/themes\n";
        std::cout << "  POST   /api/teachers/:tid/courses/:cid/themes        { title }\n";
        std::cout << "  GET    /api/themes/:id/lessons\n";
        std::cout << "  POST   /api/teachers/:tid/themes/:thid/lessons       { title, material, date?, time?, homeworkLink? }\n";
        std::cout << "  GET    /api/lessons/:id\n";
        std::cout << "  PUT    /api/teachers/:tid/lessons/:lid               { title?, material?, date?, time?, homeworkLink? }\n";
        std::cout << "  POST   /api/students/:sid/lessons/:lid/complete\n";
        std::cout << "\n--- Tests ---\n";
        std::cout << "  GET    /api/courses/:id/tests\n";
        std::cout << "  POST   /api/teachers/:tid/courses/:cid/tests         { title, questions[] }\n";
        std::cout << "  GET    /api/tests/:id\n";
        std::cout << "  POST   /api/students/:sid/tests/:tid/submit          { answers[] }\n";
        std::cout << "\n--- Students and Groups ---\n";
        std::cout << "  GET    /api/teachers/:id/students\n";
        std::cout << "  POST   /api/teachers/:id/students                    { studentEmail }\n";
        std::cout << "  GET    /api/teachers/:id/groups\n";
        std::cout << "  POST   /api/teachers/:id/groups                      { name, studentIds[] }\n";
        std::cout << "\n--- Progress and Schedule ---\n";
        std::cout << "  GET    /api/students/:id/progress\n";
        std::cout << "  GET    /api/students/:id/schedule\n";
        std::cout << "  GET    /api/teachers/:id/schedule\n";
        std::cout << "==========================\n";

        app.multithreaded().port(port).run();
    }

}
