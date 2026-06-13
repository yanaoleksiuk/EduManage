#include "services/AuthService.h"
#include "services/CourseService.h"
#include "storage/CourseStorage.h"
#include "storage/GroupStorage.h"
#include "storage/HomeworkStorage.h"
#include "storage/LessonStorage.h"
#include "storage/QuestionStorage.h"
#include "storage/StudentStorage.h"
#include "storage/TeacherStorage.h"
#include "storage/TestStorage.h"
#include "storage/ThemeStorage.h"
#include "ui/ConsoleApp.h"

int main()
{
    EduManage::StudentStorage studentStorage;
    EduManage::TeacherStorage teacherStorage;
    EduManage::CourseStorage courseStorage;
    EduManage::GroupStorage groupStorage;
    EduManage::ThemeStorage themeStorage;
    EduManage::LessonStorage lessonStorage;
    EduManage::TestStorage testStorage;
    EduManage::QuestionStorage questionStorage;

    EduManage::HomeworkStorage homeworkStorage;

    EduManage::AuthService authService(studentStorage, teacherStorage);
    EduManage::CourseService courseService(courseStorage, studentStorage, teacherStorage, groupStorage, themeStorage, lessonStorage, testStorage, questionStorage, homeworkStorage);

    EduManage::ConsoleApp app(authService, courseService);
    app.run();

    return 0;
}
