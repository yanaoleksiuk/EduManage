#pragma once

#include "../models/Course.h"
#include "../models/Group.h"
#include "../models/Lesson.h"
#include "../models/Question.h"
#include "../models/Student.h"
#include "../models/Teacher.h"
#include "../models/Test.h"
#include "../models/Theme.h"
#include "../models/User.h"
#include "../storage/CourseStorage.h"
#include "../storage/GroupStorage.h"
#include "../storage/LessonStorage.h"
#include "../storage/QuestionStorage.h"
#include "../storage/StudentStorage.h"
#include "../storage/TestStorage.h"
#include "../storage/TeacherStorage.h"
#include "../storage/ThemeStorage.h"
#include "../storage/HomeworkStorage.h"
#include "../storage/TestResultStorage.h"
#include "../models/HomeworkSubmission.h"
#include "../models/TestResult.h"
#include "AuthService.h"

namespace EduManage
{
    struct ScheduleItem
    {
        int lessonId;
        int courseId;
        char courseName[MAX_COURSE_NAME_LENGTH];
        char lessonTitle[MAX_LESSON_TITLE_LENGTH];
        char date[MAX_DATE_LENGTH];
        char time[MAX_TIME_LENGTH];
    };

    enum CreateCourseResultCode
    {
        CREATE_COURSE_OK,
        CREATE_COURSE_INVALID_TEACHER,
        CREATE_COURSE_INVALID_NAME,
        CREATE_COURSE_LIMIT_REACHED
    };

    struct CreateCourseResult
    {
        CreateCourseResultCode code;
        Course course;
    };

    enum AssignStudentResultCode
    {
        ASSIGN_STUDENT_OK,
        ASSIGN_STUDENT_INVALID_TEACHER,
        ASSIGN_STUDENT_INVALID_STUDENT,
        ASSIGN_STUDENT_ALREADY_LINKED,
        ASSIGN_STUDENT_LIMIT_REACHED
    };

    struct AssignStudentResult
    {
        AssignStudentResultCode code;
    };

    enum CreateGroupResultCode
    {
        CREATE_GROUP_OK,
        CREATE_GROUP_INVALID_TEACHER,
        CREATE_GROUP_INVALID_NAME,
        CREATE_GROUP_INVALID_STUDENTS,
        CREATE_GROUP_LIMIT_REACHED
    };

    struct CreateGroupResult
    {
        CreateGroupResultCode code;
        Group group;
    };

    enum GrantCourseAccessResultCode
    {
        GRANT_COURSE_ACCESS_OK,
        GRANT_COURSE_ACCESS_INVALID_TEACHER,
        GRANT_COURSE_ACCESS_INVALID_COURSE,
        GRANT_COURSE_ACCESS_INVALID_STUDENT,
        GRANT_COURSE_ACCESS_INVALID_GROUP,
        GRANT_COURSE_ACCESS_ALREADY_GRANTED,
        GRANT_COURSE_ACCESS_LIMIT_REACHED
    };

    struct GrantCourseAccessResult
    {
        GrantCourseAccessResultCode code;
    };

    enum CreateThemeResultCode
    {
        CREATE_THEME_OK,
        CREATE_THEME_INVALID_TEACHER,
        CREATE_THEME_INVALID_COURSE,
        CREATE_THEME_INVALID_NAME,
        CREATE_THEME_LIMIT_REACHED
    };

    struct CreateThemeResult
    {
        CreateThemeResultCode code;
        Theme theme;
    };

    enum CreateLessonResultCode
    {
        CREATE_LESSON_OK,
        CREATE_LESSON_INVALID_TEACHER,
        CREATE_LESSON_INVALID_THEME,
        CREATE_LESSON_INVALID_TITLE,
        CREATE_LESSON_INVALID_MATERIAL,
        CREATE_LESSON_LIMIT_REACHED
    };

    struct CreateLessonResult
    {
        CreateLessonResultCode code;
        Lesson lesson;
    };

    enum CreateTestResultCode
    {
        CREATE_TEST_OK,
        CREATE_TEST_INVALID_TEACHER,
        CREATE_TEST_INVALID_COURSE,
        CREATE_TEST_INVALID_TITLE,
        CREATE_TEST_INVALID_QUESTIONS,
        CREATE_TEST_LIMIT_REACHED
    };

    struct CreateTestResult
    {
        CreateTestResultCode code;
        Test test;
    };

    enum TakeTestResultCode
    {
        TAKE_TEST_OK,
        TAKE_TEST_INVALID_STUDENT,
        TAKE_TEST_INVALID_TEST,
        TAKE_TEST_NOT_ENROLLED,
        TAKE_TEST_ALREADY_PASSED,
        TAKE_TEST_INVALID_ANSWERS,
        TAKE_TEST_LIMIT_REACHED
    };

    struct TakeTestResult
    {
        TakeTestResultCode code;
        double score;
        double maxScore;
        bool passed;
    };

    class CourseService
    {
    public:
        CourseService(CourseStorage &courseStorage,
                      StudentStorage &studentStorage,
                      TeacherStorage &teacherStorage,
                      GroupStorage &groupStorage,
                      ThemeStorage &themeStorage,
                      LessonStorage &lessonStorage,
                      TestStorage &testStorage,
                      QuestionStorage &questionStorage,
                      HomeworkStorage &homeworkStorage);

