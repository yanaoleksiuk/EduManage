#pragma once
#include <cstring>

namespace EduManage
{

    struct ValidationResult
    {
        bool isValid;
        char errorMessage[128];

        static ValidationResult ok()
        {
            ValidationResult r;
            r.isValid = true;
            r.errorMessage[0] = '\0';
            return r;
        }

        static ValidationResult fail(const char *msg)
        {
            ValidationResult r;
            r.isValid = false;
            strncpy(r.errorMessage, msg, sizeof(r.errorMessage) - 1);
            r.errorMessage[sizeof(r.errorMessage) - 1] = '\0';
            return r;
        }
    };

    class Validator
    {
    public:
        static ValidationResult validateFullName(const char *name)
        {
            if (name == nullptr || name[0] == '\0')
                return ValidationResult::fail("Ім'я не може бути порожнім");
            if (strlen(name) > 49)
                return ValidationResult::fail("Ім'я надто довге (максимум 49 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validateEmail(const char *email)
        {
            if (email == nullptr || email[0] == '\0')
                return ValidationResult::fail("Email не може бути порожнім");
            if (strchr(email, '@') == nullptr)
                return ValidationResult::fail("Email повинен містити символ @");
            if (strchr(email, '.') == nullptr)
                return ValidationResult::fail("Email повинен містити крапку");
            if (strlen(email) > 49)
                return ValidationResult::fail("Email надто довгий (максимум 49 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validatePassword(const char *password)
        {
            if (password == nullptr || password[0] == '\0')
                return ValidationResult::fail("Пароль не може бути порожнім");
            size_t len = strlen(password);
            if (len < 4)
                return ValidationResult::fail("Пароль повинен мати мінімум 4 символи");
            if (len > 19)
                return ValidationResult::fail("Пароль надто довгий (максимум 19 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validateCourseName(const char *name)
        {
            if (name == nullptr || name[0] == '\0')
                return ValidationResult::fail("Назва курсу не може бути порожньою");
            if (strlen(name) > 39)
                return ValidationResult::fail("Назва курсу надто довга (максимум 39 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validateThemeName(const char *name)
        {
            if (name == nullptr || name[0] == '\0')
                return ValidationResult::fail("Назва теми не може бути порожньою");
            if (strlen(name) > 39)
                return ValidationResult::fail("Назва теми надто довга (максимум 39 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validateLessonTitle(const char *title)
        {
            if (title == nullptr || title[0] == '\0')
                return ValidationResult::fail("Заголовок уроку не може бути порожнім");
            if (strlen(title) > 59)
                return ValidationResult::fail("Заголовок уроку надто довгий (максимум 59 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validateTestTitle(const char *title)
        {
            if (title == nullptr || title[0] == '\0')
                return ValidationResult::fail("Назва тесту не може бути порожньою");
            if (strlen(title) > 59)
                return ValidationResult::fail("Назва тесту надто довга (максимум 59 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validateGroupName(const char *name)
        {
            if (name == nullptr || name[0] == '\0')
                return ValidationResult::fail("Назва групи не може бути порожньою");
            if (strlen(name) > 49)
                return ValidationResult::fail("Назва групи надто довга (максимум 49 символів)");
            return ValidationResult::ok();
        }

        static ValidationResult validatePositiveId(int id)
        {
            if (id <= 0)
                return ValidationResult::fail("ID повинен бути більше нуля");
            return ValidationResult::ok();
        }
    };

}
