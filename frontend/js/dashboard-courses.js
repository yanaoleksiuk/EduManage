async function renderCourses() {
  const el = document.getElementById("main-content");
  const u = App.user;

  let courses = [];
  try {
    if (isTeacher()) {
      const data = await api.getCourses({ teacherId: u.id });
      courses = data.courses || [];
    } else {
      const data = await api.getCourses({ studentId: u.id });
      courses = data.courses || [];
    }
  } catch {
    courses = [];
  }

  Store.allCourses = courses;

  const addBtn = isTeacher()
    ? `<button class="btn btn-primary" onclick="showCreateCourseModal()">${Icon.plus} Додати курс</button>`
    : "";

  el.innerHTML = `
    <div class="page-header">
      <h1 class="page-title">Курси та заняття</h1>
      <div style="display:flex;gap:10px">${addBtn}</div>
    </div>
    <div class="search-bar-wrap">
      <input class="form-input" id="course-search"
             placeholder="Пошук курсів…"
             oninput="filterCoursesUI(this.value)">
    </div>
    <div id="courses-list">
      ${buildCoursesHtml(courses)}
    </div>
  `;
}

function buildCoursesHtml(courses) {
  if (courses.length === 0) {
    return `<div class="empty-state"><div class="empty-icon">${Icon.courses}</div>
      <p>${isTeacher() ? "Ви ще не створили жодного курсу." : "Вас ще не записали на жоден курс."}</p></div>`;
  }
  return `<div class="accordion-list">${courses.map((c) => courseAccordion(c)).join("")}</div>`;
}

function filterCoursesUI(query) {
  const q = query.trim().toLowerCase();
  const courses = Store.allCourses || [];
  const filtered = q
    ? courses.filter((c) => c.name.toLowerCase().includes(q))
    : courses;

  const list = document.getElementById("courses-list");
  if (list) list.innerHTML = buildCoursesHtml(filtered);
}

function courseAccordion(c) {
  const badge = isStudent()
    ? `<span class="badge badge-primary">відкрито для вас</span>`
    : "";

  const teacherActions = isTeacher()
    ? `
    <button class="btn btn-sm btn-outline"
      onclick="event.stopPropagation();navigate('course',{courseId:${c.id},_label:'${esc(c.name)}'},'Курси та заняття')">
      Відкрити
    </button>
    <button class="btn btn-sm btn-ghost btn-icon" title="Видалити курс"
      onclick="event.stopPropagation();deleteCourse(${c.id})">
      ${Icon.trash}
    </button>
  `
    : "";

  return `
    <div class="accordion-item" id="acc-${c.id}">
      <div class="accordion-header" onclick="toggleAccordion(${c.id})">
        <div class="acc-left">
          <span class="acc-name">${esc(c.name)}</span>
        </div>
        <div class="acc-right">
          ${badge}
          ${teacherActions}
          <span class="chevron">${Icon.chevron}</span>
        </div>
      </div>
      <div class="accordion-body" id="acc-body-${c.id}">
        <div class="loading-wrap"><div class="spinner"></div></div>
      </div>
    </div>
  `;
}

async function toggleAccordion(courseId) {
  const item = document.getElementById(`acc-${courseId}`);
  const body = document.getElementById(`acc-body-${courseId}`);

  if (item.classList.contains("open")) {
    item.classList.remove("open");
    return;
  }
  item.classList.add("open");

  try {
    const thData = await api.getThemes(courseId);
    const themes = thData.themes || [];
    if (!Store.themeMap) Store.themeMap = {};
    themes.forEach((t) => {
      Store.themeMap[t.id] = t;
    });

    if (themes.length === 0) {
      body.innerHTML = `<div class="empty-state" style="padding:24px"><p>Тем ще немає.</p></div>`;
      return;
    }

    const lessonMap = {};
    const studentId = isStudent() ? App.user.id : null;
    await Promise.all(
      themes.map(async (t) => {
        try {
          const ld = await api.getLessons(t.id, studentId);
          lessonMap[t.id] = ld.lessons || [];
        } catch {
          lessonMap[t.id] = [];
        }
      }),
    );

    let tests = [];
    try {
      const td = await api.getTests(courseId, isStudent() ? App.user.id : null);
      tests = td.tests || [];
    } catch {}

    body.innerHTML = renderThemesBlock(courseId, themes, lessonMap, tests);
  } catch (err) {
    body.innerHTML = `<div class="empty-state" style="padding:18px"><p>${esc(err.message)}</p></div>`;
  }
}