        CreateCourseResult createCourse(int teacherId, const char *courseName);
        AssignStudentResult addTeacherStudentByEmail(int teacherId, const char *studentEmail);
        CreateGroupResult createGroup(int teacherId, const char *groupName, const int *studentIds, int studentCount);
        bool deleteGroup(int teacherId, int groupId);
        GrantCourseAccessResult grantCourseAccessToStudent(int teacherId, int courseId, int studentId);
        GrantCourseAccessResult grantCourseAccessToGroup(int teacherId, int courseId, int groupId);

        CreateThemeResult createTheme(int teacherId, int courseId, const char *themeName);
        CreateLessonResult createLesson(int teacherId, int themeId, const char *lessonTitle, const char *material, const char *date, const char *time, const char *homeworkLink);
        CreateTestResult createTest(int teacherId, int courseId, const char *testTitle, const Question *questions, int questionCount);
        TakeTestResult takeTest(int studentId, int testId, const int *answerMasks, int answerCount);

        bool getTestResult(int studentId, int testId, TestResult &result) const;
        int getTestResultsForStudent(int studentId, TestResult *buf, int maxCount) const;

        bool markLessonCompleted(int studentId, int lessonId);
        bool markLessonIncomplete(int studentId, int lessonId);
        bool saveHomeworkSubmission(int studentId, int lessonId, const char *studentName, const char *homeworkLink);
        int getSubmissionsForLesson(int lessonId, HomeworkSubmission *buf, int maxCount) const;
        bool getStudentSubmission(int studentId, int lessonId, HomeworkSubmission &out) const;
        bool isLessonCompletedByStudent(int studentId, int lessonId) const;
        void getStudentName(int studentId, char *buf, int bufSize) const;

        bool updateCourseName(int teacherId, int courseId, const char *courseName);
        bool updateThemeTitle(int teacherId, int themeId, const char *themeName);
        bool updateLesson(int teacherId, int lessonId, const char *lessonTitle, const char *material, const char *date, const char *time, const char *homeworkLink);
        bool updateTest(int teacherId, int testId, const char *testTitle, const Question *questions, int questionCount);
        bool clearLessonMaterial(int teacherId, int lessonId);
        bool clearLessonHomework(int teacherId, int lessonId);
        bool removeStudentFromTeacher(int teacherId, int studentId);

        bool deleteCourse(int teacherId, int courseId);
        bool deleteTheme(int teacherId, int themeId);
        bool deleteLesson(int teacherId, int lessonId);
        bool deleteTest(int teacherId, int testId);

        int searchCourses(const char *query, Course *buffer, int maxCount) const;
        int searchThemes(const char *query, Theme *buffer, int maxCount) const;
        int searchLessons(const char *query, Lesson *buffer, int maxCount) const;
        int searchTests(const char *query, Test *buffer, int maxCount) const;

        int getTeacherStudents(int teacherId, Student *buffer, int maxCount) const;
        int getTeacherGroups(int teacherId, Group *buffer, int maxCount) const;
        int getUpcomingScheduleForTeacher(int teacherId, ScheduleItem *buffer, int maxCount) const;
        int getUpcomingScheduleForStudent(int studentId, ScheduleItem *buffer, int maxCount) const;

        bool getAccountById(int userId, User &user, AccountRole &role) const;
        bool getStudentById(int studentId, Student &student) const;
        bool getTeacherById(int teacherId, Teacher &teacher) const;
        bool getGroupById(int groupId, Group &group) const;
        bool getCourseById(int courseId, Course &course) const;
        bool getThemeById(int themeId, Theme &theme) const;
        bool getLessonById(int lessonId, Lesson &lesson) const;
        bool getTestById(int testId, Test &test) const;
        bool getQuestionById(int questionId, Question &question) const;

        int getAllCourses(Course *buffer, int maxCount) const;
        int getAllThemes(Theme *buffer, int maxCount) const;
        int getAllLessons(Lesson *buffer, int maxCount) const;
        int getAllTests(Test *buffer, int maxCount) const;

    private:
        bool containsId(const int *values, int count, int id) const;
        void removeIdFromList(int *values, int &count, int id) const;
        bool isTeacherOwnerOfCourse(int teacherId, const Course &course) const;
        bool teacherManagesStudent(const Teacher &teacher, int studentId) const;
        void updateStudentProgress(Student &student);
        bool getCourseByLessonId(int lessonId, Course &course, Theme &theme, Lesson &lesson) const;
        bool getCourseByTestId(int testId, Course &course, Test &test) const;
        void removeLessonFromStudents(int courseId, int lessonId);
        void removeTestFromStudents(int courseId, int testId);
        void removeCourseFromStudents(int courseId);
        void deleteQuestionsForTest(const Test &test);
        double calculateQuestionScore(const Question &question, int selectedMask) const;
        int countCorrectOptions(const Question &question) const;
        long long makeDateTimeKey(const char *date, const char *time) const;
        long long getNowKey() const;
        int collectScheduleForCourse(const Course &course, ScheduleItem *buffer, int maxCount, int count) const;
        void sortScheduleItems(ScheduleItem *items, int count) const;

        CourseStorage &courseStorage;
        StudentStorage &studentStorage;
        TeacherStorage &teacherStorage;
        GroupStorage &groupStorage;
        ThemeStorage &themeStorage;
        LessonStorage &lessonStorage;
        TestStorage &testStorage;
        QuestionStorage &questionStorage;
        TestResultStorage testResultStorage_;
        HomeworkStorage &homeworkStorage;
    };
}
