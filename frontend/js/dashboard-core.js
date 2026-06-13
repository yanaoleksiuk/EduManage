const Store = {};

const App = {
  user: null,
  section: "courses",
  params: {},
  stack: [],
  calYear: new Date().getFullYear(),
  calMonth: new Date().getMonth(),
  calSelectedDay: null,
};

const Icon = {
  book: `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M4 19.5A2.5 2.5 0 0 1 6.5 17H20"/><path d="M6.5 2H20v20H6.5A2.5 2.5 0 0 1 4 19.5v-15A2.5 2.5 0 0 1 6.5 2z"/></svg>`,
  user: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"/><circle cx="12" cy="7" r="4"/></svg>`,
  courses: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2 3h6a4 4 0 0 1 4 4v14a3 3 0 0 0-3-3H2z"/><path d="M22 3h-6a4 4 0 0 0-4 4v14a3 3 0 0 1 3-3h7z"/></svg>`,
  schedule: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"/><line x1="16" y1="2" x2="16" y2="6"/><line x1="8" y1="2" x2="8" y2="6"/><line x1="3" y1="10" x2="21" y2="10"/></svg>`,
  students: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/><circle cx="9" cy="7" r="4"/><path d="M23 21v-2a4 4 0 0 0-3-3.87"/><path d="M16 3.13a4 4 0 0 1 0 7.75"/></svg>`,
  trophy: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="8 21 12 17 16 21"/><line x1="12" y1="17" x2="12" y2="11"/><path d="M7 4H4a2 2 0 0 0-2 2v1c0 4 3 7 6 8"/><path d="M17 4h3a2 2 0 0 1 2 2v1c0 4-3 7-6 8"/><path d="M8 4h8a4 4 0 0 1 0 8H8a4 4 0 0 1 0-8z"/></svg>`,
  logout: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"/><polyline points="16 17 21 12 16 7"/><line x1="21" y1="12" x2="9" y2="12"/></svg>`,
  plus: `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg>`,
  chevron: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="6 9 12 15 18 9"/></svg>`,
  arrowL: `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><line x1="19" y1="12" x2="5" y2="12"/><polyline points="12 19 5 12 12 5"/></svg>`,
  trash: `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="3 6 5 6 21 6"/><path d="M19 6l-1 14H6L5 6"/><path d="M10 11v6"/><path d="M14 11v6"/><path d="M9 6V4h6v2"/></svg>`,
  edit: `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg>`,
  check: `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"/></svg>`,
  alert: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>`,
  clock: `<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg>`,
  clipboard: `<svg xmlns="http://www.w3.org/2000/svg" width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M16 4h2a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2h2"/><rect x="8" y="2" width="8" height="4" rx="1" ry="1"/></svg>`,
};

function esc(s) {
  if (s == null) return "";
  return String(s)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}

function isTeacher() {
  return App.user && App.user.role === "teacher";
}
function isStudent() {
  return App.user && App.user.role === "student";
}

function initials(user) {
  return ((user.firstName || "?")[0] + (user.lastName || "?")[0]).toUpperCase();
}

function parseScheduleDate(dateStr) {
  if (!dateStr) return null;
  if (/^\d{4}-\d{2}-\d{2}/.test(dateStr))
    return new Date(dateStr.substring(0, 10) + "T00:00:00");
  const p = dateStr.split(".");
  if (p.length !== 3) return null;
  const year = p[2].length === 2 ? 2000 + parseInt(p[2]) : parseInt(p[2]);
  const d = new Date(year, parseInt(p[1]) - 1, parseInt(p[0]));
  return isNaN(d.getTime()) ? null : d;
}

function toast(msg, type = "info") {
  const el = document.createElement("div");
  el.className = `toast toast-${type}`;
  const ico = { success: "✓", error: "✕", info: "ℹ" };
  el.innerHTML = `<span>${ico[type] || ""}</span><span>${esc(msg)}</span>`;
  document.getElementById("toast-container").appendChild(el);
  setTimeout(() => {
    el.style.opacity = "0";
    setTimeout(() => el.remove(), 320);
  }, 3600);
}

function openModal(id) {
  const el = document.getElementById(id);
  if (el) el.classList.add("open");
}

function closeModal(id) {
  const el = document.getElementById(id);
  if (el) el.classList.remove("open");
}

