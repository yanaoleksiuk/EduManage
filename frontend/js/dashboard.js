
const Store = {};


const App = {
  user: null,
  section: 'courses',
  params: {},
  stack:  [],  
  calYear:  new Date().getFullYear(),
  calMonth: new Date().getMonth(),
};


const Icon = {
  book:     `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M4 19.5A2.5 2.5 0 0 1 6.5 17H20"/><path d="M6.5 2H20v20H6.5A2.5 2.5 0 0 1 4 19.5v-15A2.5 2.5 0 0 1 6.5 2z"/></svg>`,
  user:     `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"/><circle cx="12" cy="7" r="4"/></svg>`,
  courses:  `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2 3h6a4 4 0 0 1 4 4v14a3 3 0 0 0-3-3H2z"/><path d="M22 3h-6a4 4 0 0 0-4 4v14a3 3 0 0 1 3-3h7z"/></svg>`,
  schedule: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"/><line x1="16" y1="2" x2="16" y2="6"/><line x1="8" y1="2" x2="8" y2="6"/><line x1="3" y1="10" x2="21" y2="10"/></svg>`,
  students: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87"/><path d="M16 3.13a4 4 0 0 1 0 7.75"/></svg>`,
  trophy:   `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="8 21 12 17 16 21"/><line x1="12" y1="17" x2="12" y2="11"/><path d="M7 4H4a2 2 0 0 0-2 2v1c0 4 3 7 6 8"/><path d="M17 4h3a2 2 0 0 1 2 2v1c0 4-3 7-6 8"/><path d="M8 4h8a4 4 0 0 1 0 8H8a4 4 0 0 1 0-8z"/></svg>`,
  logout:   `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"/><polyline points="16 17 21 12 16 7"/><line x1="21" y1="12" x2="9" y2="12"/></svg>`,
  plus:     `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg>`,
  chevron:  `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="6 9 12 15 18 9"/></svg>`,
  arrowL:   `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><line x1="19" y1="12" x2="5" y2="12"/><polyline points="12 19 5 12 12 5"/></svg>`,
  trash:    `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="3 6 5 6 21 6"/><path d="M19 6l-1 14H6L5 6"/><path d="M10 11v6"/><path d="M14 11v6"/><path d="M9 6V4h6v2"/></svg>`,
  edit:     `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg>`,
  check:    `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"/></svg>`,
};


function esc(s) {
  if (s == null) return '';
  return String(s)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;');
}

function isTeacher() { return App.user && App.user.role === 'teacher'; }
function isStudent() { return App.user && App.user.role === 'student'; }

function initials(user) {
  return ((user.firstName || '?')[0] + (user.lastName || '?')[0]).toUpperCase();
}

// Parses "DD.MM.YYYY", "DD.MM.YY", or "YYYY-MM-DD" → Date
function parseScheduleDate(dateStr) {
  if (!dateStr) return null;
  if (/^\d{4}-\d{2}-\d{2}/.test(dateStr)) return new Date(dateStr.substring(0, 10) + 'T00:00:00');
  const p = dateStr.split('.');
  if (p.length !== 3) return null;
  const year = p[2].length === 2 ? 2000 + parseInt(p[2]) : parseInt(p[2]);
  const d = new Date(year, parseInt(p[1]) - 1, parseInt(p[0]));
  return isNaN(d.getTime()) ? null : d;
}

function toast(msg, type = 'info') {
  const el = document.createElement('div');
  el.className = `toast toast-${type}`;
  const ico = { success: '✓', error: '✕', info: 'ℹ' };
  el.innerHTML = `<span>${ico[type] || ''}</span><span>${esc(msg)}</span>`;
  document.getElementById('toast-container').appendChild(el);
  setTimeout(() => {
    el.style.opacity = '0';
    setTimeout(() => el.remove(), 320);
  }, 3600);
}

// ── Modal helpers ─────────────────────────────────────
function openModal(id) {
  const el = document.getElementById(id);
  if (el) el.classList.add('open');
}
function closeModal(id) {
  const el = document.getElementById(id);
  if (el) el.classList.remove('open');
}

