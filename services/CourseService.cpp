#include "CourseService.h"

#include <ctime>

#include "../common/ModelLimits.h"
#include "../common/TextUtils.h"

namespace EduManage
{
    CourseService::CourseService(CourseStorage &courseStorage,
                                 StudentStorage &studentStorage,
                                 TeacherStorage &teacherStorage,
                                 GroupStorage &groupStorage,
                                 ThemeStorage &themeStorage,
                                 LessonStorage &lessonStorage,
                                 TestStorage &testStorage,
                                 QuestionStorage &questionStorage,
                                 HomeworkStorage &homeworkStorage)
        : courseStorage(courseStorage),
          studentStorage(studentStorage),
          teacherStorage(teacherStorage),
          groupStorage(groupStorage),
          themeStorage(themeStorage),
          lessonStorage(lessonStorage),
          testStorage(testStorage),
          questionStorage(questionStorage),
          homeworkStorage(homeworkStorage) {}

    CreateCourseResult CourseService::createCourse(int teacherId, const char *courseName)
    {
        CreateCourseResult result = {};
        result.code = CREATE_COURSE_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        if (isEmptyText(courseName))
        {
            result.code = CREATE_COURSE_INVALID_NAME;
            return result;
        }

        if (teacher.createdCourseCount >= MAX_USER_CREATED_COURSES)
        {
            result.code = CREATE_COURSE_LIMIT_REACHED;
            return result;
        }

        Course course = {};
        course.id = courseStorage.getNextId();
        course.creatorTeacherId = teacherId;
        copyText(course.name, sizeof(course.name), courseName);

        if (!courseStorage.add(course))
        {
            result.code = CREATE_COURSE_INVALID_NAME;
            return result;
        }

        teacher.createdCourseIds[teacher.createdCourseCount] = course.id;
        ++teacher.createdCourseCount;
        teacherStorage.update(teacher);

        result.code = CREATE_COURSE_OK;
        result.course = course;
        return result;
    }

    AssignStudentResult CourseService::addTeacherStudentByEmail(int teacherId, const char *studentEmail)
    {
        AssignStudentResult result = {};
        result.code = ASSIGN_STUDENT_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        Student student = {};
        if (!studentStorage.getByEmail(studentEmail, student))
        {
            result.code = ASSIGN_STUDENT_INVALID_STUDENT;
            return result;
        }

        if (containsId(teacher.managedStudentIds, teacher.managedStudentCount, student.id))
        {
            result.code = ASSIGN_STUDENT_ALREADY_LINKED;
            return result;
        }

        if (teacher.managedStudentCount >= MAX_TEACHER_STUDENTS)
        {
            result.code = ASSIGN_STUDENT_LIMIT_REACHED;
            return result;
        }

        teacher.managedStudentIds[teacher.managedStudentCount] = student.id;
        ++teacher.managedStudentCount;
        teacherStorage.update(teacher);

        result.code = ASSIGN_STUDENT_OK;
        return result;
    }

    CreateGroupResult CourseService::createGroup(int teacherId, const char *groupName, const int *studentIds, int studentCount)
    {
        CreateGroupResult result = {};
        result.code = CREATE_GROUP_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        if (isEmptyText(groupName))
        {
            result.code = CREATE_GROUP_INVALID_NAME;
            return result;
        }

        if (studentIds == nullptr || studentCount <= 0 || studentCount > MAX_GROUP_STUDENTS)
        {
            result.code = CREATE_GROUP_INVALID_STUDENTS;
            return result;
        }

        if (teacher.groupCount >= MAX_TEACHER_GROUPS)
        {
            result.code = CREATE_GROUP_LIMIT_REACHED;
            return result;
        }

        Group group = {};
        group.id = groupStorage.getNextId();
        group.teacherId = teacherId;
        copyText(group.name, sizeof(group.name), groupName);

        for (int i = 0; i < studentCount; ++i)
        {
            if (!teacherManagesStudent(teacher, studentIds[i]) || containsId(group.studentIds, group.studentCount, studentIds[i]))
            {
                result.code = CREATE_GROUP_INVALID_STUDENTS;
                return result;
            }

            group.studentIds[group.studentCount] = studentIds[i];
            ++group.studentCount;
        }

        if (!groupStorage.add(group))
        {
            result.code = CREATE_GROUP_INVALID_NAME;
            return result;
        }

        teacher.groupIds[teacher.groupCount] = group.id;
        ++teacher.groupCount;
        teacherStorage.update(teacher);

        result.code = CREATE_GROUP_OK;
        result.group = group;
        return result;
    }

    bool CourseService::deleteGroup(int teacherId, int groupId)
    {
        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
            return false;

        int pos = -1;
        for (int i = 0; i < teacher.groupCount; ++i)
        {
            if (teacher.groupIds[i] == groupId)
            {
                pos = i;
                break;
            }
        }
        if (pos == -1)
            return false;

        if (!groupStorage.removeById(groupId))
            return false;

        for (int i = pos; i < teacher.groupCount - 1; ++i)
            teacher.groupIds[i] = teacher.groupIds[i + 1];
        teacher.groupIds[--teacher.groupCount] = 0;
        teacherStorage.update(teacher);

        return true;
    }

