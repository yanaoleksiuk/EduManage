#pragma once

#include "../models/Student.h"
#include "../models/Teacher.h"
#include "../models/User.h"
#include "../storage/StudentStorage.h"
#include "../storage/TeacherStorage.h"

namespace EduManage
{
    enum AccountRole
    {
        ACCOUNT_ROLE_NONE,
        ACCOUNT_ROLE_STUDENT,
        ACCOUNT_ROLE_TEACHER
    };

    enum RegisterResultCode
    {
        REGISTER_OK,
        REGISTER_INVALID_NAME,
        REGISTER_INVALID_EMAIL,
        REGISTER_INVALID_PASSWORD,
        REGISTER_DUPLICATE_EMAIL
    };

    struct RegisterResult
    {
        RegisterResultCode code;
        User user;
        AccountRole role;
    };

    enum LoginResultCode
    {
        LOGIN_OK,
        LOGIN_USER_NOT_FOUND,
        LOGIN_WRONG_PASSWORD
    };

    struct LoginResult
    {
        LoginResultCode code;
        User user;
        AccountRole role;
    };

    enum UpdateAccountResultCode
    {
        UPDATE_ACCOUNT_OK,
        UPDATE_ACCOUNT_INVALID_NAME,
        UPDATE_ACCOUNT_INVALID_EMAIL,
        UPDATE_ACCOUNT_INVALID_PASSWORD,
        UPDATE_ACCOUNT_DUPLICATE_EMAIL,
        UPDATE_ACCOUNT_NOT_FOUND
    };

    struct UpdateAccountResult
    {
        UpdateAccountResultCode code;
        User user;
        AccountRole role;
    };

    class AuthService
    {
    public:
        AuthService(StudentStorage &studentStorage, TeacherStorage &teacherStorage);

        RegisterResult registerStudent(const char *fullName, const char *email, const char *password);
        RegisterResult registerTeacher(const char *fullName, const char *email, const char *password);
        LoginResult login(const char *email, const char *password) const;
        UpdateAccountResult updateStudentProfile(int studentId, const char *fullName, const char *email, const char *password);
        UpdateAccountResult updateTeacherProfile(int teacherId, const char *fullName, const char *email, const char *password);

    private:
        int getNextUserId() const;
        void fillBaseUser(User &user, int id, const char *fullName, const char *email, const char *password) const;
        bool isValidName(const char *fullName) const;
        bool isValidEmail(const char *email) const;
        bool isValidPassword(const char *password) const;
        bool emailExists(const char *email) const;
        bool emailExistsForAnotherUser(const char *email, int currentUserId) const;

        StudentStorage &studentStorage;
        TeacherStorage &teacherStorage;
    };
}
