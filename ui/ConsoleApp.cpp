#include "ConsoleApp.h"

#include <cstring>
#include <iostream>
#include <limits>

#include "../common/ModelLimits.h"
#include "../common/TextUtils.h"

namespace EduManage
{
    ConsoleApp::ConsoleApp(AuthService &authService, CourseService &courseService)
        : authService(authService),
          courseService(courseService),
          isLoggedIn(false),
          currentRole(ACCOUNT_ROLE_NONE),
          currentUser(),
          selectedCourseId(0) {}

    void ConsoleApp::run()
    {
        int choice = -1;

        while (choice != 0)
        {
            showMenu();
            choice = readInt("Choose an action: ");

            switch (choice)
            {
            case 1:
                registerStudent();
                break;
            case 2:
                registerTeacher();
                break;
            case 3:
                login();
                break;
            case 4:
                logout();
                break;
            case 5:
                createCourse();
                break;
            case 6:
                createTheme();
                break;
            case 7:
                createLesson();
                break;
            case 8:
                createTest();
                break;
            case 9:
                addStudentToTeacherList();
                break;
            case 10:
                createGroup();
                break;
            case 11:
                grantCourseToStudent();
                break;
            case 12:
                grantCourseToGroup();
                break;
            case 13:
                showTeacherStudentsAndGroups();
                break;
            case 14:
                showStudentReport();
                break;
            case 15:
                showSchedule();
                break;
            case 16:
                markLessonCompleted();
                break;
            case 17:
                markLessonIncomplete();
                break;
            case 18:
                takeTest();
                break;
            case 19:
                editMyProfile();
                break;
            case 20:
                editCourse();
                break;
            case 21:
                editTheme();
                break;
            case 22:
                editLesson();
                break;
            case 23:
                editTest();
                break;
            case 24:
                deleteCourse();
                break;
            case 25:
                deleteTheme();
                break;
            case 26:
                deleteLesson();
                break;
            case 27:
                deleteTest();
                break;
            case 28:
                clearLessonMaterial();
                break;
            case 29:
                clearLessonHomework();
                break;
            case 30:
                searchContent();
                break;
            case 31:
                showCurrentAccount();
                break;
            case 32:
                showCourses();
                break;
            case 0:
                std::cout << "Application closed.\n";
                break;
            default:
                std::cout << "Unknown command.\n";
                break;
            }
        }
    }

    void ConsoleApp::showMenu() const
    {
        std::cout << "\n===== EduManage =====\n";
        std::cout << "1. Register student\n";
        std::cout << "2. Register teacher\n";
        std::cout << "3. Login\n";
        std::cout << "4. Logout\n";
        std::cout << "5. Create course\n";
        std::cout << "6. Create theme for course\n";
        std::cout << "7. Create lesson and add materials to theme\n";
        std::cout << "8. Create test for course\n";
        std::cout << "9. Add student to my list by email\n";
        std::cout << "10. Create group from my students\n";
        std::cout << "11. Grant course access to student\n";
        std::cout << "12. Grant course access to group\n";
        std::cout << "13. Show my students and groups\n";
        std::cout << "14. View student progress and results\n";
        std::cout << "15. Show schedule\n";
        std::cout << "16. Mark lesson as completed\n";
        std::cout << "17. Remove completed lesson mark\n";
        std::cout << "18. Pass test\n";
        std::cout << "19. Edit my profile\n";
        std::cout << "20. Edit course\n";
        std::cout << "21. Edit theme\n";
        std::cout << "22. Edit lesson\n";
        std::cout << "23. Edit test\n";
        std::cout << "24. Delete course\n";
        std::cout << "25. Delete theme\n";
        std::cout << "26. Delete lesson\n";
        std::cout << "27. Delete test\n";
        std::cout << "28. Delete lesson material\n";
        std::cout << "29. Delete homework link\n";
        std::cout << "30. Search content\n";
        std::cout << "31. My account\n";
        std::cout << "32. Show courses\n";
        std::cout << "0. Exit\n";

        if (isLoggedIn)
        {
            std::cout << "Logged in as: " << currentUser.fullName << " [" << getRoleLabel() << "]\n";
            if (currentRole == ACCOUNT_ROLE_TEACHER && selectedCourseId > 0)
            {
                Course selectedCourse = {};
                if (courseService.getCourseById(selectedCourseId, selectedCourse) &&
                    selectedCourse.creatorTeacherId == currentUser.id)
                {
                    std::cout << "Selected course: [" << selectedCourse.id << "] " << selectedCourse.name << "\n";
                }
            }
        }
    }

