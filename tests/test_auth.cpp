#include "doctest/doctest.h"
#include "../services/AuthService.h"
#include "../storage/StudentStorage.h"
#include "../storage/TeacherStorage.h"
#include "../common/Validator.h"

#include <cstdio>

static void cleanFile(const char *path) { std::remove(path); }

TEST_CASE("Validator::validateEmail")
{
    using namespace EduManage;
    CHECK(Validator::validateEmail("user@example.com").isValid == true);
    CHECK(Validator::validateEmail("a@b.c").isValid == true);
    CHECK(Validator::validateEmail("").isValid == false);
    CHECK(Validator::validateEmail("nodot@com").isValid == false);
    CHECK(Validator::validateEmail("noatsign.com").isValid == false);

    auto result = Validator::validateEmail("bademail");
    CHECK(result.isValid == false);
    CHECK(result.errorMessage[0] != '\0');
}

TEST_CASE("Validator::validatePassword")
{
    using namespace EduManage;
    CHECK(Validator::validatePassword("1234").isValid == true);
    CHECK(Validator::validatePassword("securePass!").isValid == true);
    CHECK(Validator::validatePassword("abc").isValid == false);
    CHECK(Validator::validatePassword("").isValid == false);
}

TEST_CASE("Validator::validateFullName")
{
    using namespace EduManage;
    CHECK(Validator::validateFullName("Іван Петров").isValid == true);
    CHECK(Validator::validateFullName("A").isValid == true);
    CHECK(Validator::validateFullName("").isValid == false);
}

TEST_CASE("Validator::validateCourseName")
{
    using namespace EduManage;
    CHECK(Validator::validateCourseName("Математика").isValid == true);
    CHECK(Validator::validateCourseName("").isValid == false);
}

TEST_CASE("AuthService::registerStudent - успішна реєстрація")
{
    const char *sf = "t_auth1_s.dat";
    const char *tf = "t_auth1_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auto result = auth.registerStudent("Марія Коваль", "maria@test.com", "pass1234");

    CHECK(result.code == EduManage::REGISTER_OK);
    CHECK(result.user.id > 0);
    CHECK(result.role == EduManage::ACCOUNT_ROLE_STUDENT);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::registerTeacher - успішна реєстрація")
{
    const char *sf = "t_auth2_s.dat";
    const char *tf = "t_auth2_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auto result = auth.registerTeacher("Олег Сидоренко", "oleg@test.com", "mypassword");

    CHECK(result.code == EduManage::REGISTER_OK);
    CHECK(result.role == EduManage::ACCOUNT_ROLE_TEACHER);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::registerStudent - дублікат email")
{
    const char *sf = "t_auth3_s.dat";
    const char *tf = "t_auth3_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auth.registerStudent("Перший", "same@test.com", "pass1234");
    auto result = auth.registerStudent("Другий", "same@test.com", "pass5678");

    CHECK(result.code == EduManage::REGISTER_DUPLICATE_EMAIL);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::registerStudent - порожнє ім'я")
{
    const char *sf = "t_auth4_s.dat";
    const char *tf = "t_auth4_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auto result = auth.registerStudent("", "valid@test.com", "pass1234");
    CHECK(result.code == EduManage::REGISTER_INVALID_NAME);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::registerStudent - некоректний email")
{
    const char *sf = "t_auth5_s.dat";
    const char *tf = "t_auth5_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auto result = auth.registerStudent("Тест", "invalidemail", "pass1234");
    CHECK(result.code == EduManage::REGISTER_INVALID_EMAIL);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::registerStudent - слабкий пароль")
{
    const char *sf = "t_auth6_s.dat";
    const char *tf = "t_auth6_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auto result = auth.registerStudent("Тест", "user@test.com", "abc");
    CHECK(result.code == EduManage::REGISTER_INVALID_PASSWORD);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::login - успішний вхід")
{
    const char *sf = "t_auth7_s.dat";
    const char *tf = "t_auth7_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auth.registerStudent("Логін Тест", "login@test.com", "mypass");
    auto result = auth.login("login@test.com", "mypass");

    CHECK(result.code == EduManage::LOGIN_OK);
    CHECK(result.user.id > 0);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::login - невірний пароль")
{
    const char *sf = "t_auth8_s.dat";
    const char *tf = "t_auth8_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auth.registerStudent("Логін Тест", "login2@test.com", "correctpass");
    auto result = auth.login("login2@test.com", "wrongpass");
    CHECK(result.code == EduManage::LOGIN_WRONG_PASSWORD);

    cleanFile(sf);
    cleanFile(tf);
}

TEST_CASE("AuthService::login - користувача не існує")
{
    const char *sf = "t_auth9_s.dat";
    const char *tf = "t_auth9_t.dat";
    cleanFile(sf);
    cleanFile(tf);

    EduManage::StudentStorage sStor(sf);
    EduManage::TeacherStorage tStor(tf);
    EduManage::AuthService auth(sStor, tStor);

    auto result = auth.login("nobody@test.com", "somepass");
    CHECK(result.code == EduManage::LOGIN_USER_NOT_FOUND);

    cleanFile(sf);
    cleanFile(tf);
}
