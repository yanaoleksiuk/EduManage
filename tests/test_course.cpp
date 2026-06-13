#include "doctest/doctest.h"
#include "../services/AuthService.h"
#include "../services/CourseService.h"
#include "../storage/StudentStorage.h"
#include "../storage/TeacherStorage.h"
#include "../storage/CourseStorage.h"
#include "../storage/GroupStorage.h"
#include "../storage/ThemeStorage.h"
#include "../storage/LessonStorage.h"
#include "../storage/TestStorage.h"
#include "../storage/QuestionStorage.h"
#include "../storage/HomeworkStorage.h"

#include <cstdio>
#include <cstring>

static void cleanFile(const char *path) { std::remove(path); }

struct TestEnv
{

    char sf[64], tf[64], cf[64], gf[64], thf[64], lf[64], tsf[64], qf[64];

    EduManage::StudentStorage sStor;
    EduManage::TeacherStorage tStor;
    EduManage::CourseStorage cStor;
    EduManage::GroupStorage gStor;
    EduManage::ThemeStorage thStor;
    EduManage::LessonStorage lStor;
    EduManage::TestStorage tsStor;
    EduManage::QuestionStorage qStor;
    EduManage::HomeworkStorage hwStor;

    EduManage::AuthService auth;
    EduManage::CourseService courseService;

    explicit TestEnv(const char *prefix)
        : sStor((std::snprintf(sf, 64, "%s_s.dat", prefix), sf)), tStor((std::snprintf(tf, 64, "%s_t.dat", prefix), tf)), cStor((std::snprintf(cf, 64, "%s_c.dat", prefix), cf)), gStor((std::snprintf(gf, 64, "%s_g.dat", prefix), gf)), thStor((std::snprintf(thf, 64, "%s_th.dat", prefix), thf)), lStor((std::snprintf(lf, 64, "%s_l.dat", prefix), lf)), tsStor((std::snprintf(tsf, 64, "%s_ts.dat", prefix), tsf)), qStor((std::snprintf(qf, 64, "%s_q.dat", prefix), qf)), auth(sStor, tStor), courseService(cStor, sStor, tStor, gStor, thStor, lStor, tsStor, qStor, hwStor)
    {
    }

    ~TestEnv()
    {
        cleanFile(sf);
        cleanFile(tf);
        cleanFile(cf);
        cleanFile(gf);
        cleanFile(thf);
        cleanFile(lf);
        cleanFile(tsf);
        cleanFile(qf);
    }
};

TEST_CASE("CourseService::createCourse - успішне створення")
{
    TestEnv env("tc_create1");

    auto regResult = env.auth.registerTeacher("Проф. Іванов", "prof1@test.com", "pass1234");
    REQUIRE(regResult.code == EduManage::REGISTER_OK);

    auto result = env.courseService.createCourse(regResult.user.id, "Алгоритми та структури даних");

    CHECK(result.code == EduManage::CREATE_COURSE_OK);
    CHECK(result.course.id > 0);
    CHECK(result.course.creatorTeacherId == regResult.user.id);
}

TEST_CASE("CourseService::createCourse - порожня назва")
{
    TestEnv env("tc_create2");

    auto regResult = env.auth.registerTeacher("Проф. Коваль", "koval2@test.com", "pass1234");
    REQUIRE(regResult.code == EduManage::REGISTER_OK);

    auto result = env.courseService.createCourse(regResult.user.id, "");

    CHECK(result.code == EduManage::CREATE_COURSE_INVALID_NAME);
}

TEST_CASE("CourseService::createCourse - студент намагається створити курс")
{
    TestEnv env("tc_create3");

    auto regResult = env.auth.registerStudent("Студент Тест", "stud3@test.com", "pass1234");
    REQUIRE(regResult.code == EduManage::REGISTER_OK);

    auto result = env.courseService.createCourse(regResult.user.id, "Мій курс");

    CHECK(result.code == EduManage::CREATE_COURSE_INVALID_TEACHER);
}

TEST_CASE("CourseService::grantCourseAccessToStudent - успішний запис")
{
    TestEnv env("tc_enroll1");

    auto teacherReg = env.auth.registerTeacher("Проф. Семенко", "semen4@test.com", "pass1234");
    auto studentReg = env.auth.registerStudent("Студент Олена", "olena4@test.com", "pass5678");
    REQUIRE(teacherReg.code == EduManage::REGISTER_OK);
    REQUIRE(studentReg.code == EduManage::REGISTER_OK);

    auto courseResult = env.courseService.createCourse(teacherReg.user.id, "Програмування на C++");
    REQUIRE(courseResult.code == EduManage::CREATE_COURSE_OK);

    env.courseService.addTeacherStudentByEmail(teacherReg.user.id, "olena4@test.com");

    auto enrollResult = env.courseService.grantCourseAccessToStudent(
        teacherReg.user.id, courseResult.course.id, studentReg.user.id);

    CHECK(enrollResult.code == EduManage::GRANT_COURSE_ACCESS_OK);
}

TEST_CASE("CourseService::grantCourseAccessToStudent - повторний доступ")
{
    TestEnv env("tc_enroll2");

    auto teacherReg = env.auth.registerTeacher("Проф. Лисенко", "lysen5@test.com", "pass1234");
    auto studentReg = env.auth.registerStudent("Студент Андрій", "andriy5@test.com", "pass5678");

    auto courseResult = env.courseService.createCourse(teacherReg.user.id, "Бази даних");

    env.courseService.addTeacherStudentByEmail(teacherReg.user.id, "andriy5@test.com");
    env.courseService.grantCourseAccessToStudent(teacherReg.user.id, courseResult.course.id, studentReg.user.id);
    auto enrollResult = env.courseService.grantCourseAccessToStudent(
        teacherReg.user.id, courseResult.course.id, studentReg.user.id);

    CHECK(enrollResult.code == EduManage::GRANT_COURSE_ACCESS_ALREADY_GRANTED);
}

TEST_CASE("CourseService::grantCourseAccessToStudent - неіснуючий курс")
{
    TestEnv env("tc_enroll3");

    auto teacherReg = env.auth.registerTeacher("Проф. Тест", "tprof6@test.com", "pass1234");
    auto studentReg = env.auth.registerStudent("Студент Тест", "tstud6@test.com", "pass1234");

    auto enrollResult = env.courseService.grantCourseAccessToStudent(
        teacherReg.user.id, 9999, studentReg.user.id);

    CHECK(enrollResult.code == EduManage::GRANT_COURSE_ACCESS_INVALID_COURSE);
}