    void ConsoleApp::registerStudent()
    {
        char fullName[MAX_NAME_LENGTH] = {};
        char email[MAX_EMAIL_LENGTH] = {};
        char password[MAX_PASSWORD_LENGTH] = {};
        readLine("Full name: ", fullName, MAX_NAME_LENGTH);
        readLine("Email: ", email, MAX_EMAIL_LENGTH);
        readLine("Password: ", password, MAX_PASSWORD_LENGTH);
        const RegisterResult result = authService.registerStudent(fullName, email, password);
        std::cout << (result.code == REGISTER_OK ? "Student registered.\n" : "Registration failed.\n");
    }

    void ConsoleApp::registerTeacher()
    {
        char fullName[MAX_NAME_LENGTH] = {};
        char email[MAX_EMAIL_LENGTH] = {};
        char password[MAX_PASSWORD_LENGTH] = {};
        readLine("Full name: ", fullName, MAX_NAME_LENGTH);
        readLine("Email: ", email, MAX_EMAIL_LENGTH);
        readLine("Password: ", password, MAX_PASSWORD_LENGTH);
        const RegisterResult result = authService.registerTeacher(fullName, email, password);
        std::cout << (result.code == REGISTER_OK ? "Teacher registered.\n" : "Registration failed.\n");
    }

    void ConsoleApp::login()
    {
        char email[MAX_EMAIL_LENGTH] = {};
        char password[MAX_PASSWORD_LENGTH] = {};
        readLine("Email: ", email, MAX_EMAIL_LENGTH);
        readLine("Password: ", password, MAX_PASSWORD_LENGTH);
        const LoginResult result = authService.login(email, password);
        if (result.code != LOGIN_OK)
        {
            std::cout << "Login failed.\n";
            return;
        }

        currentUser = result.user;
        currentRole = result.role;
        isLoggedIn = true;
        std::cout << "Login successful.\n";
        selectedCourseId = 0;
        showCurrentAccount();
    }

    void ConsoleApp::logout()
    {
        if (!isLoggedIn)
        {
            std::cout << "No active session.\n";
            return;
        }

        isLoggedIn = false;
        currentRole = ACCOUNT_ROLE_NONE;
        currentUser = {};
        selectedCourseId = 0;
        std::cout << "Logged out.\n";
    }

    void ConsoleApp::createCourse()
    {
        if (!ensureTeacher())
            return;
        char courseName[MAX_COURSE_NAME_LENGTH] = {};
        readLine("Course name: ", courseName, MAX_COURSE_NAME_LENGTH);
        const CreateCourseResult result = courseService.createCourse(currentUser.id, courseName);
        if (result.code != CREATE_COURSE_OK)
        {
            std::cout << "Course creation failed.\n";
            return;
        }

        std::cout << "Course created: [" << result.course.id << "] " << result.course.name << "\n";
        selectedCourseId = result.course.id;
        std::cout << "Now you can add themes or tests to this course.\n";
        refreshCurrentUser();
    }

    void ConsoleApp::createTheme()
    {
        if (!ensureTeacher())
            return;
        Course course = {};
        if (!resolveSelectedTeacherCourse(course))
        {
            return;
        }

        char themeName[MAX_THEME_NAME_LENGTH] = {};
        readLine("Theme name: ", themeName, MAX_THEME_NAME_LENGTH);
        const CreateThemeResult result = courseService.createTheme(currentUser.id, course.id, themeName);
        if (result.code != CREATE_THEME_OK)
        {
            std::cout << "Theme creation failed.\n";
            return;
        }

        std::cout << "Theme created: [" << result.theme.id << "] " << result.theme.title
                  << " for course \"" << course.name << "\".\n";
    }

    void ConsoleApp::createLesson()
    {
        if (!ensureTeacher())
            return;
        Course course = {};
        if (!resolveSelectedTeacherCourse(course))
        {
            return;
        }

        if (course.themeCount == 0)
        {
            std::cout << "This course has no themes yet. Create a theme first.\n";
            return;
        }

        printCourseThemes(course.id);
        const int themeId = readInt("Theme ID: ");

        Theme theme = {};
        if (!courseService.getThemeById(themeId, theme) || theme.courseId != course.id)
        {
            std::cout << "Choose a theme that belongs to this course.\n";
            return;
        }

        char title[MAX_LESSON_TITLE_LENGTH] = {};
        char material[MAX_MATERIAL_LENGTH] = {};
        char date[MAX_DATE_LENGTH] = {};
        char time[MAX_TIME_LENGTH] = {};
        char homework[MAX_LINK_LENGTH] = {};
        readLine("Lesson title: ", title, MAX_LESSON_TITLE_LENGTH);
        readLine("Materials: ", material, MAX_MATERIAL_LENGTH);
        readLine("Date (YYYY-MM-DD or DD.MM.YYYY): ", date, MAX_DATE_LENGTH);
        readLine("Time (HH:MM): ", time, MAX_TIME_LENGTH);
        readLine("Homework link: ", homework, MAX_LINK_LENGTH);
        const CreateLessonResult result =
            courseService.createLesson(currentUser.id, themeId, title, material, date, time, homework);
        if (result.code != CREATE_LESSON_OK)
        {
            std::cout << "Lesson creation failed.\n";
            return;
        }

        std::cout << "Lesson created: [" << result.lesson.id << "] " << result.lesson.title
                  << " in theme \"" << theme.title << "\".\n";
    }

