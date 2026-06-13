async function showLessonModal(lessonId) {
  showModal(
    "modal-lesson",
    `
    <div class="modal-header">
      <span class="modal-title">Завантаження…</span>
      <button class="modal-close" onclick="closeModal('modal-lesson')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  const [lessonData, subData] = await Promise.all([
    api.getLesson(lessonId, isStudent() ? App.user.id : null).catch(() => null),
    isTeacher()
      ? api.getLessonSubmissions(App.user.id, lessonId).catch(() => null)
      : Promise.resolve(null),
  ]);
  const lesson = lessonData ? lessonData.lesson || lessonData : null;

  const modal = document.querySelector("#modal-lesson .modal");
  if (!lesson) {
    modal.innerHTML = `
      <div class="modal-header">
        <span class="modal-title">Помилка</span>
        <button class="modal-close" onclick="closeModal('modal-lesson')">✕</button>
      </div>
      <p style="color:var(--danger)">Урок не знайдено.</p>`;
    return;
  }

  const teacherEditBtn = isTeacher()
    ? `<button class="btn btn-sm btn-outline" style="margin-right:6px"
         onclick="closeModal('modal-lesson');showEditLessonModal(${App.user.id},${lessonId})">
         ${Icon.edit} Редагувати
       </button>`
    : "";

  const myLink = lesson.myHomeworkLink || "";
  const mySubmitted = !!lesson.mySubmitted;
  const myCompleted = !!lesson.myCompleted;

  const homeworkSection = lesson.homeworkLink
    ? isStudent()
      ? `<div class="form-group">
             <label class="form-label">Домашнє завдання</label>
             <a href="${esc(lesson.homeworkLink)}" target="_blank" rel="noopener"
                class="btn btn-outline w-full" style="margin-bottom:10px">
               Відкрити завдання
             </a>
             <label class="form-label" style="margin-top:4px;display:flex;align-items:center;gap:8px">
               Моє здане завдання (посилання)
               ${
                 mySubmitted
                   ? `<span class="badge badge-success" style="font-size:.7rem">✓ здано</span>`
                   : ""
               }
             </label>
             <input class="form-input" id="hw-link-input"
                    placeholder="https://docs.google.com/…"
                    value="${esc(myLink)}">
             ${
               mySubmitted
                 ? `<div style="font-size:.75rem;color:var(--text-muted);margin-top:4px">
                    Можна оновити посилання і натиснути кнопку нижче.
                  </div>`
                 : ""
             }
           </div>`
      : `<div class="form-group">
             <label class="form-label">Посилання на домашнє завдання</label>
             <a href="${esc(lesson.homeworkLink)}" target="_blank" rel="noopener"
                class="btn btn-outline w-full">Відкрити завдання</a>
           </div>`
    : "";

  const studentActions = isStudent()
    ? (() => {
        if (myCompleted && !lesson.homeworkLink) {
          return `<div class="badge badge-success w-full" style="padding:10px;text-align:center;font-size:.85rem;border-radius:var(--radius-sm)">
                    ${Icon.check} Урок позначено як пройдений
                  </div>`;
        }
        const btnLabel = lesson.homeworkLink
          ? mySubmitted
            ? `${Icon.check} Оновити ДЗ та позначити пройденим`
            : `${Icon.check} Здати та позначити пройденим`
          : myCompleted
            ? `${Icon.check} Урок пройдено (позначити знову)`
            : `${Icon.check} Позначити урок як пройдений`;
        const btnCls = myCompleted
          ? "btn btn-outline"
          : lesson.homeworkLink
            ? "btn btn-primary"
            : "btn btn-success";
        return `<button class="${btnCls} w-full" style="margin-bottom:8px" onclick="markComplete(${lessonId})">
                  ${btnLabel}
                </button>`;
      })()
    : "";

  let submissionsSection = "";
  if (isTeacher()) {
    const subs = subData ? subData.submissions || [] : [];
    const rows =
      subs.length === 0
        ? `<p style="color:var(--text-muted);font-size:.84rem;padding:8px 0">Студенти ще не здали роботу.</p>`
        : subs
            .map(
              (s) => `
          <div style="display:flex;align-items:center;justify-content:space-between;gap:12px;
                      padding:9px 0;border-bottom:1px solid var(--border)">
            <div style="min-width:0">
              <div style="font-weight:500;font-size:.88rem">${esc(s.studentName || "Студент")}</div>
              ${
                s.homeworkLink
                  ? `<div style="font-size:.74rem;color:var(--text-muted);overflow:hidden;text-overflow:ellipsis;white-space:nowrap">${esc(s.homeworkLink)}</div>`
                  : ""
              }
            </div>
            ${
              s.homeworkLink
                ? `<a href="${esc(s.homeworkLink)}" target="_blank" rel="noopener"
                    class="btn btn-sm btn-outline" style="flex-shrink:0">Відкрити</a>`
                : `<span class="badge badge-muted" style="flex-shrink:0">без посилання</span>`
            }
          </div>`,
            )
            .join("");

    submissionsSection = `
      <div style="margin-top:16px;border-top:1px solid var(--border);padding-top:14px">
        <div style="font-weight:600;font-size:.9rem;margin-bottom:10px;display:flex;align-items:center;gap:6px">${Icon.clipboard} Виконані роботи студентів</div>
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

    ${
      lesson.date || lesson.time
        ? `
      <div class="lesson-detail-meta" style="margin-bottom:14px">
        ${lesson.date ? `<span style="display:inline-flex;align-items:center;gap:4px">${Icon.schedule} <strong>${esc(lesson.date)}</strong></span>` : ""}
        ${lesson.time ? `<span style="display:inline-flex;align-items:center;gap:4px">${Icon.clock} <strong>${esc(lesson.time)}</strong></span>` : ""}
      </div>`
        : ""
    }

    ${
      lesson.material
        ? `
      <div class="lesson-material-block">${esc(lesson.material)}</div>`
        : ""
    }

    ${homeworkSection}
    ${studentActions}
    ${submissionsSection}
  `;
}

async function markComplete(lessonId) {
  try {
    const hwInput = document.getElementById("hw-link-input");
    const hwLink = hwInput ? hwInput.value.trim() : "";
    await api.completeLesson(App.user.id, lessonId, hwLink || null);
    closeModal("modal-lesson");
    toast("Урок позначено як пройдений", "success");
  } catch (err) {
    toast(err.message, "error");
  }
}

async function showEnrollModal(courseId) {
  showModal(
    "modal-enroll",
    `
    <div class="modal-header">
      <span class="modal-title">Записати на курс</span>
      <button class="modal-close" onclick="closeModal('modal-enroll')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  const [sData, gData] = await Promise.all([
    api.getTeacherStudents(App.user.id).catch(() => ({ students: [] })),
    api.getGroups(App.user.id).catch(() => ({ groups: [] })),
  ]);
  const students = sData.students || [];
  const groups = gData.groups || [];
  Store.students = students;
  Store.groups = groups;

  const studentListHtml =
    students.length === 0
      ? `<p style="padding:12px;color:var(--text-muted);font-size:.84rem">Немає студентів. Спочатку додайте їх у розділі «Студенти та групи».</p>`
      : students
          .map(
            (s) => `
        <label class="option-label">
          <input type="radio" name="enroll-student" value="${s.id}">
          <div>
            <div style="font-weight:500;font-size:.88rem">${esc(s.fullName)}</div>
            <div style="font-size:.75rem;color:var(--text-muted)">${esc(s.email)}</div>
          </div>
        </label>`,
          )
          .join("");

  const groupListHtml =
    groups.length === 0
      ? `<p style="padding:12px;color:var(--text-muted);font-size:.84rem">Груп ще немає.</p>`
      : groups
          .map(
            (g) => `
        <label class="option-label">
          <input type="radio" name="enroll-group" value="${g.id}">
          <span>${esc(g.name)} <span style="color:var(--text-muted);font-size:.8rem">(${g.studentCount || 0} студ.)</span></span>
        </label>`,
          )
          .join("");

  document.querySelector("#modal-enroll .modal").innerHTML = `
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
  const err = document.getElementById("enroll-err");
  err.classList.add("hidden");
  const isGroup = !document
    .getElementById("enroll-pane-group")
    .classList.contains("hidden");

  if (isGroup) {
    const checked = document.querySelector(
      'input[name="enroll-group"]:checked',
    );
    if (!checked) {
      err.textContent = "Оберіть групу";
      err.classList.remove("hidden");
      return;
    }
    try {
      await api.enrollGroup(App.user.id, courseId, parseInt(checked.value));
      closeModal("modal-enroll");
      toast("Групу записано на курс", "success");
    } catch (ex) {
      err.textContent = ex.message;
      err.classList.remove("hidden");
    }
  } else {
    const checked = document.querySelector(
      'input[name="enroll-student"]:checked',
    );
    const email = (document.getElementById("enroll-email")?.value || "").trim();
    if (!checked && !email) {
      err.textContent = "Оберіть студента або введіть email";
      err.classList.remove("hidden");
      return;
    }
    try {
      if (checked)
        await api.enrollStudent(App.user.id, courseId, parseInt(checked.value));
      else await api.enrollStudentByEmail(App.user.id, courseId, email);
      closeModal("modal-enroll");
      toast("Студента записано на курс", "success");
    } catch (ex) {
      err.textContent = ex.message;
      err.classList.remove("hidden");
    }
  }
}

function showAddStudentModal() {
  showModal(
    "modal-add-student",
    `
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
  `,
  );
}

async function handleAddStudent() {
  const email = document.getElementById("add-student-email").value.trim();
  const err = document.getElementById("add-student-err");
  try {
    await api.addStudentToTeacher(App.user.id, email);
    closeModal("modal-add-student");
    toast("Студента додано", "success");
    renderStudents();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

async function showCreateGroupModal() {
  showModal(
    "modal-create-group",
    `
    <div class="modal-header">
      <span class="modal-title">Нова група</span>
      <button class="modal-close" onclick="closeModal('modal-create-group')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
  );

  if (!Store.students || Store.students.length === 0) {
    try {
      const sData = await api.getTeacherStudents(App.user.id);
      Store.students = sData.students || [];
    } catch {
      Store.students = [];
    }
  }
  const students = Store.students;

  const studentsHtml =
    students.length === 0
      ? `<p style="color:var(--text-muted);font-size:.84rem;padding:8px">Немає доступних студентів.</p>`
      : students
          .map(
            (s) => `
        <label class="option-label">
          <input type="checkbox" value="${s.id}" style="accent-color:var(--primary);width:16px;height:16px;flex-shrink:0">
          <div>
            <div style="font-weight:500;font-size:.88rem">${esc(s.fullName)}</div>
            <div style="font-size:.75rem;color:var(--text-muted)">${esc(s.email)}</div>
          </div>
        </label>`,
          )
          .join("");

  document.querySelector("#modal-create-group .modal").innerHTML = `
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
  const name = (document.getElementById("new-group-name")?.value || "").trim();
  const err = document.getElementById("create-group-err");
  if (!name) {
    err.textContent = "Введіть назву групи";
    err.classList.remove("hidden");
    return;
  }

  const checked = document.querySelectorAll(
    '#modal-create-group input[type="checkbox"]:checked',
  );
  const ids = Array.from(checked).map((el) => parseInt(el.value));
  try {
    await api.createGroup(App.user.id, name, ids);
    closeModal("modal-create-group");
    toast("Групу створено", "success");
    renderStudents();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}