    GrantCourseAccessResult CourseService::grantCourseAccessToStudent(int teacherId, int courseId, int studentId)
    {
        GrantCourseAccessResult result = {};
        result.code = GRANT_COURSE_ACCESS_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        Course course = {};
        if (!courseStorage.getById(courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            result.code = GRANT_COURSE_ACCESS_INVALID_COURSE;
            return result;
        }

        if (!teacherManagesStudent(teacher, studentId))
        {
            result.code = GRANT_COURSE_ACCESS_INVALID_STUDENT;
            return result;
        }

        Student student = {};
        if (!studentStorage.getById(studentId, student))
        {
            result.code = GRANT_COURSE_ACCESS_INVALID_STUDENT;
            return result;
        }

        if (containsId(course.enrolledStudentIds, course.enrolledStudentCount, studentId) ||
            containsId(student.enrolledCourseIds, student.enrolledCourseCount, courseId))
        {
            result.code = GRANT_COURSE_ACCESS_ALREADY_GRANTED;
            return result;
        }

        if (course.enrolledStudentCount >= MAX_COURSE_STUDENTS ||
            student.enrolledCourseCount >= MAX_USER_ENROLLED_COURSES)
        {
            result.code = GRANT_COURSE_ACCESS_LIMIT_REACHED;
            return result;
        }

        course.enrolledStudentIds[course.enrolledStudentCount] = studentId;
        ++course.enrolledStudentCount;

        student.enrolledCourseIds[student.enrolledCourseCount] = courseId;
        ++student.enrolledCourseCount;

        courseStorage.update(course);
        updateStudentProgress(student);
        studentStorage.update(student);

        result.code = GRANT_COURSE_ACCESS_OK;
        return result;
    }

    GrantCourseAccessResult CourseService::grantCourseAccessToGroup(int teacherId, int courseId, int groupId)
    {
        GrantCourseAccessResult result = {};
        result.code = GRANT_COURSE_ACCESS_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        Course course = {};
        if (!courseStorage.getById(courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            result.code = GRANT_COURSE_ACCESS_INVALID_COURSE;
            return result;
        }

        Group group = {};
        if (!groupStorage.getById(groupId, group) || group.teacherId != teacherId)
        {
            result.code = GRANT_COURSE_ACCESS_INVALID_GROUP;
            return result;
        }

        if (containsId(course.assignedGroupIds, course.assignedGroupCount, groupId))
        {
            result.code = GRANT_COURSE_ACCESS_ALREADY_GRANTED;
            return result;
        }

        if (course.assignedGroupCount >= MAX_COURSE_GROUPS)
        {
            result.code = GRANT_COURSE_ACCESS_LIMIT_REACHED;
            return result;
        }

        for (int i = 0; i < group.studentCount; ++i)
        {
            GrantCourseAccessResult grantResult = grantCourseAccessToStudent(teacherId, courseId, group.studentIds[i]);
            if (grantResult.code != GRANT_COURSE_ACCESS_OK &&
                grantResult.code != GRANT_COURSE_ACCESS_ALREADY_GRANTED)
            {
                return grantResult;
            }
        }

        courseStorage.getById(courseId, course);
        course.assignedGroupIds[course.assignedGroupCount] = groupId;
        ++course.assignedGroupCount;
        courseStorage.update(course);

        result.code = GRANT_COURSE_ACCESS_OK;
        return result;
    }

    CreateThemeResult CourseService::createTheme(int teacherId, int courseId, const char *themeName)
    {
        CreateThemeResult result = {};
        result.code = CREATE_THEME_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        Course course = {};
        if (!courseStorage.getById(courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            result.code = CREATE_THEME_INVALID_COURSE;
            return result;
        }

        if (isEmptyText(themeName))
        {
            result.code = CREATE_THEME_INVALID_NAME;
            return result;
        }

        if (course.themeCount >= MAX_COURSE_THEMES)
        {
            result.code = CREATE_THEME_LIMIT_REACHED;
            return result;
        }

        Theme theme = {};
        theme.id = themeStorage.getNextId();
        theme.courseId = courseId;
        copyText(theme.title, sizeof(theme.title), themeName);

        if (!themeStorage.add(theme))
        {
            result.code = CREATE_THEME_INVALID_NAME;
            return result;
        }

        course.themeIds[course.themeCount] = theme.id;
        ++course.themeCount;
        courseStorage.update(course);

        result.code = CREATE_THEME_OK;
        result.theme = theme;
        return result;
    }

    CreateLessonResult CourseService::createLesson(int teacherId,
                                                   int themeId,
                                                   const char *lessonTitle,
                                                   const char *material,
                                                   const char *date,
                                                   const char *time,
                                                   const char *homeworkLink)
    {
        CreateLessonResult result = {};
        result.code = CREATE_LESSON_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        Theme theme = {};
        if (!themeStorage.getById(themeId, theme))
        {
            result.code = CREATE_LESSON_INVALID_THEME;
            return result;
        }

        Course course = {};
        if (!courseStorage.getById(theme.courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            return result;
        }

        if (isEmptyText(lessonTitle))
        {
            result.code = CREATE_LESSON_INVALID_TITLE;
            return result;
        }

        if (isEmptyText(material))
        {
            result.code = CREATE_LESSON_INVALID_MATERIAL;
            return result;
        }

        if (theme.lessonCount >= MAX_THEME_LESSONS)
        {
            result.code = CREATE_LESSON_LIMIT_REACHED;
            return result;
        }

        Lesson lesson = {};
        lesson.id = lessonStorage.getNextId();
        lesson.themeId = themeId;
        copyText(lesson.title, sizeof(lesson.title), lessonTitle);
        copyText(lesson.material, sizeof(lesson.material), material);
        copyText(lesson.date, sizeof(lesson.date), date);
        copyText(lesson.time, sizeof(lesson.time), time);
        copyText(lesson.homeworkLink, sizeof(lesson.homeworkLink), homeworkLink);

        if (!lessonStorage.add(lesson))
        {
            result.code = CREATE_LESSON_INVALID_TITLE;
            return result;
        }

        theme.lessonIds[theme.lessonCount] = lesson.id;
        ++theme.lessonCount;
        themeStorage.update(theme);

        ++course.totalLessons;
        courseStorage.update(course);

        result.code = CREATE_LESSON_OK;
        result.lesson = lesson;
        return result;
    }

    CreateTestResult CourseService::createTest(int teacherId,
                                               int courseId,
                                               const char *testTitle,
                                               const Question *questions,
                                               int questionCount)
    {
        CreateTestResult result = {};
        result.code = CREATE_TEST_INVALID_TEACHER;

        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
        {
            return result;
        }

        Course course = {};
        if (!courseStorage.getById(courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            result.code = CREATE_TEST_INVALID_COURSE;
            return result;
        }

        if (isEmptyText(testTitle))
        {
            result.code = CREATE_TEST_INVALID_TITLE;
            return result;
        }

        if (questions == nullptr || questionCount <= 0 || questionCount > MAX_TEST_QUESTIONS)
        {
            result.code = CREATE_TEST_INVALID_QUESTIONS;
            return result;
        }

        if (course.testCount >= MAX_COURSE_TESTS)
        {
            result.code = CREATE_TEST_LIMIT_REACHED;
            return result;
        }

        Test test = {};
        test.id = testStorage.getNextId();
        test.courseId = courseId;
        copyText(test.title, sizeof(test.title), testTitle);

        for (int i = 0; i < questionCount; ++i)
        {
            if (isEmptyText(questions[i].text) ||
                questions[i].optionCount < 2 ||
                questions[i].optionCount > MAX_QUESTION_OPTIONS ||
                questions[i].score <= 0.0 ||
                countCorrectOptions(questions[i]) <= 0)
            {
                result.code = CREATE_TEST_INVALID_QUESTIONS;
                return result;
            }

            Question question = questions[i];
            question.id = questionStorage.getNextId();
            test.maxScore += question.score;

            if (!questionStorage.add(question))
            {
                result.code = CREATE_TEST_INVALID_QUESTIONS;
                return result;
            }

            test.questionIds[test.questionCount] = question.id;
            ++test.questionCount;
        }

        if (!testStorage.add(test))
        {
            deleteQuestionsForTest(test);
            result.code = CREATE_TEST_INVALID_TITLE;
            return result;
        }

        course.testIds[course.testCount] = test.id;
        ++course.testCount;
        courseStorage.update(course);

        result.code = CREATE_TEST_OK;
        result.test = test;
        return result;
    }

    TakeTestResult CourseService::takeTest(int studentId, int testId, const int *answerMasks, int answerCount)
    {
        TakeTestResult result = {};
        result.code = TAKE_TEST_INVALID_STUDENT;

        Student student = {};
        if (!studentStorage.getById(studentId, student))
        {
            return result;
        }

        Test test = {};
        if (!testStorage.getById(testId, test))
        {
            result.code = TAKE_TEST_INVALID_TEST;
            return result;
        }

        if (!containsId(student.enrolledCourseIds, student.enrolledCourseCount, test.courseId))
        {
            result.code = TAKE_TEST_NOT_ENROLLED;
            return result;
        }

        TestResult existingResult{};
        if (testResultStorage_.findByStudentAndTest(studentId, testId, existingResult))
        {
            result.code = TAKE_TEST_ALREADY_PASSED;
            return result;
        }

        if (answerMasks == nullptr || answerCount != test.questionCount)
        {
            result.code = TAKE_TEST_INVALID_ANSWERS;
            return result;
        }

        result.score = 0.0;
        result.maxScore = test.maxScore;

        for (int i = 0; i < test.questionCount; ++i)
        {
            Question question = {};
            if (!questionStorage.getById(test.questionIds[i], question))
            {
                result.code = TAKE_TEST_INVALID_TEST;
                return result;
            }
            result.score += calculateQuestionScore(question, answerMasks[i]);
        }

        result.passed = result.maxScore > 0.0 &&
                        result.score * 100.0 >= result.maxScore * 60.0;
        result.code = TAKE_TEST_OK;

        TestResult tr{};
        tr.id = testResultStorage_.getNextId();
        tr.studentId = studentId;
        tr.testId = testId;
        tr.score = result.score;
        tr.maxScore = result.maxScore;
        tr.passed = result.passed;
        tr.answerCount = answerCount < MAX_TEST_QUESTIONS ? answerCount : MAX_TEST_QUESTIONS;
        for (int i = 0; i < tr.answerCount; ++i)
            tr.answerMasks[i] = answerMasks[i];
        testResultStorage_.add(tr);

        if (result.passed && student.passedTestCount < MAX_USER_PASSED_TESTS)
        {
            student.passedTestIds[student.passedTestCount] = testId;
            student.passedTestScores[student.passedTestCount] = result.score;
            student.passedTestMaxScores[student.passedTestCount] = result.maxScore;
            ++student.passedTestCount;
            studentStorage.update(student);
        }

        return result;
    }

    bool CourseService::getTestResult(int studentId, int testId, TestResult &result) const
    {
        return testResultStorage_.findByStudentAndTest(studentId, testId, result);
    }

    int CourseService::getTestResultsForStudent(int studentId,
                                                TestResult *buf, int maxCount) const
    {
        return testResultStorage_.findByStudent(studentId, buf, maxCount);
    }

    bool CourseService::markLessonCompleted(int studentId, int lessonId)
    {
        Student student = {};
        if (!studentStorage.getById(studentId, student))
        {
            return false;
        }

        Course course = {};
        Theme theme = {};
        Lesson lesson = {};
        if (!getCourseByLessonId(lessonId, course, theme, lesson))
        {
            return false;
        }

        if (!containsId(student.enrolledCourseIds, student.enrolledCourseCount, course.id))
        {
            return false;
        }

        if (containsId(student.completedLessonIds, student.completedLessonCount, lessonId))
        {
            return true;
        }

        if (student.completedLessonCount >= MAX_USER_COMPLETED_LESSONS)
        {
            return false;
        }

        student.completedLessonIds[student.completedLessonCount] = lessonId;
        ++student.completedLessonCount;
        updateStudentProgress(student);
        return studentStorage.update(student);
    }

    bool CourseService::markLessonIncomplete(int studentId, int lessonId)
    {
        Student student = {};
        if (!studentStorage.getById(studentId, student))
        {
            return false;
        }

        if (!containsId(student.completedLessonIds, student.completedLessonCount, lessonId))
        {
            return true;
        }

        removeIdFromList(student.completedLessonIds, student.completedLessonCount, lessonId);
        updateStudentProgress(student);
        return studentStorage.update(student);
    }

    bool CourseService::saveHomeworkSubmission(int studentId, int lessonId,
                                               const char *studentName, const char *homeworkLink)
    {
        if (!homeworkLink || homeworkLink[0] == '\0')
            return false;

        HomeworkSubmission sub{};
        sub.studentId = studentId;
        sub.lessonId = lessonId;
        copyText(sub.studentName, sizeof(sub.studentName), studentName ? studentName : "");
        copyText(sub.homeworkLink, sizeof(sub.homeworkLink), homeworkLink);
        return homeworkStorage.upsert(sub);
    }

    int CourseService::getSubmissionsForLesson(int lessonId, HomeworkSubmission *buf, int maxCount) const
    {
        return homeworkStorage.getByLessonId(lessonId, buf, maxCount);
    }

    bool CourseService::getStudentSubmission(int studentId, int lessonId, HomeworkSubmission &out) const
    {
        return homeworkStorage.findByStudentAndLesson(studentId, lessonId, out);
    }

    bool CourseService::isLessonCompletedByStudent(int studentId, int lessonId) const
    {
        Student student{};
        if (!studentStorage.getById(studentId, student))
            return false;
        return containsId(student.completedLessonIds, student.completedLessonCount, lessonId);
    }

    void CourseService::getStudentName(int studentId, char *buf, int bufSize) const
    {
        Student s{};
        if (studentStorage.getById(studentId, s))
            copyText(buf, bufSize, s.fullName);
        else
            copyText(buf, bufSize, "");
    }

    bool CourseService::updateCourseName(int teacherId, int courseId, const char *courseName)
    {
        Teacher teacher = {};
        Course course = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !courseStorage.getById(courseId, course) ||
            !isTeacherOwnerOfCourse(teacherId, course) ||
            isEmptyText(courseName))
        {
            return false;
        }

        copyText(course.name, sizeof(course.name), courseName);
        return courseStorage.update(course);
    }

    bool CourseService::updateThemeTitle(int teacherId, int themeId, const char *themeName)
    {
        Teacher teacher = {};
        Theme theme = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !themeStorage.getById(themeId, theme) ||
            isEmptyText(themeName))
        {
            return false;
        }

        Course course = {};
        if (!courseStorage.getById(theme.courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        copyText(theme.title, sizeof(theme.title), themeName);
        return themeStorage.update(theme);
    }

    bool CourseService::updateLesson(int teacherId,
                                     int lessonId,
                                     const char *lessonTitle,
                                     const char *material,
                                     const char *date,
                                     const char *time,
                                     const char *homeworkLink)
    {
        Teacher teacher = {};
        Course course = {};
        Theme theme = {};
        Lesson lesson = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !getCourseByLessonId(lessonId, course, theme, lesson) ||
            !isTeacherOwnerOfCourse(teacherId, course) ||
            isEmptyText(lessonTitle))
        {
            return false;
        }

        copyText(lesson.title, sizeof(lesson.title), lessonTitle);
        copyText(lesson.material, sizeof(lesson.material), material);
        copyText(lesson.date, sizeof(lesson.date), date);
        copyText(lesson.time, sizeof(lesson.time), time);
        copyText(lesson.homeworkLink, sizeof(lesson.homeworkLink), homeworkLink);
        return lessonStorage.update(lesson);
    }

    bool CourseService::updateTest(int teacherId,
                                   int testId,
                                   const char *testTitle,
                                   const Question *questions,
                                   int questionCount)
    {
        Teacher teacher = {};
        Course course = {};
        Test test = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !getCourseByTestId(testId, course, test) ||
            !isTeacherOwnerOfCourse(teacherId, course) ||
            isEmptyText(testTitle) ||
            questions == nullptr ||
            questionCount <= 0 ||
            questionCount > MAX_TEST_QUESTIONS)
        {
            return false;
        }

        deleteQuestionsForTest(test);
        test.questionCount = 0;
        test.maxScore = 0.0;
        copyText(test.title, sizeof(test.title), testTitle);

        for (int i = 0; i < questionCount; ++i)
        {
            if (isEmptyText(questions[i].text) ||
                questions[i].optionCount < 2 ||
                questions[i].optionCount > MAX_QUESTION_OPTIONS ||
                questions[i].score <= 0.0 ||
                countCorrectOptions(questions[i]) <= 0)
            {
                return false;
            }

            Question question = questions[i];
            question.id = questionStorage.getNextId();
            test.maxScore += question.score;

            if (!questionStorage.add(question))
            {
                return false;
            }

            test.questionIds[test.questionCount] = question.id;
            ++test.questionCount;
        }

        removeTestFromStudents(course.id, test.id);
        return testStorage.update(test);
    }

    bool CourseService::clearLessonMaterial(int teacherId, int lessonId)
    {
        Teacher teacher = {};
        Course course = {};
        Theme theme = {};
        Lesson lesson = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !getCourseByLessonId(lessonId, course, theme, lesson) ||
            !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        lesson.material[0] = '\0';
        return lessonStorage.update(lesson);
    }

    bool CourseService::clearLessonHomework(int teacherId, int lessonId)
    {
        Teacher teacher = {};
        Course course = {};
        Theme theme = {};
        Lesson lesson = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !getCourseByLessonId(lessonId, course, theme, lesson) ||
            !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        lesson.homeworkLink[0] = '\0';
        return lessonStorage.update(lesson);
    }

    bool CourseService::removeStudentFromTeacher(int teacherId, int studentId)
    {
        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher))
            return false;
        if (!containsId(teacher.managedStudentIds, teacher.managedStudentCount, studentId))
            return false;
        removeIdFromList(teacher.managedStudentIds, teacher.managedStudentCount, studentId);
        return teacherStorage.update(teacher);
    }

    bool CourseService::deleteCourse(int teacherId, int courseId)
    {
        Teacher teacher = {};
        Course course = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !courseStorage.getById(courseId, course) ||
            !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        for (int i = course.themeCount - 1; i >= 0; --i)
        {
            deleteTheme(teacherId, course.themeIds[i]);
        }

        for (int i = course.testCount - 1; i >= 0; --i)
        {
            deleteTest(teacherId, course.testIds[i]);
        }

        removeCourseFromStudents(courseId);
        removeIdFromList(teacher.createdCourseIds, teacher.createdCourseCount, courseId);
        teacherStorage.update(teacher);
        return courseStorage.removeById(courseId);
    }

    bool CourseService::deleteTheme(int teacherId, int themeId)
    {
        Teacher teacher = {};
        Theme theme = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !themeStorage.getById(themeId, theme))
        {
            return false;
        }

        Course course = {};
        if (!courseStorage.getById(theme.courseId, course) || !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        for (int i = theme.lessonCount - 1; i >= 0; --i)
        {
            deleteLesson(teacherId, theme.lessonIds[i]);
        }

        removeIdFromList(course.themeIds, course.themeCount, themeId);
        courseStorage.update(course);
        return themeStorage.removeById(themeId);
    }

    bool CourseService::deleteLesson(int teacherId, int lessonId)
    {
        Teacher teacher = {};
        Course course = {};
        Theme theme = {};
        Lesson lesson = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !getCourseByLessonId(lessonId, course, theme, lesson) ||
            !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        removeIdFromList(theme.lessonIds, theme.lessonCount, lessonId);
        themeStorage.update(theme);

        if (course.totalLessons > 0)
        {
            --course.totalLessons;
        }
        courseStorage.update(course);
        removeLessonFromStudents(course.id, lessonId);
        return lessonStorage.removeById(lessonId);
    }

    bool CourseService::deleteTest(int teacherId, int testId)
    {
        Teacher teacher = {};
        Course course = {};
        Test test = {};
        if (!teacherStorage.getById(teacherId, teacher) ||
            !getCourseByTestId(testId, course, test) ||
            !isTeacherOwnerOfCourse(teacherId, course))
        {
            return false;
        }

        removeIdFromList(course.testIds, course.testCount, testId);
        courseStorage.update(course);
        removeTestFromStudents(course.id, test.id);
        deleteQuestionsForTest(test);
        return testStorage.removeById(testId);
    }

    int CourseService::searchCourses(const char *query, Course *buffer, int maxCount) const
    {
        Course allCourses[MAX_STORAGE_RECORDS] = {};
        const int total = courseStorage.getAll(allCourses, MAX_STORAGE_RECORDS);
        int count = 0;
        for (int i = 0; i < total && count < maxCount; ++i)
        {
            if (containsTextIgnoreCase(allCourses[i].name, query))
            {
                buffer[count++] = allCourses[i];
            }
        }
        return count;
    }

    int CourseService::searchThemes(const char *query, Theme *buffer, int maxCount) const
    {
        Theme allThemes[MAX_STORAGE_RECORDS] = {};
        const int total = themeStorage.getAll(allThemes, MAX_STORAGE_RECORDS);
        int count = 0;
        for (int i = 0; i < total && count < maxCount; ++i)
        {
            if (containsTextIgnoreCase(allThemes[i].title, query))
            {
                buffer[count++] = allThemes[i];
            }
        }
        return count;
    }

    int CourseService::searchLessons(const char *query, Lesson *buffer, int maxCount) const
    {
        Lesson allLessons[MAX_STORAGE_RECORDS] = {};
        const int total = lessonStorage.getAll(allLessons, MAX_STORAGE_RECORDS);
        int count = 0;
        for (int i = 0; i < total && count < maxCount; ++i)
        {
            if (containsTextIgnoreCase(allLessons[i].title, query) ||
                containsTextIgnoreCase(allLessons[i].material, query) ||
                containsTextIgnoreCase(allLessons[i].homeworkLink, query))
            {
                buffer[count++] = allLessons[i];
            }
        }
        return count;
    }

    int CourseService::searchTests(const char *query, Test *buffer, int maxCount) const
    {
        Test allTests[MAX_STORAGE_RECORDS] = {};
        const int total = testStorage.getAll(allTests, MAX_STORAGE_RECORDS);
        int count = 0;
        for (int i = 0; i < total && count < maxCount; ++i)
        {
            if (containsTextIgnoreCase(allTests[i].title, query))
            {
                buffer[count++] = allTests[i];
            }
        }
        return count;
    }

    int CourseService::getTeacherStudents(int teacherId, Student *buffer, int maxCount) const
    {
        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher) || buffer == nullptr || maxCount <= 0)
        {
            return 0;
        }