function renderThemesBlock(courseId, themes, lessonMap, tests) {
  const addThemeBtn = isTeacher()
    ? `<div style="padding:10px 22px;border-top:1px solid var(--border)">
         <button class="btn btn-sm btn-outline" onclick="showCreateThemeModal(${courseId})">${Icon.plus} Додати тему</button>
       </div>`
    : "";

  const addTestBtn = isTeacher()
    ? `<button class="btn btn-sm btn-outline" style="margin-left:8px" onclick="showCreateTestModal(${courseId})">${Icon.plus} Тест</button>`
    : "";

  let html = themes
    .map((t) => {
      const lessons = lessonMap[t.id] || [];
      const addLessonBtn = isTeacher()
        ? `<button class="btn btn-sm btn-ghost" onclick="showCreateLessonModal(${t.id},${courseId})">${Icon.plus} Урок</button>`
        : "";
      const editThemeBtn = isTeacher()
        ? `<button class="btn btn-sm btn-ghost btn-icon" title="Редагувати тему"
             onclick="showEditThemeModal(${courseId},${t.id})">${Icon.edit}</button>
           <button class="btn btn-sm btn-ghost btn-icon" title="Видалити тему"
             onclick="deleteTheme(${App.user.id},${t.id})">${Icon.trash}</button>`
        : "";

      const lessonsHtml =
        lessons.length === 0
          ? `<div style="padding:10px 38px;font-size:.82rem;color:var(--text-muted)">Уроків ще немає.</div>`
          : lessons.map((l) => renderLessonRow(l, courseId, t.id)).join("");

      return `
      <div class="theme-block">
        <div class="theme-row">
          <div class="theme-row-left">
            <span class="theme-title-txt">${esc(t.title)}</span>
            <span style="font-size:.76rem;color:var(--text-muted)">${t.lessonCount || 0} урок(ів)</span>
          </div>
          <div class="theme-row-right">${editThemeBtn}${addLessonBtn}</div>
        </div>
        ${lessonsHtml}
      </div>
    `;
    })
    .join("");

  let testsHtml;

  if (tests.length > 0) {
    testsHtml = `
    <div class="theme-block">
      <div class="theme-row">
        <div class="theme-row-left">
          <span class="theme-title-txt">Тести</span>
        </div>
        <div class="theme-row-right">${addTestBtn}</div>
      </div>
      ${tests.map((t) => renderTestRow(t, courseId)).join("")}
    </div>
  `;
  } else if (isTeacher()) {
    testsHtml = `
    <div class="theme-block">
      <div class="theme-row">
        <div class="theme-row-left"><span class="theme-title-txt">Тести</span></div>
        <div class="theme-row-right">${addTestBtn}</div>
      </div>
    </div>
  `;
  } else {
    testsHtml = "";
  }

  return html + testsHtml + addThemeBtn;
}

function renderLessonRow(l, courseId, themeId) {
  const date = l.date ? ` · ${esc(l.date)}` : "";
  const time = l.time ? ` ${esc(l.time)}` : "";

  const actions = isTeacher()
    ? `<button class="btn btn-sm btn-ghost btn-icon" title="Редагувати урок"
         onclick="event.stopPropagation();showEditLessonModal(${App.user.id},${l.id})">${Icon.edit}</button>
       <button class="btn btn-sm btn-ghost btn-icon" title="Видалити урок"
         onclick="event.stopPropagation();deleteLesson(${App.user.id},${l.id},${courseId})">${Icon.trash}</button>`
    : isStudent() && l.completed
      ? `<span class="badge badge-success lesson-done-badge">${Icon.check} Пройдено</span>`
      : "";

  return `
    <div class="lesson-row row-clickable" onclick="showLessonModal(${l.id})">
      <div class="lesson-info">
        <span class="lesson-title-txt">${esc(l.title)}</span>
        <span class="lesson-meta">${date}${time}</span>
      </div>
      <div class="lesson-acts">${actions}</div>
    </div>
  `;
}

