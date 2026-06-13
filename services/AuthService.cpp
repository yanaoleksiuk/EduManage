#include "AuthService.h"

#include "../common/TextUtils.h"

namespace EduManage
{
    AuthService::AuthService(StudentStorage &studentStorage, TeacherStorage &teacherStorage)
        : studentStorage(studentStorage), teacherStorage(teacherStorage) {}

    RegisterResult AuthService::registerStudent(const char *fullName, const char *email, const char *password)
    {
        RegisterResult result = {};
        result.code = REGISTER_INVALID_NAME;
        result.role = ACCOUNT_ROLE_STUDENT;

        if (!isValidName(fullName))
        {
            return result;
        }

        if (!isValidEmail(email))
        {
            result.code = REGISTER_INVALID_EMAIL;
            return result;
        }

        if (!isValidPassword(password))
        {
            result.code = REGISTER_INVALID_PASSWORD;
            return result;
        }

        if (emailExists(email))
        {
            result.code = REGISTER_DUPLICATE_EMAIL;
            return result;
        }

        Student student = {};
        fillBaseUser(student, getNextUserId(), fullName, email, password);
        student.totalProgress = 0.0;

        if (!studentStorage.add(student))
        {
            result.code = REGISTER_DUPLICATE_EMAIL;
            return result;
        }

        result.code = REGISTER_OK;
        result.user = static_cast<User &>(student);
        return result;
    }

    RegisterResult AuthService::registerTeacher(const char *fullName, const char *email, const char *password)
    {
        RegisterResult result = {};
        result.code = REGISTER_INVALID_NAME;
        result.role = ACCOUNT_ROLE_TEACHER;

        if (!isValidName(fullName))
        {
            return result;
        }

        if (!isValidEmail(email))
        {
            result.code = REGISTER_INVALID_EMAIL;
            return result;
        }

        if (!isValidPassword(password))
        {
            result.code = REGISTER_INVALID_PASSWORD;
            return result;
        }

        if (emailExists(email))
        {
            result.code = REGISTER_DUPLICATE_EMAIL;
            return result;
        }

        Teacher teacher = {};
        fillBaseUser(teacher, getNextUserId(), fullName, email, password);

        if (!teacherStorage.add(teacher))
        {
            result.code = REGISTER_DUPLICATE_EMAIL;
            return result;
        }

        result.code = REGISTER_OK;
        result.user = static_cast<User &>(teacher);
        return result;
    }

    LoginResult AuthService::login(const char *email, const char *password) const
    {
        LoginResult result = {};
        result.code = LOGIN_USER_NOT_FOUND;
        result.role = ACCOUNT_ROLE_NONE;

        Teacher teacher = {};
        if (teacherStorage.getByEmail(email, teacher))
        {
            if (!textsEqual(teacher.password, password))
            {
                result.code = LOGIN_WRONG_PASSWORD;
                return result;
            }

            result.code = LOGIN_OK;
            result.role = ACCOUNT_ROLE_TEACHER;
            result.user = static_cast<User &>(teacher);
            return result;
        }

        Student student = {};
        if (studentStorage.getByEmail(email, student))
        {
            if (!textsEqual(student.password, password))
            {
                result.code = LOGIN_WRONG_PASSWORD;
                return result;
            }

            result.code = LOGIN_OK;
            result.role = ACCOUNT_ROLE_STUDENT;
            result.user = static_cast<User &>(student);
            return result;
        }

        return result;
    }