// ── Navigation ─────────────────────────────────────────
function navigate(section, params = {}, label = null) {
  const crumbLabel = label || sectionLabel(App.section, App.params);
  App.stack.push({ section: App.section, params: App.params, label: crumbLabel });
  App.section = section;
  App.params  = params;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function goBack() {
  if (App.stack.length === 0) return;
  const prev = App.stack.pop();
  App.section = prev.section;
  App.params  = prev.params;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function navRoot(section) {
  App.stack = [];
  App.section = section;
  App.params  = {};
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function sectionLabel(section) {
  const map = {
    courses: 'Курси та заняття',
    course: 'Курс',
    schedule: 'Розклад',
    students: 'Студенти та групи',
    achievements: 'Мої досягнення',
    account: 'Мій акаунт',
    test: 'Тест',
  };
  return map[section] || section;
}

function renderBreadcrumbs() {
  const bc   = document.getElementById('breadcrumbs');
  const back = document.getElementById('back-btn');

  if (App.stack.length === 0) {
    bc.innerHTML = `<span class="crumb current">${esc(sectionLabel(App.section))}</span>`;
    back.classList.add('hidden');
    return;
  }

  back.classList.remove('hidden');
  let html = '';
  App.stack.forEach((item, idx) => {
    html += `<span class="crumb" onclick="jumpTo(${idx})">${esc(item.label || sectionLabel(item.section))}</span>`;
    html += `<span class="bc-sep">/</span>`;
  });
  html += `<span class="crumb current">${esc(App.params._label || sectionLabel(App.section))}</span>`;
  bc.innerHTML = html;
}

function jumpTo(idx) {
  const target = App.stack[idx];
  App.stack    = App.stack.slice(0, idx);
  App.section  = target.section;
  App.params   = target.params;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function syncSidebarActive() {
  document.querySelectorAll('.nav-item[data-section]').forEach(el => {
    el.classList.toggle('active', el.dataset.section === App.section ||
      (App.stack.length > 0 && App.stack[0].section === el.dataset.section));
  });
}

// ── Sidebar ────────────────────────────────────────────
function renderSidebar() {
  const u = App.user;
  const items = isTeacher()
    ? [
        { section: 'account',  icon: Icon.user,     label: 'Мій акаунт' },
        { section: 'courses',  icon: Icon.courses,  label: 'Курси' },
        { section: 'schedule', icon: Icon.schedule, label: 'Розклад' },
        { section: 'students', icon: Icon.students, label: 'Студенти та групи' },
      ]
    : [
        { section: 'account',      icon: Icon.user,     label: 'Мій акаунт' },
        { section: 'courses',      icon: Icon.courses,  label: 'Курси' },
        { section: 'schedule',     icon: Icon.schedule, label: 'Розклад' },
        { section: 'achievements', icon: Icon.trophy,   label: 'Мої досягнення' },
      ];

  const nav = document.getElementById('sidebar-nav');
  nav.innerHTML = items.map(it => `
    <div class="nav-item${App.section === it.section ? ' active' : ''}" data-section="${it.section}"
         onclick="navRoot('${it.section}')">
      ${it.icon}
      <span>${it.label}</span>
    </div>
  `).join('');

  document.getElementById('sidebar-user-name').textContent = `${u.firstName} ${u.lastName}`;
  document.getElementById('sidebar-user-role').textContent = isTeacher() ? 'Викладач' : 'Студент';
  document.getElementById('sidebar-avatar').textContent    = initials(u);
}

// ── Content ────────────────────────────────────────────
async function renderContent() {
  const el = document.getElementById('main-content');
  el.innerHTML = '<div class="loading-wrap"><div class="spinner"></div></div>';

  try {
    switch (App.section) {
      case 'courses':      await renderCourses();      break;
      case 'course':       await renderCourseDetail(); break;
      case 'schedule':     await renderSchedule();     break;
      case 'students':     await renderStudents();     break;
      case 'achievements': await renderAchievements(); break;
      case 'account':      await renderAccount();      break;
      case 'test':         await renderTest();         break;
      default:             el.innerHTML = '<p>Розділ не знайдено.</p>';
    }
  } catch (err) {
    el.innerHTML = `<div class="empty-state"><div class="empty-icon">⚠️</div><p>${esc(err.message)}</p></div>`;
  }
}

// ══════════════════════════════════════════════════════
// COURSES
// ══════════════════════════════════════════════════════
async function renderCourses() {
  const el = document.getElementById('main-content');
  const u  = App.user;

  let courses = [];
  try {
    if (isTeacher()) {
      const data = await api.getCourses({ teacherId: u.id });
      courses = data.courses || [];
    } else {
      const data = await api.getCourses({ studentId: u.id });
      courses = data.courses || [];
    }
  } catch { courses = []; }

  const addBtn = isTeacher()
    ? `<button class="btn btn-primary" onclick="showCreateCourseModal()">${Icon.plus} Додати курс</button>`
    : '';

  let body = '';
  if (courses.length === 0) {
    body = `<div class="empty-state"><div class="empty-icon">📚</div>
      <p>${isTeacher() ? 'Ви ще не створили жодного курсу.' : 'Вас ще не записали на жоден курс.'}</p></div>`;
  } else {
    body = `<div class="accordion-list">${courses.map(c => courseAccordion(c)).join('')}</div>`;
  }

  el.innerHTML = `
    <div class="page-header">
      <h1 class="page-title">Курси та заняття</h1>
      <div style="display:flex;gap:10px">${addBtn}</div>
    </div>
    ${body}
  `;
}

function courseAccordion(c) {
  const badge = isTeacher()
    ? `<span class="badge badge-primary">${esc(c.teacherName || 'Ваш курс')}</span>`
    : `<span class="badge badge-primary">відкрито для вас</span>`;

  const teacherActions = isTeacher() ? `
    <button class="btn btn-sm btn-outline" onclick="event.stopPropagation();navigate('course',{courseId:${c.id},_label:'${esc(c.name)}'},'Курси та заняття')">Відкрити</button>
    <button class="btn btn-sm btn-ghost btn-icon" title="Видалити курс"
      onclick="event.stopPropagation();deleteCourse(${c.id})">${Icon.trash}</button>
  ` : '';

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

  if (item.classList.contains('open')) {
    item.classList.remove('open');
    return;
  }
  item.classList.add('open');

  // Load themes + lessons lazily
  try {
    const thData = await api.getThemes(courseId);
    const themes = thData.themes || [];
    if (!Store.themeMap) Store.themeMap = {};
    themes.forEach(t => { Store.themeMap[t.id] = t; });

    if (themes.length === 0) {
      body.innerHTML = `<div class="empty-state" style="padding:24px"><p>Тем ще немає.</p></div>`;
      return;
    }

    // Load lessons for all themes in parallel
    const lessonMap = {};
    await Promise.all(themes.map(async t => {
      try {
        const ld = await api.getLessons(t.id);
        lessonMap[t.id] = ld.lessons || [];
      } catch { lessonMap[t.id] = []; }
    }));

    // Also load tests
    let tests = [];
    try { const td = await api.getTests(courseId); tests = td.tests || []; } catch {}

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
    : '';

  const addTestBtn = isTeacher()
    ? `<button class="btn btn-sm btn-outline" style="margin-left:8px" onclick="showCreateTestModal(${courseId})">${Icon.plus} Тест</button>`
    : '';

  let html = themes.map(t => {
    const lessons = lessonMap[t.id] || [];
    const addLessonBtn = isTeacher()
      ? `<button class="btn btn-sm btn-ghost" onclick="showCreateLessonModal(${t.id},${courseId})">${Icon.plus} Урок</button>`
      : '';
    const editThemeBtn = isTeacher()
      ? `<button class="btn btn-sm btn-ghost btn-icon" title="Редагувати тему"
           onclick="showEditThemeModal(${courseId},${t.id})">${Icon.edit}</button>`
      : '';

    const lessonsHtml = lessons.length === 0
      ? `<div style="padding:10px 38px;font-size:.82rem;color:var(--text-muted)">Уроків ще немає.</div>`
      : lessons.map(l => renderLessonRow(l, courseId, t.id)).join('');

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
  }).join('');

  // Tests section
  const testsHtml = tests.length > 0 ? `
    <div class="theme-block">
      <div class="theme-row">
        <div class="theme-row-left">
          <span class="theme-title-txt">📝 Тести</span>
        </div>
        <div class="theme-row-right">${addTestBtn}</div>
      </div>
      ${tests.map(t => renderTestRow(t, courseId)).join('')}
    </div>
  ` : (isTeacher() ? `
    <div class="theme-block">
      <div class="theme-row">
        <div class="theme-row-left"><span class="theme-title-txt">📝 Тести</span></div>
        <div class="theme-row-right">${addTestBtn}</div>
      </div>
    </div>
  ` : '');

  return html + testsHtml + addThemeBtn;
}

function renderLessonRow(l, courseId, themeId) {
  const date = l.date ? ` · ${esc(l.date)}` : '';
  const time = l.time ? ` ${esc(l.time)}` : '';

  const actions = isTeacher()
    ? `<button class="btn btn-sm btn-ghost btn-icon" title="Редагувати урок"
         onclick="event.stopPropagation();showEditLessonModal(${App.user.id},${l.id})">${Icon.edit}</button>
       <button class="btn btn-sm btn-ghost btn-icon" title="Видалити урок"
         onclick="event.stopPropagation();deleteLesson(${App.user.id},${l.id},${courseId})">${Icon.trash}</button>`
    : '';

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
  const rowClick = isStudent()
    ? `onclick="navigate('test',{testId:${t.id},courseId:${courseId},_label:'${esc(t.title)}'},'Курси та заняття')"`
    : `onclick="showTestDetailModal(${t.id})"`;

  const actions = isTeacher()
    ? `<button class="btn btn-sm btn-ghost btn-icon" title="Видалити тест"
         onclick="event.stopPropagation();deleteTest(${App.user.id},${t.id},${courseId})">${Icon.trash}</button>`
    : '';

  return `
    <div class="lesson-row row-clickable" ${rowClick}>
      <div class="lesson-info">
        <span class="lesson-title-txt">${esc(t.title)}</span>
        <span class="lesson-meta">Макс. балів: ${t.maxScore || 0}</span>
      </div>
      <div class="lesson-acts">${actions}</div>
    </div>
  `;
}

// ── Course detail (teacher deep-dive) ─────────────────
async function renderCourseDetail() {
  const el       = document.getElementById('main-content');
  const courseId = App.params.courseId;

  const [cData, thData, testData] = await Promise.all([
    api.getCourse(courseId),
    api.getThemes(courseId),
    api.getTests(courseId).catch(() => ({ tests: [] })),
  ]);

  const course = cData.course || cData;
  const themes = thData.themes || [];
  const tests  = testData.tests || [];
  if (!Store.themeMap) Store.themeMap = {};
  themes.forEach(t => { Store.themeMap[t.id] = t; });

  const lessonMap = {};
  await Promise.all(themes.map(async t => {
    try { const ld = await api.getLessons(t.id); lessonMap[t.id] = ld.lessons || []; }
    catch { lessonMap[t.id] = []; }
  }));

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

// ══════════════════════════════════════════════════════
// SCHEDULE
// ══════════════════════════════════════════════════════
// Fallback: collect lessons with dates directly from courses → themes → lessons
async function fetchScheduleFromCourses() {
  const u = App.user;
  const coursesData = isTeacher()
    ? await api.getCourses({ teacherId: u.id })
    : await api.getCourses({ studentId: u.id });
  const courses = coursesData.courses || [];
  const items = [];

  await Promise.all(courses.map(async course => {
    const thData = await api.getThemes(course.id).catch(() => ({ themes: [] }));
    await Promise.all((thData.themes || []).map(async theme => {
      const lData = await api.getLessons(theme.id).catch(() => ({ lessons: [] }));
      (lData.lessons || []).forEach(l => {
        if (l.date) {
          items.push({
            lessonTitle: l.title,
            courseName:  course.name,
            date:        l.date,
            time:        l.time,
            lessonId:    l.id,
          });
        }
      });
    }));
  }));

  return items;
}

async function renderSchedule() {
  const el = document.getElementById('main-content');
  const u  = App.user;

  let items = [];
  try {
    const data = isTeacher()
      ? await api.getTeacherSchedule(u.id)
      : await api.getStudentSchedule(u.id);
    items = data.schedule || data.lessons || [];
  } catch { items = []; }

  // If schedule endpoint returns nothing, build it from lessons directly
  if (items.length === 0) {
    try { items = await fetchScheduleFromCourses(); } catch { }
  }

  // Sort chronologically
  items.sort((a, b) => {
    const da = parseScheduleDate(a.date), db = parseScheduleDate(b.date);
    if (!da && !db) return 0;
    if (!da) return 1;
    if (!db) return -1;
    return da - db;
  });

  const eventDates = items
    .map(it => parseScheduleDate(it.date))
    .filter(d => d && d.getFullYear() === App.calYear && d.getMonth() === App.calMonth)
    .map(d => d.getDate());

  const listHtml = items.length === 0
    ? `<div class="empty-state"><div class="empty-icon">📅</div><p>Розкладу поки немає.</p></div>`
    : items.map(it => {
        const lid = it.lessonId || it.id;
        return `
          <div class="schedule-item${lid ? ' row-clickable' : ''}"
               ${lid ? `onclick="showLessonModal(${lid})"` : ''}>
            <div class="sched-info">
              <div class="sched-title">${esc(it.lessonTitle || it.title || '')}</div>
              <div class="sched-date">${esc(it.date || '')}${it.time ? ' · ' + esc(it.time) : ''}</div>
              <div class="sched-type">${esc(it.courseName || it.type || '')}</div>
            </div>
          </div>`;
      }).join('');

  el.innerHTML = `
    <div class="page-header">
      <h1 class="page-title">Розклад</h1>
    </div>
    <div class="schedule-layout">
      <div class="schedule-list-wrap">${listHtml}</div>
      <div>${calendarWidget(App.calYear, App.calMonth, eventDates)}</div>
    </div>
  `;
}

function calendarWidget(year, month, eventDates = []) {
  const monthNames = ['Січень','Лютий','Березень','Квітень','Травень','Червень',
                      'Липень','Серпень','Вересень','Жовтень','Листопад','Грудень'];
  const dow = ['Пн','Вт','Ср','Чт','Пт','Сб','Нд'];

  const today        = new Date();
  const firstWeekDay = ((new Date(year, month, 1).getDay() + 6) % 7); // Mon=0
  const daysInMonth  = new Date(year, month + 1, 0).getDate();

  let cells = '';
  for (let i = 0; i < firstWeekDay; i++) cells += `<span class="cal-day empty"></span>`;
  for (let d = 1; d <= daysInMonth; d++) {
    const isToday  = d === today.getDate() && month === today.getMonth() && year === today.getFullYear();
    const hasEvent = eventDates.includes(d);
    cells += `<span class="cal-day${isToday ? ' today' : ''}${hasEvent ? ' has-event' : ''}">${d}</span>`;
  }

  return `
    <div class="calendar">
      <div class="cal-header">
        <button class="cal-nav" onclick="calPrev()">&#8249;</button>
        <span class="cal-month-lbl">${monthNames[month]} ${year}</span>
        <button class="cal-nav" onclick="calNext()">&#8250;</button>
      </div>
      <div class="cal-grid">
        ${dow.map(d => `<span class="cal-dow">${d}</span>`).join('')}
        ${cells}
      </div>
    </div>
  `;
}

function calPrev() {
  if (App.calMonth === 0) { App.calYear--; App.calMonth = 11; }
  else App.calMonth--;
  renderContent();
}
function calNext() {
  if (App.calMonth === 11) { App.calYear++; App.calMonth = 0; }
  else App.calMonth++;
  renderContent();
}

async function renderStudents() {
  const el = document.getElementById('main-content');
  const u  = App.user;

  const [sData, gData] = await Promise.all([
    api.getTeacherStudents(u.id).catch(() => ({ students: [] })),
    api.getGroups(u.id).catch(() => ({ groups: [] })),
  ]);
  const students = sData.students || [];
  const groups   = gData.groups   || [];
  Store.students = students;
  Store.groups   = groups;

  const studentRows = students.length === 0
    ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">Студентів ще немає.<br><span style="font-size:.8rem">Додайте студента за email.</span></div>`
    : students.map(s => `
      <div class="student-item clickable" onclick="showStudentProgressModal(${s.id},'${esc(s.fullName)}')">
        <div class="student-avatar">${(s.fullName||'?')[0]}</div>
        <div class="student-info">
          <div class="student-name">${esc(s.fullName)}</div>
          <div class="student-email">${esc(s.email)}</div>
        </div>
        <span style="color:var(--text-muted);font-size:.78rem;flex-shrink:0">прогрес →</span>
      </div>
    `).join('');

  const groupRows = groups.length === 0
    ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">Груп ще немає.</div>`
    : groups.map(g => `
      <div class="group-item clickable" onclick="showGroupModal(${g.id})">
        <div>
          <div class="group-name">${esc(g.name)}</div>
          <div class="group-count">${g.studentCount || 0} студент(ів)</div>
        </div>
        <span style="color:var(--text-muted);font-size:.78rem">переглянути →</span>
      </div>
    `).join('');

  el.innerHTML = `
    <div class="page-header">
      <h1 class="page-title">Студенти та групи</h1>
    </div>
    <div class="students-layout">
      <div class="section-card">
        <div class="section-card-header">
          <span class="section-card-title">Студенти</span>
          <button class="btn btn-sm btn-primary" onclick="showAddStudentModal()">${Icon.plus} Додати</button>
        </div>
        ${studentRows}
      </div>
      <div class="section-card">
        <div class="section-card-header">
          <span class="section-card-title">Групи</span>
          <button class="btn btn-sm btn-primary" onclick="showCreateGroupModal()">${Icon.plus} Група</button>
        </div>
        ${groupRows}
      </div>
    </div>
  `;
}

async function renderAchievements() {
  const el = document.getElementById('main-content');
  const u  = App.user;

  let progress = null;
  try {
    const data = await api.getProgress(u.id);
    progress = data;
  } catch { progress = null; }

  const pct = (a, b) => b > 0 ? Math.round((a / b) * 100) : 0;

  const cards = progress ? `
    <div class="progress-grid">
      ${donutCard(pct(progress.completedLessons, progress.totalLessons), 'Уроки',
          `${progress.completedLessons || 0} з ${progress.totalLessons || 0}`)}
      ${donutCard(pct(progress.passedTests, progress.totalTests), 'Тести',
          `${progress.passedTests || 0} з ${progress.totalTests || 0}`)}
      ${donutCard(pct(progress.enrolledCourses, progress.enrolledCourses), 'Курси',
          `${progress.enrolledCourses || 0} записано`)}
    </div>
    <div class="section-card" style="max-width:600px">
      <div class="section-card-header"><span class="section-card-title">Результати тестів</span></div>
      ${(progress.testResults || []).length === 0
        ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">Тестів ще не здавалось.</div>`
        : (progress.testResults || []).map(tr => `
          <div style="padding:12px 20px;border-bottom:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;gap:12px">
            <div>
              <div style="font-weight:500;font-size:.9rem">${esc(tr.testTitle || 'Тест')}</div>
              <div style="font-size:.77rem;color:var(--text-muted)">${esc(tr.courseName || '')}</div>
            </div>
            <span class="badge ${tr.passed ? 'badge-success' : 'badge-danger'}">${tr.score}/${tr.maxScore}</span>
          </div>
        `).join('')}
    </div>
  ` : `<div class="empty-state"><div class="empty-icon">🏆</div><p>Прогрес поки недоступний.</p></div>`;

  el.innerHTML = `
    <div class="page-header"><h1 class="page-title">Мої досягнення</h1></div>
    ${cards}
  `;
}

function donutCard(percent, label, sublabel) {
  const r    = 30, cx = 40, cy = 40;
  const circ = 2 * Math.PI * r;
  const fill = Math.min(percent, 100) / 100 * circ;
  return `
    <div class="progress-card">
      <svg width="80" height="80" viewBox="0 0 80 80">
        <circle cx="${cx}" cy="${cy}" r="${r}" fill="none" stroke="var(--border)" stroke-width="8"/>
        <circle cx="${cx}" cy="${cy}" r="${r}" fill="none" stroke="var(--primary)" stroke-width="8"
          stroke-dasharray="${fill} ${circ - fill}" stroke-linecap="round"
          transform="rotate(-90 ${cx} ${cy})"/>
        <text x="${cx}" y="${cy + 5}" text-anchor="middle" font-size="15" font-weight="700" fill="var(--text)">${percent}%</text>
      </svg>
      <h4>${esc(label)}</h4>
      <p>${esc(sublabel)}</p>
    </div>
  `;
}

async function renderAccount() {
  const el = document.getElementById('main-content');
  const u  = App.user;

  el.innerHTML = `
    <div class="page-header"><h1 class="page-title">Мій акаунт</h1></div>
    <div class="account-card">
      <div class="account-avatar">${initials(u)}</div>
      <span class="badge badge-primary account-role">${isTeacher() ? 'Викладач' : 'Студент'}</span>
      <form id="account-form" onsubmit="handleUpdateAccount(event)" style="margin-top:18px">
        <div class="form-row">
          <div class="form-group">
            <label class="form-label">Ім'я</label>
            <input class="form-input" id="acc-first" value="${esc(u.firstName)}" required>
          </div>
          <div class="form-group">
            <label class="form-label">Прізвище</label>
            <input class="form-input" id="acc-last" value="${esc(u.lastName)}" required>
          </div>
        </div>
        <div class="form-group">
          <label class="form-label">Email</label>
          <input class="form-input" type="email" id="acc-email" value="${esc(u.email)}" required>
        </div>
        <div class="form-group">
          <label class="form-label">Новий пароль <span style="color:var(--text-muted);font-weight:400">(залиш порожнім якщо не змінюєш)</span></label>
          <input class="form-input" type="password" id="acc-password" placeholder="••••••">
        </div>
        <div id="acc-error" class="form-error hidden"></div>
        <button class="btn btn-primary" type="submit">Зберегти зміни</button>
      </form>
    </div>
  `;
}

async function handleUpdateAccount(e) {
  e.preventDefault();
  const err   = document.getElementById('acc-error');
  err.classList.add('hidden');

  const data = {
    firstName: document.getElementById('acc-first').value.trim(),
    lastName:  document.getElementById('acc-last').value.trim(),
    email:     document.getElementById('acc-email').value.trim(),
  };
  const pw = document.getElementById('acc-password').value;
  if (pw) data.password = pw;

  try {
    await api.updateUser(App.user.id, data);
    App.user = { ...App.user, ...data };
    sessionStorage.setItem('edu_user', JSON.stringify(App.user));
    renderSidebar();
    toast('Профіль оновлено', 'success');
    renderAccount();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

async function renderTest() {
  const el     = document.getElementById('main-content');
  const testId = App.params.testId;

  const data = await api.getTest(testId);
  const test = data.test;
  const qs   = test.questions || [];

  el.innerHTML = `
    <div class="test-page">
      <h2 class="test-page-title">${esc(test.title)}</h2>
      <form id="test-form" onsubmit="submitTest(event,${test.id},${qs.length})">
        ${qs.map((q, idx) => `
          <div class="question-card">
            <div class="q-header">
              <span class="q-num">${idx + 1}</span>
              <span class="q-text">${esc(q.text)}</span>
              <span class="q-score">${q.score} бал</span>
            </div>
            ${(q.options || []).map((opt, k) => `
              <label class="option-label">
                <input type="radio" name="q_${q.id}" value="${1 << k}">
                ${esc(opt)}
              </label>
            `).join('')}
          </div>
        `).join('')}
        <div class="test-footer">
          <button class="btn btn-primary btn-lg" type="submit">Завершити тест</button>
        </div>
      </form>
    </div>
  `;
}

async function submitTest(e, testId, qCount) {
  e.preventDefault();
  const form = document.getElementById('test-form');
  const answers = [];

  for (let i = 0; i < qCount; i++) {
    const radios = form.querySelectorAll(`input[name^="q_"]`);
  }

  const inputs = form.querySelectorAll('input[type="radio"]:checked');
  const maskMap = {};
  inputs.forEach(inp => {
    const name = inp.name; // q_<id>
    const id   = parseInt(name.split('_')[1]);
    maskMap[id] = (maskMap[id] || 0) | parseInt(inp.value);
  });

  // Get ordered question ids
  const qCards = form.querySelectorAll('.question-card');
  const orderedMasks = Array.from(qCards).map(card => {
    const radio = card.querySelector('input[type="radio"]');
    if (!radio) return 0;
    const qId = parseInt(radio.name.split('_')[1]);
    return maskMap[qId] || 0;
  });

  try {
    const res = await api.submitTest(App.user.id, testId, orderedMasks);
    const el  = document.getElementById('main-content');
    el.innerHTML = `
      <div class="test-result">
        <div class="result-score">${res.score} / ${res.maxScore}</div>
        <div class="result-label">балів</div>
        <div class="result-status" style="color:${res.passed ? 'var(--success)' : 'var(--danger)'}">
          ${res.passed ? '✓ Тест пройдено успішно!' : '✕ Тест не пройдено'}
        </div>
        <button class="btn btn-outline" style="margin-top:24px" onclick="goBack()">Назад</button>
      </div>
    `;
  } catch (err) {
    toast(err.message, 'error');
  }
}

function showCreateCourseModal() {
  showModal('modal-create-course', `
    <div class="modal-header">
      <span class="modal-title">Новий курс</span>
      <button class="modal-close" onclick="closeModal('modal-create-course')">✕</button>
    </div>
    <form onsubmit="handleCreateCourse(event)">
      <div class="form-group">
        <label class="form-label">Назва курсу</label>
        <input class="form-input" id="new-course-name" placeholder="Наприклад: Focus 3" required autofocus>
      </div>
      <div id="create-course-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Створити курс</button>
    </form>
  `);
}

async function handleCreateCourse(e) {
  e.preventDefault();
  const name = document.getElementById('new-course-name').value.trim();
  const err  = document.getElementById('create-course-err');
  try {
    await api.createCourse(App.user.id, name);
    closeModal('modal-create-course');
    toast('Курс створено', 'success');
    renderCourses();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

function showCreateThemeModal(courseId) {
  showModal('modal-create-theme', `
    <div class="modal-header">
      <span class="modal-title">Нова тема</span>
      <button class="modal-close" onclick="closeModal('modal-create-theme')">✕</button>
    </div>
    <form onsubmit="handleCreateTheme(event,${courseId})">
      <div class="form-group">
        <label class="form-label">Назва теми</label>
        <input class="form-input" id="new-theme-title" placeholder="Наприклад: Unit 1" required autofocus>
      </div>
      <div id="create-theme-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Створити тему</button>
    </form>
  `);
}

async function handleCreateTheme(e, courseId) {
  e.preventDefault();
  const title = document.getElementById('new-theme-title').value.trim();
  const err   = document.getElementById('create-theme-err');
  try {
    await api.createTheme(App.user.id, courseId, title);
    closeModal('modal-create-theme');
    toast('Тему створено', 'success');
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

function showEditThemeModal(courseId, themeId) {
  const theme = Store.themeMap && Store.themeMap[themeId];
  showModal('modal-edit-theme', `
    <div class="modal-header">
      <span class="modal-title">Редагувати тему</span>
      <button class="modal-close" onclick="closeModal('modal-edit-theme')">✕</button>
    </div>
    <form onsubmit="handleEditTheme(event,${courseId},${themeId})">
      <div class="form-group">
        <label class="form-label">Назва теми</label>
        <input class="form-input" id="edit-theme-title" value="${esc(theme ? theme.title : '')}" required autofocus>
      </div>
      <div id="edit-theme-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Зберегти</button>
    </form>
  `);
}

async function handleEditTheme(e, courseId, themeId) {
  e.preventDefault();
  const title = document.getElementById('edit-theme-title').value.trim();
  const err   = document.getElementById('edit-theme-err');
  try {
    await api.updateTheme(App.user.id, themeId, title);
    if (Store.themeMap && Store.themeMap[themeId]) Store.themeMap[themeId].title = title;
    closeModal('modal-edit-theme');
    toast('Тему оновлено', 'success');
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

function showCreateLessonModal(themeId, courseId) {
  showModal('modal-create-lesson', `
    <div class="modal-header">
      <span class="modal-title">Новий урок</span>
      <button class="modal-close" onclick="closeModal('modal-create-lesson')">✕</button>
    </div>
    <form onsubmit="handleCreateLesson(event,${themeId},${courseId})">
      <div class="form-group">
        <label class="form-label">Назва уроку</label>
        <input class="form-input" id="new-lesson-title" placeholder="Наприклад: Past Tenses" required autofocus>
      </div>
      <div class="form-group">
        <label class="form-label">Матеріал</label>
        <textarea class="form-input form-textarea" id="new-lesson-material" placeholder="Зміст уроку…" required></textarea>
      </div>
      <div class="form-row">
        <div class="form-group">
          <label class="form-label">Дата</label>
          <input class="form-input" id="new-lesson-date" placeholder="ДД.ММ.РР">
        </div>
        <div class="form-group">
          <label class="form-label">Час</label>
          <input class="form-input" id="new-lesson-time" placeholder="11:00">
        </div>
      </div>
      <div class="form-group">
        <label class="form-label">Посилання на домашнє завдання</label>
        <input class="form-input" id="new-lesson-hw" placeholder="https:
      </div>
      <div id="create-lesson-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Створити урок</button>
    </form>
  `, true);
}

async function handleCreateLesson(e, themeId, courseId) {
  e.preventDefault();
  const err = document.getElementById('create-lesson-err');
  try {
    await api.createLesson(
      App.user.id, themeId,
      document.getElementById('new-lesson-title').value.trim(),
      document.getElementById('new-lesson-material').value.trim(),
      document.getElementById('new-lesson-date').value.trim(),
      document.getElementById('new-lesson-time').value.trim(),
      document.getElementById('new-lesson-hw').value.trim(),
    );
    closeModal('modal-create-lesson');
    toast('Урок створено', 'success');
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

function showCreateTestModal(courseId) {
  showModal('modal-create-test', `
    <div class="modal-header">
      <span class="modal-title">Новий тест</span>
      <button class="modal-close" onclick="closeModal('modal-create-test')">✕</button>
    </div>
    <form onsubmit="handleCreateTest(event,${courseId})">
      <div class="form-group">
        <label class="form-label">Назва тесту</label>
        <input class="form-input" id="new-test-title" placeholder="Наприклад: Test of Unit 1" required autofocus>
      </div>
      <div id="test-questions-list"></div>
      <button type="button" class="btn btn-outline btn-sm" onclick="addTestQuestion()" style="margin-bottom:14px">
        ${Icon.plus} Додати запитання
      </button>
      <div id="create-test-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Створити тест</button>
    </form>
  `, true);

  addTestQuestion(); // Start with one question
}

let testQCount = 0;
function addTestQuestion() {
  const id  = ++testQCount;
  const el  = document.getElementById('test-questions-list');
  const div = document.createElement('div');
  div.className = 'question-block';
  div.id        = `tq-${id}`;
  div.innerHTML = `
    <div class="question-block-header">
      <span class="question-block-title">Запитання ${id}</span>
      <button type="button" class="btn btn-ghost btn-icon btn-sm" onclick="document.getElementById('tq-${id}').remove()">${Icon.trash}</button>
    </div>
    <div class="form-group">
      <input class="form-input" name="tq_text_${id}" placeholder="Текст запитання" required>
    </div>
    ${[0,1,2,3].map(k => `
      <div class="option-row">
        <input type="checkbox" name="tq_correct_${id}_${k}">
        <input class="form-input" name="tq_opt_${id}_${k}" placeholder="Варіант ${String.fromCharCode(65+k)}" required>
      </div>
    `).join('')}
    <div class="form-group" style="margin-top:8px">
      <label class="form-label" style="font-weight:400;font-size:.8rem">Балів за питання</label>
      <input class="form-input" type="number" name="tq_score_${id}" value="1" min="0.1" step="0.1" style="width:100px">
    </div>
  `;
  el.appendChild(div);
}

async function handleCreateTest(e, courseId) {
  e.preventDefault();
  const form  = e.target;
  const title = document.getElementById('new-test-title').value.trim();
  const err   = document.getElementById('create-test-err');

  const qBlocks = document.querySelectorAll('#test-questions-list .question-block');
  const questions = [];
  for (const block of qBlocks) {
    const id  = block.id.split('-')[1];
    const text = block.querySelector(`[name="tq_text_${id}"]`).value.trim();
    const opts = [0,1,2,3].map(k => block.querySelector(`[name="tq_opt_${id}_${k}"]`).value.trim());
    const flags = [0,1,2,3].map(k => block.querySelector(`[name="tq_correct_${id}_${k}"]`).checked ? 1 : 0);
    const score  = parseFloat(block.querySelector(`[name="tq_score_${id}"]`).value) || 1;
    if (!text) continue;
    questions.push({ text, options: opts, correctFlags: flags, score });
  }

  if (questions.length === 0) {
    err.textContent = 'Додайте хоча б одне запитання';
    err.classList.remove('hidden');
    return;
  }

  try {
    await api.createTest(App.user.id, courseId, title, questions);
    closeModal('modal-create-test');
    toast('Тест створено', 'success');
    testQCount = 0;
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

// ── Test detail modal (teacher read-only preview) ────────
async function showTestDetailModal(testId) {
  showModal('modal-test-detail', `
    <div class="modal-header">
      <span class="modal-title">Завантаження…</span>
      <button class="modal-close" onclick="closeModal('modal-test-detail')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `, true);

  const data = await api.getTest(testId).catch(() => null);
  if (!data) { toast('Тест не знайдено', 'error'); closeModal('modal-test-detail'); return; }
  const test = data.test || data;
  const qs   = test.questions || [];

  const questionsHtml = qs.length === 0
    ? `<p style="color:var(--text-muted);font-size:.86rem;padding:12px 0">Запитань ще немає.</p>`
    : qs.map((q, idx) => `
        <div class="question-card" style="margin-bottom:12px">
          <div class="q-header">
            <span class="q-num">${idx + 1}</span>
            <span class="q-text">${esc(q.text)}</span>
            <span class="q-score">${q.score} бал</span>
          </div>
          ${(q.options || []).map((opt, k) => {
            const correct = q.correctFlags && q.correctFlags[k];
            return `<div class="option-label" style="${correct ? 'color:var(--success);font-weight:600' : ''}">
              ${correct ? '✓ ' : ''}${esc(opt)}
            </div>`;
          }).join('')}
        </div>`).join('');

  document.querySelector('#modal-test-detail .modal').innerHTML = `
    <div class="modal-header">
      <span class="modal-title">${esc(test.title)}</span>
      <button class="modal-close" onclick="closeModal('modal-test-detail')">✕</button>
    </div>
    <div style="font-size:.84rem;color:var(--text-muted);margin-bottom:16px">
      ${qs.length} запит(ань) · Макс. балів: ${test.maxScore || qs.reduce((a, q) => a + (q.score || 0), 0)}
    </div>
    ${questionsHtml}
  `;
}

// ── Lesson detail modal (teacher + student) ─────────────
async function showLessonModal(lessonId) {
  showModal('modal-lesson', `
    <div class="modal-header">
      <span class="modal-title">Завантаження…</span>
      <button class="modal-close" onclick="closeModal('modal-lesson')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `, true);

  const [lessonData, subData] = await Promise.all([
    api.getLesson(lessonId).catch(() => null),
    isTeacher()
      ? api.getLessonSubmissions(App.user.id, lessonId).catch(() => null)
      : Promise.resolve(null),
  ]);
  const lesson = lessonData ? (lessonData.lesson || lessonData) : null;

  const modal = document.querySelector('#modal-lesson .modal');
  if (!lesson) {
    modal.innerHTML = `
      <div class="modal-header">
        <span class="modal-title">Помилка</span>
        <button class="modal-close" onclick="closeModal('modal-lesson')">✕</button>
      </div>
      <p style="color:var(--danger)">Урок не знайдено.</p>`;
    return;
  }

  // Teacher gets an "Edit" button in the header
  const teacherEditBtn = isTeacher()
    ? `<button class="btn btn-sm btn-outline" style="margin-right:6px"
         onclick="closeModal('modal-lesson');showEditLessonModal(${App.user.id},${lessonId})">
         ${Icon.edit} Редагувати
       </button>`
    : '';

  // Homework block differs by role
  const homeworkSection = lesson.homeworkLink
    ? (isStudent()
        ? `<div class="form-group">
             <label class="form-label">Домашнє завдання</label>
             <a href="${esc(lesson.homeworkLink)}" target="_blank" rel="noopener"
                class="btn btn-outline w-full" style="margin-bottom:10px">
               Відкрити завдання
             </a>
             <label class="form-label" style="margin-top:4px">Здати виконану роботу (посилання)</label>
             <input class="form-input" id="hw-link-input" placeholder="https://docs.google.com/…">
           </div>`
        : `<div class="form-group">
             <label class="form-label">Посилання на домашнє завдання</label>
             <a href="${esc(lesson.homeworkLink)}" target="_blank" rel="noopener"
                class="btn btn-outline w-full">Відкрити завдання</a>
           </div>`)
    : '';

  const studentActions = isStudent()
    ? `<button class="${lesson.homeworkLink ? 'btn btn-primary' : 'btn btn-success'} w-full"
               style="margin-bottom:8px" onclick="markComplete(${lessonId})">
         ${Icon.check} ${lesson.homeworkLink ? 'Здати та позначити пройденим' : 'Позначити урок як пройдений'}
       </button>`
    : '';

  // Teacher: submitted homework list
  let submissionsSection = '';
  if (isTeacher()) {
    const subs = subData ? (subData.submissions || []) : [];
    const rows = subs.length === 0
      ? `<p style="color:var(--text-muted);font-size:.84rem;padding:8px 0">Студенти ще не здали роботу.</p>`
      : subs.map(s => `
          <div style="display:flex;align-items:center;justify-content:space-between;gap:12px;
                      padding:9px 0;border-bottom:1px solid var(--border)">
            <div style="min-width:0">
              <div style="font-weight:500;font-size:.88rem">${esc(s.studentName || 'Студент')}</div>
              ${s.homeworkLink
                ? `<div style="font-size:.74rem;color:var(--text-muted);overflow:hidden;text-overflow:ellipsis;white-space:nowrap">${esc(s.homeworkLink)}</div>`
                : ''}
            </div>
            ${s.homeworkLink
              ? `<a href="${esc(s.homeworkLink)}" target="_blank" rel="noopener"
                    class="btn btn-sm btn-outline" style="flex-shrink:0">Відкрити</a>`
              : `<span class="badge badge-muted" style="flex-shrink:0">без посилання</span>`}
          </div>`).join('');

    submissionsSection = `
      <div style="margin-top:16px;border-top:1px solid var(--border);padding-top:14px">
        <div style="font-weight:600;font-size:.9rem;margin-bottom:10px">📋 Виконані роботи студентів</div>
        ${rows}
      </div>`;
  }

  modal.innerHTML = `
    <div class="modal-header">
      <span class="modal-title" style="overflow:hidden;text-overflow:ellipsis;white-space:nowrap;max-width:320px">${esc(lesson.title)}</span>
      <div style="display:flex;align-items:center;gap:4px;flex-shrink:0">
        ${teacherEditBtn}
        <button class="modal-close" onclick="closeModal('modal-lesson')">✕</button>
      </div>
    </div>

    ${(lesson.date || lesson.time) ? `
      <div class="lesson-detail-meta" style="margin-bottom:14px">
        ${lesson.date ? `<span>📅 <strong>${esc(lesson.date)}</strong></span>` : ''}
        ${lesson.time ? `<span>🕐 <strong>${esc(lesson.time)}</strong></span>` : ''}
      </div>` : ''}

    ${lesson.material ? `
      <div class="lesson-material-block">${esc(lesson.material)}</div>` : ''}

    ${homeworkSection}
    ${studentActions}
    ${submissionsSection}
  `;
}

async function submitHomework(lessonId) {
  // In this backend, homework is stored in the lesson link; just mark as complete
  await markComplete(lessonId);
}

async function markComplete(lessonId) {
  try {
    const hwInput = document.getElementById('hw-link-input');
    const hwLink  = hwInput ? hwInput.value.trim() : '';
    await api.completeLesson(App.user.id, lessonId, hwLink || null);
    closeModal('modal-lesson');
    toast('Урок позначено як пройдений', 'success');
  } catch (err) {
    toast(err.message, 'error');
  }
}

// ── Enroll modal (teacher) — tabs: student list + groups ──
async function showEnrollModal(courseId) {
  showModal('modal-enroll', `
    <div class="modal-header">
      <span class="modal-title">Записати на курс</span>
      <button class="modal-close" onclick="closeModal('modal-enroll')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `, true);

  const [sData, gData] = await Promise.all([
    api.getTeacherStudents(App.user.id).catch(() => ({ students: [] })),
    api.getGroups(App.user.id).catch(() => ({ groups: [] })),
  ]);
  const students = sData.students || [];
  const groups   = gData.groups   || [];
  Store.students = students;
  Store.groups   = groups;

  const studentListHtml = students.length === 0
    ? `<p style="padding:12px;color:var(--text-muted);font-size:.84rem">Немає студентів. Спочатку додайте їх у розділі «Студенти та групи».</p>`
    : students.map(s => `
        <label class="option-label">
          <input type="radio" name="enroll-student" value="${s.id}">
          <div>
            <div style="font-weight:500;font-size:.88rem">${esc(s.fullName)}</div>
            <div style="font-size:.75rem;color:var(--text-muted)">${esc(s.email)}</div>
          </div>
        </label>`).join('');

  const groupListHtml = groups.length === 0
    ? `<p style="padding:12px;color:var(--text-muted);font-size:.84rem">Груп ще немає.</p>`
    : groups.map(g => `
        <label class="option-label">
          <input type="radio" name="enroll-group" value="${g.id}">
          <span>${esc(g.name)} <span style="color:var(--text-muted);font-size:.8rem">(${g.studentCount || 0} студ.)</span></span>
        </label>`).join('');

  document.querySelector('#modal-enroll .modal').innerHTML = `
    <div class="modal-header">
      <span class="modal-title">Записати на курс</span>
      <button class="modal-close" onclick="closeModal('modal-enroll')">✕</button>
    </div>

    <div class="tab-row">
      <button class="tab-btn active" onclick="switchTab(this,'enroll','student')">Студент</button>
      <button class="tab-btn"        onclick="switchTab(this,'enroll','group')">Група</button>
    </div>

    <div id="enroll-pane-student">
      <div class="enroll-list">${studentListHtml}</div>
      <div style="margin-top:10px">
        <div style="font-size:.8rem;color:var(--text-muted);margin-bottom:5px">Або введіть email напряму:</div>
        <input class="form-input" id="enroll-email" placeholder="student@example.com">
      </div>
    </div>

    <div id="enroll-pane-group" class="hidden">
      <div class="enroll-list">${groupListHtml}</div>
    </div>

    <div id="enroll-err" class="form-error hidden" style="margin-top:10px"></div>
    <button class="btn btn-primary w-full" style="margin-top:14px" onclick="handleEnroll(${courseId})">Записати</button>
  `;
}

async function handleEnroll(courseId) {
  const err      = document.getElementById('enroll-err');
  err.classList.add('hidden');
  const isGroup  = !document.getElementById('enroll-pane-group').classList.contains('hidden');

  if (isGroup) {
    const checked = document.querySelector('input[name="enroll-group"]:checked');
    if (!checked) { err.textContent = 'Оберіть групу'; err.classList.remove('hidden'); return; }
    try {
      await api.enrollGroup(App.user.id, courseId, parseInt(checked.value));
      closeModal('modal-enroll');
      toast('Групу записано на курс', 'success');
    } catch (ex) { err.textContent = ex.message; err.classList.remove('hidden'); }
  } else {
    const checked = document.querySelector('input[name="enroll-student"]:checked');
    const email   = (document.getElementById('enroll-email')?.value || '').trim();
    if (!checked && !email) { err.textContent = 'Оберіть студента або введіть email'; err.classList.remove('hidden'); return; }
    try {
      if (checked) await api.enrollStudent(App.user.id, courseId, parseInt(checked.value));
      else         await api.enrollStudentByEmail(App.user.id, courseId, email);
      closeModal('modal-enroll');
      toast('Студента записано на курс', 'success');
    } catch (ex) { err.textContent = ex.message; err.classList.remove('hidden'); }
  }
}

// ── Add student to teacher ─────────────────────────────
function showAddStudentModal() {
  showModal('modal-add-student', `
    <div class="modal-header">
      <span class="modal-title">Додати студента</span>
      <button class="modal-close" onclick="closeModal('modal-add-student')">✕</button>
    </div>
    <div class="form-group">
      <label class="form-label">Email студента</label>
      <input class="form-input" id="add-student-email" placeholder="student@example.com" autofocus>
    </div>
    <div id="add-student-err" class="form-error hidden"></div>
    <button class="btn btn-primary w-full" onclick="handleAddStudent()">Додати</button>
  `);
}

async function handleAddStudent() {
  const email = document.getElementById('add-student-email').value.trim();
  const err   = document.getElementById('add-student-err');
  try {
    await api.addStudentToTeacher(App.user.id, email);
    closeModal('modal-add-student');
    toast('Студента додано', 'success');
    renderStudents();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

// ── Create group ───────────────────────────────────────
async function showCreateGroupModal() {
  showModal('modal-create-group', `
    <div class="modal-header">
      <span class="modal-title">Нова група</span>
      <button class="modal-close" onclick="closeModal('modal-create-group')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `);

  if (!Store.students || Store.students.length === 0) {
    try {
      const sData  = await api.getTeacherStudents(App.user.id);
      Store.students = sData.students || [];
    } catch { Store.students = []; }
  }
  const students = Store.students;

  const studentsHtml = students.length === 0
    ? `<p style="color:var(--text-muted);font-size:.84rem;padding:8px">Немає доступних студентів.</p>`
    : students.map(s => `
        <label class="option-label">
          <input type="checkbox" value="${s.id}" style="accent-color:var(--primary);width:16px;height:16px;flex-shrink:0">
          <div>
            <div style="font-weight:500;font-size:.88rem">${esc(s.fullName)}</div>
            <div style="font-size:.75rem;color:var(--text-muted)">${esc(s.email)}</div>
          </div>
        </label>`).join('');

  document.querySelector('#modal-create-group .modal').innerHTML = `
    <div class="modal-header">
      <span class="modal-title">Нова група</span>
      <button class="modal-close" onclick="closeModal('modal-create-group')">✕</button>
    </div>
    <div class="form-group">
      <label class="form-label">Назва групи</label>
      <input class="form-input" id="new-group-name" placeholder="Наприклад: Група А" autofocus>
    </div>
    <div class="form-group">
      <label class="form-label">Студенти</label>
      <div class="enroll-list" style="max-height:220px">${studentsHtml}</div>
    </div>
    <div id="create-group-err" class="form-error hidden"></div>
    <button class="btn btn-primary w-full" onclick="handleCreateGroup()">Створити групу</button>
  `;
}

async function handleCreateGroup() {
  const name = (document.getElementById('new-group-name')?.value || '').trim();
  const err  = document.getElementById('create-group-err');
  if (!name) { err.textContent = 'Введіть назву групи'; err.classList.remove('hidden'); return; }

  const checked = document.querySelectorAll('#modal-create-group input[type="checkbox"]:checked');
  const ids = Array.from(checked).map(el => parseInt(el.value));
  try {
    await api.createGroup(App.user.id, name, ids);
    closeModal('modal-create-group');
    toast('Групу створено', 'success');
    renderStudents();
  } catch (ex) { err.textContent = ex.message; err.classList.remove('hidden'); }
}

// ── Delete helpers ─────────────────────────────────────
async function deleteCourse(courseId) {
  if (!confirm('Видалити курс? Це незворотньо.')) return;
  try {
    await api.deleteCourse(courseId);
    toast('Курс видалено', 'success');
    renderCourses();
  } catch (err) { toast(err.message, 'error'); }
}

async function deleteLesson(teacherId, lessonId, courseId) {
  if (!confirm('Видалити урок?')) return;
  try {
    await api.deleteLesson(teacherId, lessonId);
    toast('Урок видалено', 'success');
    renderContent();
  } catch (err) { toast(err.message, 'error'); }
}

async function deleteTest(teacherId, testId, courseId) {
  if (!confirm('Видалити тест?')) return;
  try {
    await api.deleteTest(teacherId, testId);
    toast('Тест видалено', 'success');
    renderContent();
  } catch (err) { toast(err.message, 'error'); }
}

// ── Generic modal renderer ─────────────────────────────
function showModal(id, html, large = false) {
  let overlay = document.getElementById(id);
  if (!overlay) {
    overlay = document.createElement('div');
    overlay.className  = 'modal-overlay';
    overlay.id         = id;
    overlay.innerHTML  = `<div class="modal${large ? ' modal-lg' : ''}"></div>`;
    overlay.addEventListener('click', (e) => {
      if (e.target === overlay) overlay.classList.remove('open');
    });
    document.body.appendChild(overlay);
  }
  overlay.querySelector('.modal').innerHTML = html;
  requestAnimationFrame(() => overlay.classList.add('open'));
}

// ── Tab switcher ───────────────────────────────────────
function switchTab(btnEl, prefix, tab) {
  btnEl.closest('.tab-row').querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
  btnEl.classList.add('active');
  document.querySelectorAll(`[id^="${prefix}-pane-"]`).forEach(pane => {
    pane.classList.toggle('hidden', !pane.id.endsWith(tab));
  });
}

// ── Student progress modal (teacher) ───────────────────
async function showStudentProgressModal(studentId, name) {
  showModal('modal-student-progress', `
    <div class="modal-header">
      <div>
        <div class="modal-title">Прогрес студента</div>
        <div style="font-size:.82rem;color:var(--text-muted)">${esc(name)}</div>
      </div>
      <button class="modal-close" onclick="closeModal('modal-student-progress')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `, true);

  try {
    const data    = await api.getProgress(studentId);
    const courses = data.courses || [];

    const coursesHtml = courses.length === 0
      ? `<p style="text-align:center;color:var(--text-muted);padding:20px;font-size:.88rem">
           Студент не записаний на жоден курс.</p>`
      : courses.map(c => {
          const pct = Math.round(c.progressPercent || 0);
          const color = pct >= 70 ? 'var(--success)' : pct >= 30 ? 'var(--warning)' : 'var(--danger)';
          return `
            <div class="progress-row">
              <div class="progress-row-header">
                <span class="progress-row-label">${esc(c.courseName)}</span>
                <span class="progress-row-count">${c.completedLessons} / ${c.totalLessons} уроків</span>
              </div>
              <div class="progress-bar-wrap">
                <div class="progress-bar" style="width:${pct}%;background:${color}"></div>
              </div>
              <div class="progress-row-pct">${pct}%</div>
            </div>`;
        }).join('');

    const overallPct = Math.round(data.totalProgress || 0);
    document.querySelector('#modal-student-progress .modal').innerHTML = `
      <div class="modal-header">
        <div>
          <div class="modal-title">Прогрес студента</div>
          <div style="font-size:.82rem;color:var(--text-muted)">${esc(name)}</div>
        </div>
        <button class="modal-close" onclick="closeModal('modal-student-progress')">✕</button>
      </div>

      <div style="display:flex;align-items:center;gap:14px;padding:14px 0 16px;border-bottom:1px solid var(--border);margin-bottom:18px">
        <div class="student-avatar" style="width:44px;height:44px;font-size:1rem">${(name||'?')[0]}</div>
        <div style="flex:1">
          <div style="font-weight:600;margin-bottom:4px">${esc(name)}</div>
          <div class="progress-bar-wrap" style="margin-bottom:3px">
            <div class="progress-bar" style="width:${overallPct}%"></div>
          </div>
          <div style="font-size:.75rem;color:var(--primary)">Загальний прогрес: ${overallPct}%</div>
        </div>
      </div>

      <div>${coursesHtml}</div>
    `;
  } catch (err) {
    document.querySelector('#modal-student-progress .modal').innerHTML = `
      <div class="modal-header">
        <span class="modal-title">Прогрес: ${esc(name)}</span>
        <button class="modal-close" onclick="closeModal('modal-student-progress')">✕</button>
      </div>
      <p style="color:var(--danger);font-size:.9rem">${esc(err.message)}</p>`;
  }
}

// ── Group view modal ────────────────────────────────────
function showGroupModal(groupId) {
  const groups   = Store.groups   || [];
  const students = Store.students || [];
  const group    = groups.find(g => g.id === groupId);
  if (!group) { toast('Групу не знайдено', 'error'); return; }

  const members = (group.studentIds || []).map(id => {
    const s = students.find(st => st.id === id);
    return s || { id, fullName: `Студент #${id}`, email: '' };
  });

  const membersHtml = members.length === 0
    ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">Група порожня.</div>`
    : members.map(s => `
        <div class="student-item">
          <div class="student-avatar">${(s.fullName||'?')[0]}</div>
          <div class="student-info">
            <div class="student-name">${esc(s.fullName)}</div>
            ${s.email ? `<div class="student-email">${esc(s.email)}</div>` : ''}
          </div>
        </div>`).join('');

  showModal('modal-group', `
    <div class="modal-header">
      <div>
        <div class="modal-title">${esc(group.name)}</div>
        <div style="font-size:.8rem;color:var(--text-muted)">${members.length} студент(ів)</div>
      </div>
      <button class="modal-close" onclick="closeModal('modal-group')">✕</button>
    </div>
    <div class="section-card" style="box-shadow:none;padding:0">
      ${membersHtml}
    </div>
  `);
}

// ── Edit lesson modal (teacher) ─────────────────────────
async function showEditLessonModal(teacherId, lessonId) {
  showModal('modal-edit-lesson', `
    <div class="modal-header">
      <span class="modal-title">Редагувати урок</span>
      <button class="modal-close" onclick="closeModal('modal-edit-lesson')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `, true);

  const data = await api.getLesson(lessonId).catch(() => null);
  if (!data) { toast('Урок не знайдено', 'error'); closeModal('modal-edit-lesson'); return; }
  const l = data.lesson || data;

  document.querySelector('#modal-edit-lesson .modal').innerHTML = `
    <div class="modal-header">
      <span class="modal-title">Редагувати урок</span>
      <button class="modal-close" onclick="closeModal('modal-edit-lesson')">✕</button>
    </div>
    <form onsubmit="handleEditLesson(event,${teacherId},${lessonId})">
      <div class="form-group">
        <label class="form-label">Назва уроку</label>
        <input class="form-input" id="edit-lesson-title" value="${esc(l.title)}" required autofocus>
      </div>
      <div class="form-group">
        <label class="form-label">Матеріал</label>
        <textarea class="form-input form-textarea" id="edit-lesson-material">${esc(l.material)}</textarea>
      </div>
      <div class="form-row">
        <div class="form-group">
          <label class="form-label">Дата</label>
          <input class="form-input" id="edit-lesson-date" value="${esc(l.date)}" placeholder="ДД.ММ.РР">
        </div>
        <div class="form-group">
          <label class="form-label">Час</label>
          <input class="form-input" id="edit-lesson-time" value="${esc(l.time)}" placeholder="11:00">
        </div>
      </div>
      <div class="form-group">
        <label class="form-label">Посилання на домашнє завдання</label>
        <input class="form-input" id="edit-lesson-hw" value="${esc(l.homeworkLink)}" placeholder="https://…">
      </div>
      <div id="edit-lesson-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Зберегти зміни</button>
    </form>
  `;
}

async function handleEditLesson(e, teacherId, lessonId) {
  e.preventDefault();
  const err = document.getElementById('edit-lesson-err');
  try {
    await api.updateLesson(teacherId, lessonId, {
      title:        document.getElementById('edit-lesson-title').value.trim(),
      material:     document.getElementById('edit-lesson-material').value.trim(),
      date:         document.getElementById('edit-lesson-date').value.trim(),
      time:         document.getElementById('edit-lesson-time').value.trim(),
      homeworkLink: document.getElementById('edit-lesson-hw').value.trim(),
    });
    closeModal('modal-edit-lesson');
    toast('Урок оновлено', 'success');
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove('hidden');
  }
}

// ── Logout ─────────────────────────────────────────────
function handleLogout() {
  sessionStorage.removeItem('edu_user');
  window.location.href = 'index.html';
}

// ══════════════════════════════════════════════════════
// INIT
// ══════════════════════════════════════════════════════
document.addEventListener('DOMContentLoaded', () => {
  const userData = sessionStorage.getItem('edu_user');
  if (!userData) {
    window.location.href = 'index.html';
    return;
  }
  App.user = JSON.parse(userData);
  renderSidebar();
  renderBreadcrumbs();
  renderContent();
});
