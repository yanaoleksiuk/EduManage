async function fetchScheduleFromCourses() {
  const u = App.user;
  const coursesData = isTeacher()
    ? await api.getCourses({ teacherId: u.id })
    : await api.getCourses({ studentId: u.id });
  const courses = coursesData.courses || [];
  const items = [];

  await Promise.all(
    courses.map(async (course) => {
      const thData = await api
        .getThemes(course.id)
        .catch(() => ({ themes: [] }));
      await Promise.all(
        (thData.themes || []).map(async (theme) => {
          const lData = await api
            .getLessons(theme.id)
            .catch(() => ({ lessons: [] }));
          (lData.lessons || []).forEach((l) => {
            if (l.date) {
              items.push({
                lessonTitle: l.title,
                courseName: course.name,
                date: l.date,
                time: l.time,
                lessonId: l.id,
              });
            }
          });
        }),
      );
    }),
  );

  return items;
}

async function renderSchedule() {
  const u = App.user;

  let items = [];
  try {
    const data = isTeacher()
      ? await api.getTeacherSchedule(u.id)
      : await api.getStudentSchedule(u.id);
    items = data.schedule || data.lessons || [];
  } catch {
    items = [];
  }

  if (items.length === 0) {
    try {
      items = await fetchScheduleFromCourses();
    } catch {}
  }

  items.sort((a, b) => {
    const da = parseScheduleDate(a.date),
      db = parseScheduleDate(b.date);
    if (!da && !db) return 0;
    if (!da) return 1;
    if (!db) return -1;
    return da - db;
  });
  Store.scheduleItems = items;

  renderScheduleView();
}

function renderScheduleView() {
  const el = document.getElementById("main-content");
  if (!el) return;
  const items = Store.scheduleItems || [];
  const sel = App.calSelectedDay;

  const visible = sel
    ? items.filter((it) => {
        const d = parseScheduleDate(it.date);
        return (
          d &&
          d.getFullYear() === App.calYear &&
          d.getMonth() === App.calMonth &&
          d.getDate() === sel
        );
      })
    : items;

  const eventDates = items
    .map((it) => parseScheduleDate(it.date))
    .filter(
      (d) =>
        d && d.getFullYear() === App.calYear && d.getMonth() === App.calMonth,
    )
    .map((d) => d.getDate());

  const monthNames = [
    "січня",
    "лютого",
    "березня",
    "квітня",
    "травня",
    "червня",
    "липня",
    "серпня",
    "вересня",
    "жовтня",
    "листопада",
    "грудня",
  ];

  const filterBar = sel
    ? `<div class="sched-filter-bar">
         ${Icon.schedule} Уроки <strong>${sel} ${monthNames[App.calMonth]} ${App.calYear}</strong>
         <button class="btn btn-sm btn-ghost" style="margin-left:auto" onclick="calClearDay()">
           ✕
         </button>
       </div>`
    : "";

  const listHtml =
    visible.length === 0
      ? `<div class="empty-state" style="padding:32px 0">
         <div class="empty-icon">${Icon.schedule}</div>
         <p>${sel ? "На цей день уроків немає." : "Розкладу поки немає."}</p>
       </div>`
      : visible
          .map((it) => {
            const lid = it.lessonId || it.id;
            return `
          <div class="schedule-item${lid ? " row-clickable" : ""}"
               ${lid ? `onclick="showLessonModal(${lid})"` : ""}>
            <div class="sched-info">
              <div class="sched-title">${esc(it.lessonTitle || it.title || "")}</div>
              <div class="sched-date">${esc(it.date || "")}${it.time ? " · " + esc(it.time) : ""}</div>
              <div class="sched-type">${esc(it.courseName || it.type || "")}</div>
            </div>
          </div>`;
          })
          .join("");

  el.innerHTML = `
    <div class="page-header">
      <h1 class="page-title">Розклад</h1>
    </div>
    <div class="schedule-layout">
      <div class="schedule-list-wrap">
        ${filterBar}
        ${listHtml}
      </div>
      <div>${calendarWidget(App.calYear, App.calMonth, eventDates, sel)}</div>
    </div>
  `;
}

function calendarWidget(year, month, eventDates = [], selectedDay = null) {
  const monthNames = [
    "Січень",
    "Лютий",
    "Березень",
    "Квітень",
    "Травень",
    "Червень",
    "Липень",
    "Серпень",
    "Вересень",
    "Жовтень",
    "Листопад",
    "Грудень",
  ];
  const dow = ["Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Нд"];

  const today = new Date();
  const firstWeekDay = (new Date(year, month, 1).getDay() + 6) % 7;
  const daysInMonth = new Date(year, month + 1, 0).getDate();

  let cells = "";
  for (let i = 0; i < firstWeekDay; i++)
    cells += `<span class="cal-day empty"></span>`;
  for (let d = 1; d <= daysInMonth; d++) {
    const isToday =
      d === today.getDate() &&
      month === today.getMonth() &&
      year === today.getFullYear();
    const hasEvent = eventDates.includes(d);
    const isSelected = d === selectedDay;
    const cls = [
      "cal-day",
      isToday ? "today" : "",
      hasEvent ? "has-event" : "",
      isSelected ? "selected" : "",
    ]
      .filter(Boolean)
      .join(" ");

    const click = hasEvent ? `onclick="calSelectDay(${d})"` : "";
    cells += `<span class="${cls}" ${click}>${d}</span>`;
  }

  return `
    <div class="calendar">
      <div class="cal-header">
        <button class="cal-nav" onclick="calPrev()">&#8249;</button>
        <span class="cal-month-lbl">${monthNames[month]} ${year}</span>
        <button class="cal-nav" onclick="calNext()">&#8250;</button>
      </div>
      <div class="cal-grid">
        ${dow.map((d) => `<span class="cal-dow">${d}</span>`).join("")}
        ${cells}
      </div>
    </div>
  `;
}

function calSelectDay(day) {
  App.calSelectedDay = App.calSelectedDay === day ? null : day;
  renderScheduleView();
}

function calClearDay() {
  App.calSelectedDay = null;
  renderScheduleView();
}

function calPrev() {
  App.calSelectedDay = null;
  if (App.calMonth === 0) {
    App.calYear--;
    App.calMonth = 11;
  } else App.calMonth--;
  renderContent();
}

function calNext() {
  App.calSelectedDay = null;
  if (App.calMonth === 11) {
    App.calYear++;
    App.calMonth = 0;
  } else App.calMonth++;
  renderContent();
}