function renderTestRow(t, courseId) {
  if (isStudent()) {
    if (t.taken) {
      const scoreDisp = Number.isInteger(t.score)
        ? t.score
        : parseFloat((t.score || 0).toFixed(2));
      const badgeCls = t.passed ? "badge-success" : "badge-danger";
      const badgeTxt = t.passed ? "Здано" : "Не здано";
      return `
        <div class="lesson-row row-clickable"
             onclick="navigate('test-result',{testId:${t.id},courseId:${courseId},_label:'${esc(t.title)}'},'Курси та заняття')">
          <div class="lesson-info">
            <span class="lesson-title-txt">${esc(t.title)}</span>
            <span class="lesson-meta">
              ${scoreDisp} / ${t.maxScore || 0} балів
            </span>
          </div>
          <div class="lesson-acts">
            <span class="badge ${badgeCls}" style="font-size:.72rem">${badgeTxt}</span>
            <span style="color:var(--text-muted);font-size:.76rem;margin-left:6px">результат →</span>
          </div>
        </div>`;
    }
    return `
      <div class="lesson-row row-clickable"
           onclick="navigate('test',{testId:${t.id},courseId:${courseId},_label:'${esc(t.title)}'},'Курси та заняття')">
        <div class="lesson-info">
          <span class="lesson-title-txt">${esc(t.title)}</span>
          <span class="lesson-meta">Макс. балів: ${t.maxScore || 0}</span>
        </div>
        <div class="lesson-acts">
          <span style="color:var(--primary);font-size:.76rem">пройти →</span>
        </div>
      </div>`;
  }

  return `
    <div class="lesson-row row-clickable" onclick="showTestDetailModal(${t.id})">
      <div class="lesson-info">
        <span class="lesson-title-txt">${esc(t.title)}</span>
        <span class="lesson-meta">Макс. балів: ${t.maxScore || 0}</span>
      </div>
      <div class="lesson-acts">
        <button class="btn btn-sm btn-ghost btn-icon" title="Редагувати тест"
          onclick="event.stopPropagation();showEditTestModal(${t.id})">${Icon.edit}</button>
        <button class="btn btn-sm btn-ghost btn-icon" title="Видалити тест"
          onclick="event.stopPropagation();deleteTest(${App.user.id},${t.id},${courseId})">${Icon.trash}</button>
      </div>
    </div>`;
}

async function renderCourseDetail() {
  const el = document.getElementById("main-content");
  const courseId = App.params.courseId;

  const [cData, thData, testData] = await Promise.all([
    api.getCourse(courseId),
    api.getThemes(courseId),
    api.getTests(courseId).catch(() => ({ tests: [] })),
  ]);

  const course = cData.course || cData;
  const themes = thData.themes || [];
  const tests = testData.tests || [];
  if (!Store.themeMap) Store.themeMap = {};
  themes.forEach((t) => {
    Store.themeMap[t.id] = t;
  });

  const lessonMap = {};
  await Promise.all(
    themes.map(async (t) => {
      try {
        const ld = await api.getLessons(t.id);
        lessonMap[t.id] = ld.lessons || [];
      } catch {
        lessonMap[t.id] = [];
      }
    }),
  );

  el.innerHTML = `
    <div class="page-header">
      <h1 class="page-title">${esc(course.name)}</h1>
      <div style="display:flex;gap:10px">
        <button class="btn btn-outline" onclick="showEnrollModal(${courseId})">${Icon.plus} Записати</button>
        <button class="btn btn-primary" onclick="showCreateThemeModal(${courseId})">${Icon.plus} Тема</button>
        <button class="btn btn-primary" onclick="showCreateTestModal(${courseId})">${Icon.plus} Тест</button>
      </div>
    </div>
    <div class="accordion-list">
      <div class="accordion-item open">
        <div class="accordion-body" style="display:block;border-top:none">
          ${renderThemesBlock(courseId, themes, lessonMap, tests)}
        </div>
      </div>
    </div>
  `;
}
