#pragma once

#include "../models/Student.h"
#include "../models/Teacher.h"
#include "../services/AuthService.h"
#include "../services/CourseService.h"

namespace EduManage
{
    class ConsoleApp
    {
    public:
        ConsoleApp(AuthService &authService, CourseService &courseService);
        void run();

    private:
        void showMenu() const;
        void registerStudent();
        void registerTeacher();
        void login();
        void logout();

        void createCourse();
        void createTheme();
        void createLesson();
        void createTest();

        void addStudentToTeacherList();
        void createGroup();
        void grantCourseToStudent();
        void grantCourseToGroup();
        void showTeacherStudentsAndGroups() const;
        void showStudentReport() const;
        void showSchedule() const;

        void editMyProfile();
        void editCourse();
        void editTheme();
        void editLesson();
        void editTest();

        void deleteCourse();
        void deleteTheme();
        void deleteLesson();
        void deleteTest();
        void clearLessonMaterial();
        void clearLessonHomework();

        void markLessonCompleted();
        void markLessonIncomplete();
        void takeTest();
        void searchContent() const;
        void showCurrentAccount();
        void showCourses();
        void showMyCourses();
        void showCourseDetails(int courseId);
        void printTeacherCourses() const;
        void printCourseThemes(int courseId) const;
        bool resolveSelectedTeacherCourse(Course &course);

        void readLine(const char *label, char *buffer, int size) const;
        int readInt(const char *label) const;
        double readDouble(const char *label) const;
        int readOptionMask(const char *label, int optionCount) const;
        void readQuestion(Question &question) const;

        bool ensureLoggedIn() const;
        bool ensureTeacher() const;
        bool ensureStudent() const;
        void printBaseUser(const User &user) const;
        void printTeacherAccount(const Teacher &teacher) const;
        void printStudentAccount(const Student &student) const;
        void refreshCurrentUser();
        const char *getRoleLabel() const;

        AuthService &authService;
        CourseService &courseService;
        bool isLoggedIn;
        AccountRole currentRole;
        User currentUser;
        int selectedCourseId;
    };
}
