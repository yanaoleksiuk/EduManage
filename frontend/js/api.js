const API_BASE = "http://localhost:8080";

async function apiCall(method, path, body = null) {
  const opts = {
    method,
    headers: { "Content-Type": "application/json" },
  };
  if (body !== null) opts.body = JSON.stringify(body);

  const res = await fetch(API_BASE + path, opts);
  const data = await res.json();

  if (!res.ok || data.success === false) {
    throw new Error(data.message || `HTTP ${res.status}`);
  }
  return data;
}

const api = {
  login(email, password) {
    return apiCall("POST", "/api/auth/login", { email, password });
  },
  register(role, firstName, lastName, email, password) {
    const fullName = `${firstName} ${lastName}`.trim();
    return apiCall("POST", "/api/auth/register", {
      role,
      fullName,
      email,
      password,
    });
  },

  getUser(id) {
    return apiCall("GET", `/api/users/${id}`);
  },
  updateUser(id, data) {
    return apiCall("PUT", `/api/users/${id}`, data);
  },
  deleteUser(id) {
    return apiCall("DELETE", `/api/users/${id}`);
  },

  getCourses(params = {}) {
    const qs = new URLSearchParams(params).toString();
    return apiCall("GET", `/api/courses/search${qs ? "?" + qs : ""}`);
  },
  getCourse(id) {
    return apiCall("GET", `/api/courses/${id}`);
  },
  createCourse(teacherId, name) {
    return apiCall("POST", `/api/teachers/${teacherId}/courses`, { name });
  },
  updateCourse(id, name) {
    return apiCall("PUT", `/api/courses/${id}`, { name });
  },
  deleteCourse(id) {
    return apiCall("DELETE", `/api/courses/${id}`);
  },

  updateTheme(teacherId, themeId, title) {
    return apiCall("PUT", `/api/teachers/${teacherId}/themes/${themeId}`, {
      title,
    });
  },
  deleteTheme(teacherId, themeId) {
    return apiCall("DELETE", `/api/teachers/${teacherId}/themes/${themeId}`);
  },

  getThemes(courseId) {
    return apiCall("GET", `/api/courses/${courseId}/themes`);
  },
  createTheme(teacherId, courseId, title) {
    return apiCall(
      "POST",
      `/api/teachers/${teacherId}/courses/${courseId}/themes`,
      { title },
    );
  },

  getLessons(themeId, studentId = null) {
    const qs = studentId ? `?studentId=${studentId}` : "";
    return apiCall("GET", `/api/themes/${themeId}/lessons${qs}`);
  },
  getLesson(id, studentId = null) {
    const qs = studentId ? `?studentId=${studentId}` : "";
    return apiCall("GET", `/api/lessons/${id}${qs}`);
  },
  createLesson(teacherId, themeId, title, material, date, time, homeworkLink) {
    return apiCall(
      "POST",
      `/api/teachers/${teacherId}/themes/${themeId}/lessons`,
      {
        title,
        material,
        date,
        time,
        homeworkLink,
      },
    );
  },
  updateLesson(teacherId, lessonId, data) {
    return apiCall(
      "PUT",
      `/api/teachers/${teacherId}/lessons/${lessonId}`,
      data,
    );
  },
  deleteLesson(teacherId, lessonId) {
    return apiCall("DELETE", `/api/teachers/${teacherId}/lessons/${lessonId}`);
  },
  completeLesson(studentId, lessonId, homeworkLink = null) {
    return apiCall(
      "POST",
      `/api/students/${studentId}/lessons/${lessonId}/complete`,
      homeworkLink ? { homeworkLink } : null,
    );
  },
  getLessonSubmissions(teacherId, lessonId) {
    return apiCall(
      "GET",
      `/api/teachers/${teacherId}/lessons/${lessonId}/submissions`,
    );
  },

  getTests(courseId, studentId = null) {
    const qs = studentId ? `?studentId=${studentId}` : "";
    return apiCall("GET", `/api/courses/${courseId}/tests${qs}`);
  },
  getTest(id) {
    return apiCall("GET", `/api/tests/${id}`);
  },
  getTestResult(studentId, testId) {
    return apiCall("GET", `/api/students/${studentId}/tests/${testId}/result`);
  },
  getStudentTestResults(studentId) {
    return apiCall("GET", `/api/students/${studentId}/test-results`);
  },
  createTest(teacherId, courseId, title, questions) {
    return apiCall(
      "POST",
      `/api/teachers/${teacherId}/courses/${courseId}/tests`,
      { title, questions },
    );
  },
  updateTest(teacherId, testId, title, questions) {
    return apiCall("PUT", `/api/teachers/${teacherId}/tests/${testId}`, {
      title,
      questions,
    });
  },
  deleteTest(teacherId, testId) {
    return apiCall("DELETE", `/api/teachers/${teacherId}/tests/${testId}`);
  },
  submitTest(studentId, testId, answers) {
    return apiCall(
      "POST",
      `/api/students/${studentId}/tests/${testId}/submit`,
      { answers },
    );
  },

  getTeacherStudents(teacherId) {
    return apiCall("GET", `/api/teachers/${teacherId}/students`);
  },
  addStudentToTeacher(teacherId, studentEmail) {
    return apiCall("POST", `/api/teachers/${teacherId}/students`, {
      studentEmail,
    });
  },
  removeStudentFromTeacher(teacherId, studentId) {
    return apiCall(
      "DELETE",
      `/api/teachers/${teacherId}/students/${studentId}`,
    );
  },
  getGroups(teacherId) {
    return apiCall("GET", `/api/teachers/${teacherId}/groups`);
  },
  createGroup(teacherId, name, studentIds) {
    return apiCall("POST", `/api/teachers/${teacherId}/groups`, {
      name,
      studentIds,
    });
  },
  deleteGroup(teacherId, groupId) {
    return apiCall("DELETE", `/api/teachers/${teacherId}/groups/${groupId}`);
  },

  getEnrollOptions(teacherId, courseId) {
    return apiCall(
      "GET",
      `/api/teachers/${teacherId}/courses/${courseId}/enroll-options`,
    );
  },
  enrollStudent(teacherId, courseId, studentId) {
    return apiCall(
      "POST",
      `/api/teachers/${teacherId}/courses/${courseId}/enroll`,
      { studentId },
    );
  },
  enrollStudentByEmail(teacherId, courseId, studentEmail) {
    return apiCall(
      "POST",
      `/api/teachers/${teacherId}/courses/${courseId}/enroll`,
      { studentEmail },
    );
  },
  enrollGroup(teacherId, courseId, groupId) {
    return apiCall(
      "POST",
      `/api/teachers/${teacherId}/courses/${courseId}/enroll-group`,
      { groupId },
    );
  },

  getProgress(studentId) {
    return apiCall("GET", `/api/students/${studentId}/progress`);
  },
  getStudentSchedule(id) {
    return apiCall("GET", `/api/students/${id}/schedule`);
  },
  getTeacherSchedule(id) {
    return apiCall("GET", `/api/teachers/${id}/schedule`);
  },
};