function navigate(section, params = {}, label = null) {
  const crumbLabel = label || sectionLabel(App.section, App.params);
  App.stack.push({
    section: App.section,
    params: App.params,
    label: crumbLabel,
  });
  App.section = section;
  App.params = params;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function goBack() {
  if (App.stack.length === 0) return;
  const prev = App.stack.pop();
  App.section = prev.section;
  App.params = prev.params;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function navRoot(section) {
  App.stack = [];
  App.section = section;
  App.params = {};
  if (section !== "schedule") App.calSelectedDay = null;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function sectionLabel(section) {
  const map = {
    courses: "Курси та заняття",
    course: "Курс",
    schedule: "Розклад",
    students: "Студенти та групи",
    achievements: "Мої досягнення",
    account: "Мій акаунт",
    test: "Тест",
    "test-result": "Результат тесту",
  };
  return map[section] || section;
}

function renderBreadcrumbs() {
  const bc = document.getElementById("breadcrumbs");
  const back = document.getElementById("back-btn");

  if (App.stack.length === 0) {
    bc.innerHTML = `<span class="crumb current">${esc(sectionLabel(App.section))}</span>`;
    back.classList.add("hidden");
    return;
  }

  back.classList.remove("hidden");
  let html = "";
  App.stack.forEach((item, idx) => {
    html += `<span class="crumb" onclick="jumpTo(${idx})">${esc(item.label || sectionLabel(item.section))}</span>`;
    html += `<span class="bc-sep">/</span>`;
  });
  html += `<span class="crumb current">${esc(App.params._label || sectionLabel(App.section))}</span>`;
  bc.innerHTML = html;
}

function jumpTo(idx) {
  const target = App.stack[idx];
  App.stack = App.stack.slice(0, idx);
  App.section = target.section;
  App.params = target.params;
  syncSidebarActive();
  renderBreadcrumbs();
  renderContent();
}

function syncSidebarActive() {
  document.querySelectorAll(".nav-item[data-section]").forEach((el) => {
    el.classList.toggle(
      "active",
      el.dataset.section === App.section ||
        (App.stack.length > 0 && App.stack[0].section === el.dataset.section),
    );
  });
}

const _moonIcon = `<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24"
  fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"/>
</svg>`;
const _sunIcon = `<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24"
  fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <circle cx="12" cy="12" r="5"/>
  <line x1="12" y1="1" x2="12" y2="3"/><line x1="12" y1="21" x2="12" y2="23"/>
  <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"/><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"/>
  <line x1="1" y1="12" x2="3" y2="12"/><line x1="21" y1="12" x2="23" y2="12"/>
  <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"/><line x1="18.36" y1="5.64" x2="19.78" y2="4.22"/>
</svg>`;

function initTheme() {
  const saved = localStorage.getItem("edu_theme") || "light";
  document.documentElement.setAttribute("data-theme", saved);
  updateThemeToggle();
}

function toggleTheme() {
  const isDark = document.documentElement.getAttribute("data-theme") === "dark";
  const next = isDark ? "light" : "dark";
  document.documentElement.setAttribute("data-theme", next);
  localStorage.setItem("edu_theme", next);
  updateThemeToggle();
}

function updateThemeToggle() {
  const btn = document.getElementById("theme-toggle");
  if (!btn) return;
  const isDark = document.documentElement.getAttribute("data-theme") === "dark";
  btn.innerHTML = `
    ${isDark ? _sunIcon : _moonIcon}
    <span>${isDark ? "Світла тема" : "Темна тема"}</span>
    <div class="theme-switch"></div>
  `;
}

function renderSidebar() {
  const u = App.user;
  const items = isTeacher()
    ? [
        { section: "account", icon: Icon.user, label: "Мій акаунт" },
        { section: "courses", icon: Icon.courses, label: "Курси" },
        { section: "schedule", icon: Icon.schedule, label: "Розклад" },
        {
          section: "students",
          icon: Icon.students,
          label: "Студенти та групи",
        },
      ]
    : [
        { section: "account", icon: Icon.user, label: "Мій акаунт" },
        { section: "courses", icon: Icon.courses, label: "Курси" },
        { section: "schedule", icon: Icon.schedule, label: "Розклад" },
        { section: "achievements", icon: Icon.trophy, label: "Мої досягнення" },
      ];

  const nav = document.getElementById("sidebar-nav");
  nav.innerHTML = items
    .map(
      (it) => `
    <div class="nav-item${App.section === it.section ? " active" : ""}" data-section="${it.section}"
         onclick="navRoot('${it.section}')">
      ${it.icon}
      <span>${it.label}</span>
    </div>
  `,
    )
    .join("");

  document.getElementById("sidebar-user-name").textContent =
    `${u.firstName} ${u.lastName}`;
  document.getElementById("sidebar-user-role").textContent = isTeacher()
    ? "Викладач"
    : "Студент";
  document.getElementById("sidebar-avatar").textContent = initials(u);
}

async function renderContent() {
  const el = document.getElementById("main-content");
  el.innerHTML = '<div class="loading-wrap"><div class="spinner"></div></div>';

  try {
    switch (App.section) {
      case "courses":
        await renderCourses();
        break;
      case "course":
        await renderCourseDetail();
        break;
      case "schedule":
        await renderSchedule();
        break;
      case "students":
        await renderStudents();
        break;
      case "achievements":
        await renderAchievements();
        break;
      case "account":
        await renderAccount();
        break;
      case "test":
        await renderTest();
        break;
      case "test-result":
        await renderTestResult();
        break;
      default:
        el.innerHTML = "<p>Розділ не знайдено.</p>";
    }
  } catch (err) {
    el.innerHTML = `<div class="empty-state"><div class="empty-icon">${Icon.alert}</div><p>${esc(err.message)}</p></div>`;
  }
}