    UpdateAccountResult AuthService::updateStudentProfile(int studentId,
                                                          const char *fullName,
                                                          const char *email,
                                                          const char *password)
    {
        UpdateAccountResult result = {};
        result.code = UPDATE_ACCOUNT_NOT_FOUND;
        result.role = ACCOUNT_ROLE_STUDENT;

        Student student = {};
        if (!studentStorage.getById(studentId, student))
        {
            return result;
        }

        if (!isValidName(fullName))
        {
            result.code = UPDATE_ACCOUNT_INVALID_NAME;
            return result;
        }

        if (!isValidEmail(email))
        {
            result.code = UPDATE_ACCOUNT_INVALID_EMAIL;
            return result;
        }

        if (!isValidPassword(password))
        {
            result.code = UPDATE_ACCOUNT_INVALID_PASSWORD;
            return result;
        }

        if (emailExistsForAnotherUser(email, studentId))
        {
            result.code = UPDATE_ACCOUNT_DUPLICATE_EMAIL;
            return result;
        }

        fillBaseUser(student, studentId, fullName, email, password);
        if (!studentStorage.update(student))
        {
            return result;
        }

        result.code = UPDATE_ACCOUNT_OK;
        result.user = static_cast<User &>(student);
        return result;
    }

    UpdateAccountResult AuthService::updateTeacherProfile(int teacherId,
                                                          const char *fullName,
                                                          const char *email,
                                                          const char *password)
    {
        UpdateAccountResult result = {};
        result.code = UPDATE_ACCOUNT_NOT_FOUND;
        result.role = ACCOUNT_ROLE_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        if (!isValidName(fullName))
        {
            result.code = UPDATE_ACCOUNT_INVALID_NAME;
            return result;
        }

        if (!isValidEmail(email))
        {
            result.code = UPDATE_ACCOUNT_INVALID_EMAIL;
            return result;
        }

        if (!isValidPassword(password))
        {
            result.code = UPDATE_ACCOUNT_INVALID_PASSWORD;
            return result;
        }

        if (emailExistsForAnotherUser(email, teacherId))
        {
            result.code = UPDATE_ACCOUNT_DUPLICATE_EMAIL;
            return result;
        }

        fillBaseUser(teacher, teacherId, fullName, email, password);
        if (!teacherStorage.update(teacher))
        {
            return result;
        }

        result.code = UPDATE_ACCOUNT_OK;
        result.user = static_cast<User &>(teacher);
        return result;
    }

    int AuthService::getNextUserId() const
    {
        const int nextStudentId = studentStorage.getNextId();
        const int nextTeacherId = teacherStorage.getNextId();
        return nextStudentId > nextTeacherId ? nextStudentId : nextTeacherId;
    }

    void AuthService::fillBaseUser(User &user, int id, const char *fullName, const char *email, const char *password) const
    {
        user.id = id;
        copyText(user.fullName, sizeof(user.fullName), fullName);
        copyText(user.email, sizeof(user.email), email);
        copyText(user.password, sizeof(user.password), password);
    }

    bool AuthService::isValidName(const char *fullName) const
    {
        return !isEmptyText(fullName);
    }

    bool AuthService::isValidEmail(const char *email) const
    {
        if (isEmptyText(email))
        {
            return false;
        }

        bool hasAt = false;
        bool hasDot = false;
        for (int i = 0; email[i] != '\0'; ++i)
        {
            if (email[i] == '@')
            {
                hasAt = true;
            }

            if (email[i] == '.')
            {
                hasDot = true;
            }
        }

        return hasAt && hasDot;
    }

    bool AuthService::isValidPassword(const char *password) const
    {
        if (isEmptyText(password))
        {
            return false;
        }

        int length = 0;
        while (password[length] != '\0')
        {
            ++length;
        }

        return length >= 4;
    }

    bool AuthService::emailExists(const char *email) const
    {
        return studentStorage.emailExists(email) || teacherStorage.emailExists(email);
    }

    bool AuthService::emailExistsForAnotherUser(const char *email, int currentUserId) const
    {
        Student student = {};
        if (studentStorage.getByEmail(email, student) && student.id != currentUserId)
        {
            return true;
        }

        Teacher teacher = {};
        if (teacherStorage.getByEmail(email, teacher) && teacher.id != currentUserId)
        {
            return true;
        }

        return false;
    }
}