        int count = 0;
        for (int i = 0; i < teacher.managedStudentCount && count < maxCount; ++i)
        {
            Student student = {};
            if (studentStorage.getById(teacher.managedStudentIds[i], student))
            {
                buffer[count++] = student;
            }
        }
        return count;
    }

    int CourseService::getTeacherGroups(int teacherId, Group *buffer, int maxCount) const
    {
        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher) || buffer == nullptr || maxCount <= 0)
        {
            return 0;
        }

        int count = 0;
        for (int i = 0; i < teacher.groupCount && count < maxCount; ++i)
        {
            Group group = {};
            if (groupStorage.getById(teacher.groupIds[i], group))
            {
                buffer[count++] = group;
            }
        }
        return count;
    }

    int CourseService::getUpcomingScheduleForTeacher(int teacherId, ScheduleItem *buffer, int maxCount) const
    {
        Teacher teacher = {};
        if (!teacherStorage.getById(teacherId, teacher) || buffer == nullptr || maxCount <= 0)
        {
            return 0;
        }

        int count = 0;
        for (int i = 0; i < teacher.createdCourseCount && count < maxCount; ++i)
        {
            Course course = {};
            if (!courseStorage.getById(teacher.createdCourseIds[i], course))
            {
                continue;
            }

            count = collectScheduleForCourse(course, buffer, maxCount, count);
        }

        sortScheduleItems(buffer, count);
        return count;
    }

    int CourseService::getUpcomingScheduleForStudent(int studentId, ScheduleItem *buffer, int maxCount) const
    {
        Student student = {};
        if (!studentStorage.getById(studentId, student) || buffer == nullptr || maxCount <= 0)
        {
            return 0;
        }

        int count = 0;
        for (int i = 0; i < student.enrolledCourseCount && count < maxCount; ++i)
        {
            Course course = {};
            if (!courseStorage.getById(student.enrolledCourseIds[i], course))
            {
                continue;
            }

            count = collectScheduleForCourse(course, buffer, maxCount, count);
        }

        sortScheduleItems(buffer, count);
        return count;
    }

    bool CourseService::getAccountById(int userId, User &user, AccountRole &role) const
    {
        Teacher teacher = {};
        if (teacherStorage.getById(userId, teacher))
        {
            user = static_cast<const User &>(teacher);
            role = ACCOUNT_ROLE_TEACHER;
            return true;
        }

        Student student = {};
        if (studentStorage.getById(userId, student))
        {
            user = static_cast<const User &>(student);
            role = ACCOUNT_ROLE_STUDENT;
            return true;
        }

        role = ACCOUNT_ROLE_NONE;
        return false;
    }

    bool CourseService::getStudentById(int studentId, Student &student) const
    {
        return studentStorage.getById(studentId, student);
    }

    bool CourseService::getTeacherById(int teacherId, Teacher &teacher) const
    {
        return teacherStorage.getById(teacherId, teacher);
    }

    bool CourseService::getGroupById(int groupId, Group &group) const
    {
        return groupStorage.getById(groupId, group);
    }

    bool CourseService::getCourseById(int courseId, Course &course) const
    {
        return courseStorage.getById(courseId, course);
    }

    bool CourseService::getThemeById(int themeId, Theme &theme) const
    {
        return themeStorage.getById(themeId, theme);
    }

    bool CourseService::getLessonById(int lessonId, Lesson &lesson) const
    {
        return lessonStorage.getById(lessonId, lesson);
    }

    bool CourseService::getTestById(int testId, Test &test) const
    {
        return testStorage.getById(testId, test);
    }

    bool CourseService::getQuestionById(int questionId, Question &question) const
    {
        return questionStorage.getById(questionId, question);
    }

    int CourseService::getAllCourses(Course *buffer, int maxCount) const
    {
        return courseStorage.getAll(buffer, maxCount);
    }

    int CourseService::getAllThemes(Theme *buffer, int maxCount) const
    {
        return themeStorage.getAll(buffer, maxCount);
    }

    int CourseService::getAllLessons(Lesson *buffer, int maxCount) const
    {
        return lessonStorage.getAll(buffer, maxCount);
    }

    int CourseService::getAllTests(Test *buffer, int maxCount) const
    {
        return testStorage.getAll(buffer, maxCount);
    }

    bool CourseService::containsId(const int *values, int count, int id) const
    {
        for (int i = 0; i < count; ++i)
        {
            if (values[i] == id)
            {
                return true;
            }
        }
        return false;
    }

    void CourseService::removeIdFromList(int *values, int &count, int id) const
    {
        for (int i = 0; i < count; ++i)
        {
            if (values[i] != id)
            {
                continue;
            }

            for (int j = i; j < count - 1; ++j)
            {
                values[j] = values[j + 1];
            }

            values[count - 1] = 0;
            --count;
            return;
        }
    }

    bool CourseService::isTeacherOwnerOfCourse(int teacherId, const Course &course) const
    {
        return course.creatorTeacherId == teacherId;
    }

    bool CourseService::teacherManagesStudent(const Teacher &teacher, int studentId) const
    {
        return containsId(teacher.managedStudentIds, teacher.managedStudentCount, studentId);
    }

    void CourseService::updateStudentProgress(Student &student)
    {
        int totalAvailableLessons = 0;
        int completedLessons = 0;

        for (int i = 0; i < student.enrolledCourseCount; ++i)
        {
            Course course = {};
            if (!courseStorage.getById(student.enrolledCourseIds[i], course))
            {
                continue;
            }

            totalAvailableLessons += course.totalLessons;
        }

        for (int i = 0; i < student.completedLessonCount; ++i)
        {
            Lesson lesson = {};
            if (!lessonStorage.getById(student.completedLessonIds[i], lesson))
            {
                continue;
            }

            Theme theme = {};
            if (!themeStorage.getById(lesson.themeId, theme))
            {
                continue;
            }

            if (containsId(student.enrolledCourseIds, student.enrolledCourseCount, theme.courseId))
            {
                ++completedLessons;
            }
        }

        if (totalAvailableLessons <= 0)
        {
            student.totalProgress = 0.0;
            return;
        }

        student.totalProgress = static_cast<double>(completedLessons) * 100.0 /
                                static_cast<double>(totalAvailableLessons);
    }

    bool CourseService::getCourseByLessonId(int lessonId, Course &course, Theme &theme, Lesson &lesson) const
    {
        if (!lessonStorage.getById(lessonId, lesson))
        {
            return false;
        }

        if (!themeStorage.getById(lesson.themeId, theme))
        {
            return false;
        }

        return courseStorage.getById(theme.courseId, course);
    }

    bool CourseService::getCourseByTestId(int testId, Course &course, Test &test) const
    {
        if (!testStorage.getById(testId, test))
        {
            return false;
        }

        return courseStorage.getById(test.courseId, course);
    }

    void CourseService::removeLessonFromStudents(int courseId, int lessonId)
    {
        Student students[MAX_STORAGE_RECORDS] = {};
        const int count = studentStorage.getAll(students, MAX_STORAGE_RECORDS);

        for (int i = 0; i < count; ++i)
        {
            if (!containsId(students[i].enrolledCourseIds, students[i].enrolledCourseCount, courseId) ||
                !containsId(students[i].completedLessonIds, students[i].completedLessonCount, lessonId))
            {
                continue;
            }

            removeIdFromList(students[i].completedLessonIds, students[i].completedLessonCount, lessonId);
            updateStudentProgress(students[i]);
            studentStorage.update(students[i]);
        }
    }

    void CourseService::removeTestFromStudents(int courseId, int testId)
    {
        Student students[MAX_STORAGE_RECORDS] = {};
        const int count = studentStorage.getAll(students, MAX_STORAGE_RECORDS);

        for (int i = 0; i < count; ++i)
        {
            if (!containsId(students[i].enrolledCourseIds, students[i].enrolledCourseCount, courseId) ||
                !containsId(students[i].passedTestIds, students[i].passedTestCount, testId))
            {
                continue;
            }

            for (int index = 0; index < students[i].passedTestCount; ++index)
            {
                if (students[i].passedTestIds[index] != testId)
                {
                    continue;
                }

                for (int move = index; move < students[i].passedTestCount - 1; ++move)
                {
                    students[i].passedTestIds[move] = students[i].passedTestIds[move + 1];
                    students[i].passedTestScores[move] = students[i].passedTestScores[move + 1];
                    students[i].passedTestMaxScores[move] = students[i].passedTestMaxScores[move + 1];
                }

                --students[i].passedTestCount;
                break;
            }

            studentStorage.update(students[i]);
        }
    }

    void CourseService::removeCourseFromStudents(int courseId)
    {
        Student students[MAX_STORAGE_RECORDS] = {};
        const int count = studentStorage.getAll(students, MAX_STORAGE_RECORDS);

        for (int i = 0; i < count; ++i)
        {
            if (!containsId(students[i].enrolledCourseIds, students[i].enrolledCourseCount, courseId))
            {
                continue;
            }

            removeIdFromList(students[i].enrolledCourseIds, students[i].enrolledCourseCount, courseId);
            updateStudentProgress(students[i]);
            studentStorage.update(students[i]);
        }
    }

    void CourseService::deleteQuestionsForTest(const Test &test)
    {
        for (int i = 0; i < test.questionCount; ++i)
        {
            questionStorage.removeById(test.questionIds[i]);
        }
    }

    double CourseService::calculateQuestionScore(const Question &question, int selectedMask) const
    {
        const int correctCount = countCorrectOptions(question);
        if (correctCount <= 0 || question.score <= 0.0)
        {
            return 0.0;
        }

        const double scorePerCorrect = question.score / static_cast<double>(correctCount);
        double score = 0.0;

        for (int optionIndex = 0; optionIndex < question.optionCount; ++optionIndex)
        {
            const int bit = 1 << optionIndex;
            if ((selectedMask & bit) != 0 && question.correctOptionFlags[optionIndex] == 1)
            {
                score += scorePerCorrect;
            }
        }

        return score > question.score ? question.score : score;
    }

    int CourseService::countCorrectOptions(const Question &question) const
    {
        int count = 0;
        for (int optionIndex = 0; optionIndex < question.optionCount; ++optionIndex)
        {
            if (question.correctOptionFlags[optionIndex] == 1)
            {
                ++count;
            }
        }
        return count;
    }

    long long CourseService::makeDateTimeKey(const char *date, const char *time) const
    {
        if (isEmptyText(date))
        {
            return 0;
        }

        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;

        if (std::strlen(date) >= 10 && date[4] == '-')
        {
            year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + (date[3] - '0');
            month = (date[5] - '0') * 10 + (date[6] - '0');
            day = (date[8] - '0') * 10 + (date[9] - '0');
        }
        else if (std::strlen(date) >= 10 && date[2] == '.')
        {
            day = (date[0] - '0') * 10 + (date[1] - '0');
            month = (date[3] - '0') * 10 + (date[4] - '0');
            year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
        }

        if (!isEmptyText(time) && std::strlen(time) >= 5)
        {
            hour = (time[0] - '0') * 10 + (time[1] - '0');
            minute = (time[3] - '0') * 10 + (time[4] - '0');
        }

        return static_cast<long long>(year) * 100000000LL +
               static_cast<long long>(month) * 1000000LL +
               static_cast<long long>(day) * 10000LL +
               static_cast<long long>(hour) * 100LL +
               static_cast<long long>(minute);
    }

    long long CourseService::getNowKey() const
    {
        std::time_t now = std::time(nullptr);
        std::tm localTime = {};
#if defined(_WIN32)
        localtime_s(&localTime, &now);
#else
        localTime = *std::localtime(&now);
#endif
        return static_cast<long long>(localTime.tm_year + 1900) * 100000000LL +
               static_cast<long long>(localTime.tm_mon + 1) * 1000000LL +
               static_cast<long long>(localTime.tm_mday) * 10000LL +
               static_cast<long long>(localTime.tm_hour) * 100LL +
               static_cast<long long>(localTime.tm_min);
    }

    int CourseService::collectScheduleForCourse(const Course &course, ScheduleItem *buffer, int maxCount, int count) const
    {
        const long long nowKey = getNowKey();

        for (int themeIndex = 0; themeIndex < course.themeCount && count < maxCount; ++themeIndex)
        {
            Theme theme = {};
            if (!themeStorage.getById(course.themeIds[themeIndex], theme))
            {
                continue;
            }

            for (int lessonIndex = 0; lessonIndex < theme.lessonCount && count < maxCount; ++lessonIndex)
            {
                Lesson lesson = {};
                if (!lessonStorage.getById(theme.lessonIds[lessonIndex], lesson))
                {
                    continue;
                }

                const long long lessonKey = makeDateTimeKey(lesson.date, lesson.time);
                if (lessonKey == 0 || lessonKey < nowKey)
                {
                    continue;
                }

                buffer[count].lessonId = lesson.id;
                buffer[count].courseId = course.id;
                copyText(buffer[count].courseName, sizeof(buffer[count].courseName), course.name);
                copyText(buffer[count].lessonTitle, sizeof(buffer[count].lessonTitle), lesson.title);
                copyText(buffer[count].date, sizeof(buffer[count].date), lesson.date);
                copyText(buffer[count].time, sizeof(buffer[count].time), lesson.time);
                ++count;
            }
        }

        return count;
    }

    void CourseService::sortScheduleItems(ScheduleItem *items, int count) const
    {
        for (int i = 0; i < count; ++i)
        {
            for (int j = i + 1; j < count; ++j)
            {
                if (makeDateTimeKey(items[j].date, items[j].time) < makeDateTimeKey(items[i].date, items[i].time))
                {
                    const ScheduleItem temp = items[i];
                    items[i] = items[j];
                    items[j] = temp;
                }
            }
        }
    }
}