    void ConsoleApp::createTest()
    {
        if (!ensureTeacher())
            return;
        Course course = {};
        if (!resolveSelectedTeacherCourse(course))
        {
            return;
        }

        char testTitle[MAX_TEST_TITLE_LENGTH] = {};
        readLine("Test title: ", testTitle, MAX_TEST_TITLE_LENGTH);
        const int questionCount = readInt("Question count (1-20): ");
        if (questionCount <= 0 || questionCount > MAX_TEST_QUESTIONS)
        {
            std::cout << "Invalid question count.\n";
            return;
        }

        Question questions[MAX_TEST_QUESTIONS] = {};
        for (int i = 0; i < questionCount; ++i)
        {
            std::cout << "\nQuestion " << (i + 1) << "\n";
            readQuestion(questions[i]);
        }

        const CreateTestResult result = courseService.createTest(currentUser.id, course.id, testTitle, questions, questionCount);
        if (result.code != CREATE_TEST_OK)
        {
            std::cout << "Test creation failed.\n";
            return;
        }

        std::cout << "Test created: [" << result.test.id << "] " << result.test.title
                  << " for course \"" << course.name << "\".\n";
    }

    void ConsoleApp::addStudentToTeacherList()
    {
        if (!ensureTeacher())
            return;
        char email[MAX_EMAIL_LENGTH] = {};
        readLine("Student email: ", email, MAX_EMAIL_LENGTH);
        const AssignStudentResult result = courseService.addTeacherStudentByEmail(currentUser.id, email);
        std::cout << (result.code == ASSIGN_STUDENT_OK ? "Student added to your list.\n" : "Unable to add student.\n");
    }

    void ConsoleApp::createGroup()
    {
        if (!ensureTeacher())
            return;

        Student students[MAX_STORAGE_RECORDS] = {};
        const int studentCountAvailable = courseService.getTeacherStudents(currentUser.id, students, MAX_STORAGE_RECORDS);
        if (studentCountAvailable == 0)
        {
            std::cout << "Add students to your list first.\n";
            return;
        }

        std::cout << "Your students:\n";
        for (int i = 0; i < studentCountAvailable; ++i)
        {
            std::cout << "  [" << students[i].id << "] " << students[i].fullName << " <" << students[i].email << ">\n";
        }

        char groupName[MAX_NAME_LENGTH] = {};
        readLine("Group name: ", groupName, MAX_NAME_LENGTH);
        const int count = readInt("How many students in the group: ");
        if (count <= 0 || count > MAX_GROUP_STUDENTS)
        {
            std::cout << "Invalid count.\n";
            return;
        }

        int studentIds[MAX_GROUP_STUDENTS] = {};
        for (int i = 0; i < count; ++i)
        {
            studentIds[i] = readInt("Student ID: ");
        }

        const CreateGroupResult result = courseService.createGroup(currentUser.id, groupName, studentIds, count);
        std::cout << (result.code == CREATE_GROUP_OK ? "Group created.\n" : "Group creation failed.\n");
    }

    void ConsoleApp::grantCourseToStudent()
    {
        if (!ensureTeacher())
            return;
        Student students[MAX_STORAGE_RECORDS] = {};
        const int count = courseService.getTeacherStudents(currentUser.id, students, MAX_STORAGE_RECORDS);
        if (count == 0)
        {
            std::cout << "No linked students.\n";
            return;
        }

        std::cout << "Linked students:\n";
        for (int i = 0; i < count; ++i)
        {
            std::cout << "  [" << students[i].id << "] " << students[i].fullName << "\n";
        }

        const int courseId = readInt("Course ID: ");
        const int studentId = readInt("Student ID: ");
        const GrantCourseAccessResult result = courseService.grantCourseAccessToStudent(currentUser.id, courseId, studentId);
        std::cout << (result.code == GRANT_COURSE_ACCESS_OK ? "Course access granted.\n" : "Grant failed.\n");
    }

