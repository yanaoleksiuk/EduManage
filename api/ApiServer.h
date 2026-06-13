#pragma once
#include "crow.h"
#include "CorsMiddleware.h"
#include "../storage/StudentStorage.h"
#include "../storage/TeacherStorage.h"
#include "../storage/CourseStorage.h"
#include "../storage/GroupStorage.h"
#include "../storage/ThemeStorage.h"
#include "../storage/LessonStorage.h"
#include "../storage/TestStorage.h"
#include "../storage/QuestionStorage.h"
#include "../storage/HomeworkStorage.h"
#include "../services/AuthService.h"
#include "../services/CourseService.h"
#include "AuthRoutes.h"
#include "UserRoutes.h"
#include "CourseRoutes.h"
#include "TeacherRoutes.h"
#include "ThemeRoutes.h"
#include "TestRoutes.h"
#include "GroupRoutes.h"
#include "ProgressRoutes.h"

namespace EduManage
{

    class ApiServer
    {
    public:
        ApiServer();
        void run(int port = 8080);

    private:
        StudentStorage studentStorage;
        TeacherStorage teacherStorage;
        CourseStorage courseStorage;
        GroupStorage groupStorage;
        ThemeStorage themeStorage;
        LessonStorage lessonStorage;
        TestStorage testStorage;
        QuestionStorage questionStorage;
        HomeworkStorage homeworkStorage;

        AuthService authService;
        CourseService courseService;

        crow::App<CorsMiddleware> app;

        void setupRoutes();
    };

}