    void ConsoleApp::grantCourseToGroup()
    {
        if (!ensureTeacher())
            return;
        Group groups[MAX_STORAGE_RECORDS] = {};
        const int count = courseService.getTeacherGroups(currentUser.id, groups, MAX_STORAGE_RECORDS);
        if (count == 0)
        {
            std::cout << "No groups found.\n";
            return;
        }

        std::cout << "Your groups:\n";
        for (int i = 0; i < count; ++i)
        {
            std::cout << "  [" << groups[i].id << "] " << groups[i].name << " (" << groups[i].studentCount << " students)\n";
        }

        const int courseId = readInt("Course ID: ");
        const int groupId = readInt("Group ID: ");
        const GrantCourseAccessResult result = courseService.grantCourseAccessToGroup(currentUser.id, courseId, groupId);
        std::cout << (result.code == GRANT_COURSE_ACCESS_OK ? "Group access granted.\n" : "Grant failed.\n");
    }

    void ConsoleApp::showTeacherStudentsAndGroups() const
    {
        if (!ensureTeacher())
            return;

        Student students[MAX_STORAGE_RECORDS] = {};
        Group groups[MAX_STORAGE_RECORDS] = {};
        const int studentCount = courseService.getTeacherStudents(currentUser.id, students, MAX_STORAGE_RECORDS);
        const int groupCount = courseService.getTeacherGroups(currentUser.id, groups, MAX_STORAGE_RECORDS);

        std::cout << "\nStudents:\n";
        for (int i = 0; i < studentCount; ++i)
        {
            std::cout << "  [" << students[i].id << "] " << students[i].fullName << " <" << students[i].email << ">\n";
        }

        std::cout << "Groups:\n";
        for (int i = 0; i < groupCount; ++i)
        {
            std::cout << "  [" << groups[i].id << "] " << groups[i].name << ": ";
            for (int j = 0; j < groups[i].studentCount; ++j)
            {
                Student student = {};
                if (courseService.getStudentById(groups[i].studentIds[j], student))
                {
                    std::cout << student.fullName;
                    if (j + 1 < groups[i].studentCount)
                    {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << "\n";
        }
    }

    void ConsoleApp::showStudentReport() const
    {
        if (!ensureTeacher())
            return;
        const int studentId = readInt("Student ID: ");

        Student student = {};
        if (!courseService.getStudentById(studentId, student))
        {
            std::cout << "Student not found.\n";
            return;
        }

        std::cout << "\n===== Student Report =====\n";
        printStudentAccount(student);
        std::cout << "Completed lessons:\n";
        for (int i = 0; i < student.completedLessonCount; ++i)
        {
            Lesson lesson = {};
            if (courseService.getLessonById(student.completedLessonIds[i], lesson))
            {
                std::cout << "  [" << lesson.id << "] " << lesson.title << " | " << lesson.date << " " << lesson.time << "\n";
            }
        }

        std::cout << "Passed tests:\n";
        for (int i = 0; i < student.passedTestCount; ++i)
        {
            Test test = {};
            if (courseService.getTestById(student.passedTestIds[i], test))
            {
                std::cout << "  [" << test.id << "] " << test.title << " | "
                          << student.passedTestScores[i] << "/" << student.passedTestMaxScores[i] << "\n";
            }
        }
    }

    void ConsoleApp::showSchedule() const
    {
        if (!ensureLoggedIn())
            return;

        ScheduleItem items[MAX_STORAGE_RECORDS] = {};
        int count = 0;
        if (currentRole == ACCOUNT_ROLE_TEACHER)
        {
            count = courseService.getUpcomingScheduleForTeacher(currentUser.id, items, MAX_STORAGE_RECORDS);
        }
        else if (currentRole == ACCOUNT_ROLE_STUDENT)
        {
            count = courseService.getUpcomingScheduleForStudent(currentUser.id, items, MAX_STORAGE_RECORDS);
        }

        if (count == 0)
        {
            std::cout << "No upcoming lessons found.\n";
            return;
        }

        std::cout << "\nUpcoming schedule:\n";
        for (int i = 0; i < count; ++i)
        {
            std::cout << "  " << items[i].date << " " << items[i].time
                      << " | " << items[i].courseName
                      << " | " << items[i].lessonTitle
                      << " | lesson ID " << items[i].lessonId << "\n";
        }
    }

    void ConsoleApp::editMyProfile()
    {
        if (!ensureLoggedIn())
            return;

        char fullName[MAX_NAME_LENGTH] = {};
        char email[MAX_EMAIL_LENGTH] = {};
        char password[MAX_PASSWORD_LENGTH] = {};
        readLine("New full name: ", fullName, MAX_NAME_LENGTH);
        readLine("New email: ", email, MAX_EMAIL_LENGTH);
        readLine("New password: ", password, MAX_PASSWORD_LENGTH);

        UpdateAccountResult result = {};
        if (currentRole == ACCOUNT_ROLE_TEACHER)
        {
            result = authService.updateTeacherProfile(currentUser.id, fullName, email, password);
        }
        else if (currentRole == ACCOUNT_ROLE_STUDENT)
        {
            result = authService.updateStudentProfile(currentUser.id, fullName, email, password);
        }

        if (result.code != UPDATE_ACCOUNT_OK)
        {
            std::cout << "Profile update failed.\n";
            return;
        }

        currentUser = result.user;
        currentRole = result.role;
        std::cout << "Profile updated.\n";
    }

    void ConsoleApp::editCourse()
    {
        if (!ensureTeacher())
            return;
        const int courseId = readInt("Course ID: ");
        char name[MAX_COURSE_NAME_LENGTH] = {};
        readLine("New course name: ", name, MAX_COURSE_NAME_LENGTH);
        std::cout << (courseService.updateCourseName(currentUser.id, courseId, name) ? "Course updated.\n" : "Course update failed.\n");
    }

    void ConsoleApp::editTheme()
    {
        if (!ensureTeacher())
            return;
        const int themeId = readInt("Theme ID: ");
        char name[MAX_THEME_NAME_LENGTH] = {};
        readLine("New theme name: ", name, MAX_THEME_NAME_LENGTH);
        std::cout << (courseService.updateThemeTitle(currentUser.id, themeId, name) ? "Theme updated.\n" : "Theme update failed.\n");
    }

    void ConsoleApp::editLesson()
    {
        if (!ensureTeacher())
            return;
        const int lessonId = readInt("Lesson ID: ");
        char title[MAX_LESSON_TITLE_LENGTH] = {};
        char material[MAX_MATERIAL_LENGTH] = {};
        char date[MAX_DATE_LENGTH] = {};
        char time[MAX_TIME_LENGTH] = {};
        char homework[MAX_LINK_LENGTH] = {};
        readLine("New lesson title: ", title, MAX_LESSON_TITLE_LENGTH);
        readLine("New materials: ", material, MAX_MATERIAL_LENGTH);
        readLine("New date: ", date, MAX_DATE_LENGTH);
        readLine("New time: ", time, MAX_TIME_LENGTH);
        readLine("New homework link: ", homework, MAX_LINK_LENGTH);
        std::cout << (courseService.updateLesson(currentUser.id, lessonId, title, material, date, time, homework)
                          ? "Lesson updated.\n"
                          : "Lesson update failed.\n");
    }

    void ConsoleApp::editTest()
    {
        if (!ensureTeacher())
            return;
        const int testId = readInt("Test ID: ");
        char title[MAX_TEST_TITLE_LENGTH] = {};
        readLine("New test title: ", title, MAX_TEST_TITLE_LENGTH);
        const int questionCount = readInt("Question count: ");
        if (questionCount <= 0 || questionCount > MAX_TEST_QUESTIONS)
        {
            std::cout << "Invalid question count.\n";
            return;
        }

        Question questions[MAX_TEST_QUESTIONS] = {};
        for (int i = 0; i < questionCount; ++i)
        {
            std::cout << "\nQuestion " << (i + 1) << "\n";
            readQuestion(questions[i]);
        }

        std::cout << (courseService.updateTest(currentUser.id, testId, title, questions, questionCount)
                          ? "Test updated.\n"
                          : "Test update failed.\n");
    }

    void ConsoleApp::deleteCourse()
    {
        if (!ensureTeacher())
            return;
        std::cout << (courseService.deleteCourse(currentUser.id, readInt("Course ID: ")) ? "Course deleted.\n" : "Course deletion failed.\n");
    }

    void ConsoleApp::deleteTheme()
    {
        if (!ensureTeacher())
            return;
        std::cout << (courseService.deleteTheme(currentUser.id, readInt("Theme ID: ")) ? "Theme deleted.\n" : "Theme deletion failed.\n");
    }

    void ConsoleApp::deleteLesson()
    {
        if (!ensureTeacher())
            return;
        std::cout << (courseService.deleteLesson(currentUser.id, readInt("Lesson ID: ")) ? "Lesson deleted.\n" : "Lesson deletion failed.\n");
    }

    void ConsoleApp::deleteTest()
    {
        if (!ensureTeacher())
            return;
        std::cout << (courseService.deleteTest(currentUser.id, readInt("Test ID: ")) ? "Test deleted.\n" : "Test deletion failed.\n");
    }

    void ConsoleApp::clearLessonMaterial()
    {
        if (!ensureTeacher())
            return;
        std::cout << (courseService.clearLessonMaterial(currentUser.id, readInt("Lesson ID: ")) ? "Material deleted.\n" : "Material deletion failed.\n");
    }

    void ConsoleApp::clearLessonHomework()
    {
        if (!ensureTeacher())
            return;
        std::cout << (courseService.clearLessonHomework(currentUser.id, readInt("Lesson ID: ")) ? "Homework link deleted.\n" : "Homework link deletion failed.\n");
    }

    void ConsoleApp::markLessonCompleted()
    {
        if (!ensureStudent())
            return;
        std::cout << (courseService.markLessonCompleted(currentUser.id, readInt("Lesson ID: ")) ? "Lesson completed.\n" : "Unable to mark lesson.\n");
        refreshCurrentUser();
    }

    void ConsoleApp::markLessonIncomplete()
    {
        if (!ensureStudent())
            return;
        std::cout << (courseService.markLessonIncomplete(currentUser.id, readInt("Lesson ID: ")) ? "Completed mark removed.\n" : "Unable to update lesson.\n");
        refreshCurrentUser();
    }

    void ConsoleApp::takeTest()
    {
        if (!ensureStudent())
            return;
        const int testId = readInt("Test ID: ");
        Test test = {};
        if (!courseService.getTestById(testId, test))
        {
            std::cout << "Test not found.\n";
            return;
        }

        int answerMasks[MAX_TEST_QUESTIONS] = {};
        for (int i = 0; i < test.questionCount; ++i)
        {
            Question question = {};
            if (!courseService.getQuestionById(test.questionIds[i], question))
            {
                std::cout << "Question data is unavailable.\n";
                return;
            }

            std::cout << "\n"
                      << (i + 1) << ". " << question.text << "\n";
            for (int j = 0; j < question.optionCount; ++j)
            {
                std::cout << "   " << (j + 1) << ". " << question.options[j] << "\n";
            }
            std::cout << "Points: " << question.score << "\n";
            answerMasks[i] = readOptionMask("Choose options (e.g. 1 3): ", question.optionCount);
        }

        const TakeTestResult result = courseService.takeTest(currentUser.id, testId, answerMasks, test.questionCount);
        if (result.code != TAKE_TEST_OK)
        {
            std::cout << "Test submission failed.\n";
            return;
        }

        std::cout << "Score: " << result.score << "/" << result.maxScore << "\n";
        std::cout << (result.passed ? "Status: passed\n" : "Status: failed\n");
    }

    void ConsoleApp::searchContent() const
    {
        char query[MAX_SEARCH_TEXT_LENGTH] = {};
        readLine("Search query: ", query, MAX_SEARCH_TEXT_LENGTH);

        Course courses[MAX_STORAGE_RECORDS] = {};
        Theme themes[MAX_STORAGE_RECORDS] = {};
        Lesson lessons[MAX_STORAGE_RECORDS] = {};
        Test tests[MAX_STORAGE_RECORDS] = {};

        const int courseCount = courseService.searchCourses(query, courses, MAX_STORAGE_RECORDS);
        const int themeCount = courseService.searchThemes(query, themes, MAX_STORAGE_RECORDS);
        const int lessonCount = courseService.searchLessons(query, lessons, MAX_STORAGE_RECORDS);
        const int testCount = courseService.searchTests(query, tests, MAX_STORAGE_RECORDS);

        std::cout << "\nCourses:\n";
        for (int i = 0; i < courseCount; ++i)
        {
            std::cout << "  [" << courses[i].id << "] " << courses[i].name << "\n";
        }

        std::cout << "Themes:\n";
        for (int i = 0; i < themeCount; ++i)
        {
            std::cout << "  [" << themes[i].id << "] " << themes[i].title << "\n";
        }

        std::cout << "Lessons:\n";
        for (int i = 0; i < lessonCount; ++i)
        {
            std::cout << "  [" << lessons[i].id << "] " << lessons[i].title << "\n";
        }

        std::cout << "Tests:\n";
        for (int i = 0; i < testCount; ++i)
        {
            std::cout << "  [" << tests[i].id << "] " << tests[i].title << "\n";
        }
    }

    void ConsoleApp::showCurrentAccount()
    {
        if (!ensureLoggedIn())
            return;
        refreshCurrentUser();

        if (currentRole == ACCOUNT_ROLE_TEACHER)
        {
            Teacher teacher = {};
            if (courseService.getTeacherById(currentUser.id, teacher))
            {
                printTeacherAccount(teacher);
            }
            return;
        }

        Student student = {};
        if (courseService.getStudentById(currentUser.id, student))
        {
            printStudentAccount(student);
        }
    }

    void ConsoleApp::showMyCourses()
    {
        Course courses[MAX_STORAGE_RECORDS] = {};
        const int count = courseService.getAllCourses(courses, MAX_STORAGE_RECORDS);
        if (count == 0)
        {
            std::cout << "No courses found.\n";
            return;
        }

        for (int i = 0; i < count; ++i)
        {
            std::cout << "[" << courses[i].id << "] " << courses[i].name
                      << " | lessons: " << courses[i].totalLessons
                      << " | tests: " << courses[i].testCount
                      << " | students: " << courses[i].enrolledStudentCount
                      << " | groups: " << courses[i].assignedGroupCount << "\n";
        }

        const int courseId = readInt("Open course details (0 to skip): ");
        if (courseId > 0)
        {
            Course course = {};
            if (courseService.getCourseById(courseId, course) &&
                currentRole == ACCOUNT_ROLE_TEACHER &&
                course.creatorTeacherId == currentUser.id)
            {
                selectedCourseId = courseId;
                std::cout << "Course selected for next actions.\n";
            }
            showCourseDetails(courseId);
        }
    }

    void ConsoleApp::showCourseDetails(int courseId)
    {
        Course course = {};
        if (!courseService.getCourseById(courseId, course))
        {
            std::cout << "Course not found.\n";
            return;
        }

        std::cout << "\nCourse: " << course.name << "\nThemes:\n";
        for (int i = 0; i < course.themeCount; ++i)
        {
            Theme theme = {};
            if (!courseService.getThemeById(course.themeIds[i], theme))
            {
                continue;
            }

            std::cout << "  [" << theme.id << "] " << theme.title << "\n";
            for (int j = 0; j < theme.lessonCount; ++j)
            {
                Lesson lesson = {};
                if (courseService.getLessonById(theme.lessonIds[j], lesson))
                {
                    std::cout << "     lesson [" << lesson.id << "] " << lesson.title
                              << " | " << lesson.date << " " << lesson.time << "\n";
                }
            }
        }

        std::cout << "Tests:\n";
        for (int i = 0; i < course.testCount; ++i)
        {
            Test test = {};
            if (courseService.getTestById(course.testIds[i], test))
            {
                std::cout << "  [" << test.id << "] " << test.title << " | max score " << test.maxScore << "\n";
            }
        }
    }

    bool ConsoleApp::resolveSelectedTeacherCourse(Course &course)
    {
        Teacher teacher = {};
        if (!courseService.getTeacherById(currentUser.id, teacher) || teacher.createdCourseCount == 0)
        {
            std::cout << "Create a course first.\n";
            return false;
        }

        if (selectedCourseId > 0 &&
            courseService.getCourseById(selectedCourseId, course) &&
            course.creatorTeacherId == currentUser.id)
        {
            return true;
        }

        std::cout << "First select a course from the course list.\n";
        printTeacherCourses();
        const int courseId = readInt("Select course ID: ");
        if (!courseService.getCourseById(courseId, course) || course.creatorTeacherId != currentUser.id)
        {
            std::cout << "Choose one of your own courses.\n";
            return false;
        }

        selectedCourseId = courseId;
        std::cout << "Course selected: [" << course.id << "] " << course.name << "\n";
        return true;
    }

    void ConsoleApp::printTeacherCourses() const
    {
        Teacher teacher = {};
        if (!courseService.getTeacherById(currentUser.id, teacher))
        {
            std::cout << "Teacher account was not found.\n";
            return;
        }

        if (teacher.createdCourseCount == 0)
        {
            std::cout << "You have no courses yet. Create a course first.\n";
            return;
        }

        std::cout << "Your courses:\n";
        for (int i = 0; i < teacher.createdCourseCount; ++i)
        {
            Course course = {};
            if (courseService.getCourseById(teacher.createdCourseIds[i], course))
            {
                std::cout << "  [" << course.id << "] " << course.name
                          << " | themes: " << course.themeCount
                          << " | tests: " << course.testCount << "\n";
            }
        }
    }

    void ConsoleApp::printCourseThemes(int courseId) const
    {
        Course course = {};
        if (!courseService.getCourseById(courseId, course))
        {
            return;
        }

        std::cout << "Themes in this course:\n";
        for (int i = 0; i < course.themeCount; ++i)
        {
            Theme theme = {};
            if (courseService.getThemeById(course.themeIds[i], theme))
            {
                std::cout << "  [" << theme.id << "] " << theme.title
                          << " | lessons: " << theme.lessonCount << "\n";
            }
        }
    }

    void ConsoleApp::readLine(const char *label, char *buffer, int size) const
    {
        std::cout << label;
        std::cin.getline(buffer, size);
    }

    int ConsoleApp::readInt(const char *label) const
    {
        int value = 0;
        std::cout << label;
        std::cin >> value;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }

    double ConsoleApp::readDouble(const char *label) const
    {
        double value = 0.0;
        std::cout << label;
        std::cin >> value;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }

    int ConsoleApp::readOptionMask(const char *label, int optionCount) const
    {
        char input[MAX_SELECTION_TEXT_LENGTH] = {};
        readLine(label, input, MAX_SELECTION_TEXT_LENGTH);

        int mask = 0;
        for (int i = 0; input[i] != '\0'; ++i)
        {
            if (input[i] >= '1' && input[i] <= '0' + optionCount)
            {
                mask |= (1 << (input[i] - '1'));
            }
        }
        return mask;
    }

    void ConsoleApp::readQuestion(Question &question) const
    {
        readLine("Text: ", question.text, MAX_QUESTION_TEXT_LENGTH);
        question.optionCount = readInt("Options (2-4): ");
        if (question.optionCount < 2)
        {
            question.optionCount = 2;
        }
        if (question.optionCount > MAX_QUESTION_OPTIONS)
        {
            question.optionCount = MAX_QUESTION_OPTIONS;
        }

        for (int i = 0; i < question.optionCount; ++i)
        {
            char label[32] = {};
            if (i == 0)
            {
                copyText(label, sizeof(label), "Option 1: ");
            }
            else if (i == 1)
            {
                copyText(label, sizeof(label), "Option 2: ");
            }
            else if (i == 2)
            {
                copyText(label, sizeof(label), "Option 3: ");
            }
            else
            {
                copyText(label, sizeof(label), "Option 4: ");
            }
            readLine(label, question.options[i], MAX_OPTION_TEXT_LENGTH);
        }

        question.score = readDouble("Points for this question: ");
        const int correctMask = readOptionMask("Correct options (e.g. 1 3): ", question.optionCount);
        for (int i = 0; i < question.optionCount; ++i)
        {
            question.correctOptionFlags[i] = (correctMask & (1 << i)) != 0 ? 1 : 0;
        }
    }

    bool ConsoleApp::ensureLoggedIn() const
    {
        if (isLoggedIn)
            return true;
        std::cout << "Please login first.\n";
        return false;
    }

    bool ConsoleApp::ensureTeacher() const
    {
        if (!ensureLoggedIn())
            return false;
        if (currentRole == ACCOUNT_ROLE_TEACHER)
            return true;
        std::cout << "Teacher access is required.\n";
        return false;
    }

    bool ConsoleApp::ensureStudent() const
    {
        if (!ensureLoggedIn())
            return false;
        if (currentRole == ACCOUNT_ROLE_STUDENT)
            return true;
        std::cout << "Student access is required.\n";
        return false;
    }

    void ConsoleApp::printBaseUser(const User &user) const
    {
        std::cout << "ID: " << user.id << "\n";
        std::cout << "Full name: " << user.fullName << "\n";
        std::cout << "Email: " << user.email << "\n";
    }

    void ConsoleApp::printTeacherAccount(const Teacher &teacher) const
    {
        std::cout << "\n===== Account =====\n";
        printBaseUser(teacher);
        std::cout << "Role: teacher\n";
        std::cout << "Created courses: " << teacher.createdCourseCount << "\n";
        std::cout << "My students: " << teacher.managedStudentCount << "\n";
        std::cout << "Groups: " << teacher.groupCount << "\n";
    }

    void ConsoleApp::printStudentAccount(const Student &student) const
    {
        std::cout << "\n===== Account =====\n";
        printBaseUser(student);
        std::cout << "Role: student\n";
        std::cout << "Enrolled courses: " << student.enrolledCourseCount << "\n";
        std::cout << "Completed lessons: " << student.completedLessonCount << "\n";
        std::cout << "Passed tests: " << student.passedTestCount << "\n";
        std::cout << "Progress: " << student.totalProgress << "%\n";
    }

    void ConsoleApp::refreshCurrentUser()
    {
        if (!isLoggedIn)
            return;

        User refreshed = {};
        AccountRole role = ACCOUNT_ROLE_NONE;
        if (courseService.getAccountById(currentUser.id, refreshed, role))
        {
            currentUser = refreshed;
            currentRole = role;
        }
    }

    const char *ConsoleApp::getRoleLabel() const
    {
        if (currentRole == ACCOUNT_ROLE_TEACHER)
            return "teacher";
        if (currentRole == ACCOUNT_ROLE_STUDENT)
            return "student";
        return "guest";
    }
}
