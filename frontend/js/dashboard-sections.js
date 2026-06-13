async function renderStudents() {
  const el = document.getElementById("main-content");
  const u = App.user;

  const [sData, gData] = await Promise.all([
    api.getTeacherStudents(u.id).catch(() => ({ students: [] })),
    api.getGroups(u.id).catch(() => ({ groups: [] })),
  ]);
  const students = sData.students || [];
  const groups = gData.groups || [];
  Store.students = students;
  Store.groups = groups;

  const studentRows =
    students.length === 0
      ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">
         Студентів ще немає.<br>
         <span style="font-size:.8rem">Додайте студента за email.</span>
       </div>`
      : students
          .map(
            (s) => `
        <div class="student-item">
          <div class="student-avatar">${(s.fullName || "?")[0]}</div>
          <div class="student-info clickable" onclick="showStudentProgressModal(${s.id},'${esc(s.fullName)}')">
            <div class="student-name">${esc(s.fullName)}</div>
            <div class="student-email">${esc(s.email)}</div>
          </div>
          <div style="display:flex;align-items:center;gap:6px;flex-shrink:0">
            <span style="color:var(--text-muted);font-size:.78rem" class="clickable"
                  onclick="showStudentProgressModal(${s.id},'${esc(s.fullName)}')">прогрес →</span>
            <button class="btn btn-sm btn-ghost btn-icon" title="Видалити студента"
                    onclick="removeStudentFromTeacher(${s.id},'${esc(s.fullName)}')">${Icon.trash}</button>
          </div>
        </div>
      `,
          )
          .join("");

  const groupRows =
    groups.length === 0
      ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">Груп ще немає.</div>`
      : groups
          .map(
            (g) => `
        <div class="group-item clickable" onclick="showGroupModal(${g.id})">
          <div>
            <div class="group-name">${esc(g.name)}</div>
            <div class="group-count">${g.studentCount || 0} студент(ів)</div>
          </div>
          <div style="display:flex;align-items:center;gap:6px;flex-shrink:0">
            <span style="color:var(--text-muted);font-size:.78rem">переглянути →</span>
            <button class="btn btn-sm btn-ghost btn-icon" title="Видалити групу"
                    onclick="event.stopPropagation(); deleteGroup(${g.id},'${esc(g.name)}')">${Icon.trash}</button>
          </div>
        </div>
      `,
          )
          .join("");

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
  const el = document.getElementById("main-content");
  const u = App.user;

  let data = null;
  try {
    data = await api.getProgress(u.id);
  } catch {
    data = null;
  }

  if (!data) {
    el.innerHTML = `
      <div class="page-header"><h1 class="page-title">Мої досягнення</h1></div>
      <div class="empty-state">
        <div class="empty-icon">${Icon.trophy}</div>
        <p>Прогрес поки недоступний.</p>
      </div>`;
    return;
  }

  const courses = data.courses || [];
  const overallPct = Math.round(data.totalProgress || 0);
  const totalCompleted = courses.reduce(
    (s, c) => s + (c.completedLessons || 0),
    0,
  );
  const totalLessons = courses.reduce((s, c) => s + (c.totalLessons || 0), 0);
  const completedCourses = courses.filter(
    (c) => (c.progressPercent || 0) >= 100,
  ).length;
  const lessonPct =
    totalLessons > 0 ? Math.round((totalCompleted / totalLessons) * 100) : 0;
  const coursePct =
    courses.length > 0
      ? Math.round((completedCourses / courses.length) * 100)
      : 0;

  const courseRows =
    courses.length === 0
      ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">
         Ви ще не записані на жоден курс.
       </div>`
      : courses
          .map((c) => {
            const pct = Math.round(c.progressPercent || 0);
            return `
          <div class="progress-row">
            <div class="progress-row-header">
              <span class="progress-row-label">${esc(c.courseName)}</span>
              <span class="progress-row-count">${c.completedLessons || 0} / ${c.totalLessons || 0} уроків</span>
            </div>
            <div class="progress-bar-wrap">
              <div class="progress-bar" style="width:${pct}%"></div>
            </div>
            <div class="progress-row-pct">${pct}%</div>
          </div>`;
          })
          .join("");

  el.innerHTML = `
    <div class="page-header"><h1 class="page-title">Мої досягнення</h1></div>
    <div class="progress-grid">
      ${donutCard(overallPct, "Загальний прогрес", `${overallPct}%`)}
      ${donutCard(lessonPct, "Уроки", `${totalCompleted} з ${totalLessons}`)}
      ${donutCard(coursePct, "Курси завершено", `${completedCourses} з ${courses.length}`)}
    </div>
    <div class="section-card" style="max-width:700px">
      <div class="section-card-header">
        <span class="section-card-title">Прогрес по курсах</span>
      </div>
      <div style="padding:16px 20px">
        ${courseRows}
      </div>
    </div>
  `;
}

function donutCard(percent, label, sublabel) {
  const r = 30,
    cx = 40,
    cy = 40;
  const circ = 2 * Math.PI * r;
  const fill = (Math.min(percent, 100) / 100) * circ;
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
  const el = document.getElementById("main-content");
  const u = App.user;

  el.innerHTML = `
    <div class="page-header"><h1 class="page-title">Мій акаунт</h1></div>
    <div class="account-card">
      <div class="account-avatar">${initials(u)}</div>
      <span class="badge badge-primary account-role">${isTeacher() ? "Викладач" : "Студент"}</span>
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
          <label class="form-label">
            Новий пароль
            <span style="color:var(--text-muted);font-weight:400">(залиш порожнім якщо не змінюєш)</span>
          </label>
          <input class="form-input" type="password" id="acc-password" placeholder="••••••">
        </div>
        <div id="acc-error" class="form-error hidden"></div>
        <button class="btn btn-primary" type="submit">Зберегти зміни</button>
      </form>

      <div style="margin-top:32px;padding-top:20px;border-top:1px solid var(--border)">
        <div style="font-size:.84rem;color:var(--text-muted);margin-bottom:10px">Небезпечна зона</div>
        <button class="btn btn-sm" style="background:var(--danger-bg);color:var(--danger-text);border:1px solid var(--danger-text)"
                onclick="showDeleteAccountModal()">
          ${Icon.trash} Видалити акаунт
        </button>
      </div>
    </div>
  `;
}

async function handleUpdateAccount(e) {
  e.preventDefault();
  const err = document.getElementById("acc-error");
  err.classList.add("hidden");

  const data = {
    firstName: document.getElementById("acc-first").value.trim(),
    lastName: document.getElementById("acc-last").value.trim(),
    email: document.getElementById("acc-email").value.trim(),
  };
  const pw = document.getElementById("acc-password").value;
  if (pw) data.password = pw;

  try {
    await api.updateUser(App.user.id, data);
    App.user = { ...App.user, ...data };
    sessionStorage.setItem("edu_user", JSON.stringify(App.user));
    renderSidebar();
    toast("Профіль оновлено", "success");
    renderAccount();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

function showDeleteAccountModal() {
  showModal(
    "modal-delete-account",
    `
    <div class="modal-header">
      <span class="modal-title" style="color:var(--danger-text)">${Icon.trash} Видалення акаунту</span>
      <button class="modal-close" onclick="closeModal('modal-delete-account')">✕</button>
    </div>
    <div style="background:var(--danger-bg);border-radius:var(--radius-sm);padding:12px 14px;margin-bottom:16px;font-size:.84rem;color:var(--danger-text);line-height:1.5">
      Ця дія <strong>незворотня</strong>. Акаунт і всі пов'язані дані будуть видалені назавжди.
    </div>
    <div class="form-group">
      <label class="form-label">
        Для підтвердження введіть слово
        <strong style="color:var(--danger-text);font-family:monospace">ВИДАЛИТИ</strong>
      </label>
      <input class="form-input" id="delete-confirm-input"
             placeholder="ВИДАЛИТИ" autocomplete="off"
             oninput="document.getElementById('delete-confirm-btn').disabled = this.value !== 'ВИДАЛИТИ'">
    </div>
    <button id="delete-confirm-btn" class="btn w-full" disabled
            style="background:var(--danger);color:#fff;opacity:.5;transition:opacity .15s"
            onclick="handleDeleteAccount()"
            onmouseenter="if(!this.disabled)this.style.opacity='.85'"
            onmouseleave="this.style.opacity=this.disabled?'.5':'1'">
      Видалити акаунт назавжди
    </button>
  `,
  );

  const input = document.getElementById("delete-confirm-input");
  if (input)
    input.addEventListener("input", () => {
      const btn = document.getElementById("delete-confirm-btn");
      if (btn) btn.style.opacity = btn.disabled ? ".5" : "1";
    });
}

async function handleDeleteAccount() {
  try {
    await api.deleteUser(App.user.id);
    closeModal("modal-delete-account");
    sessionStorage.removeItem("edu_user");
    toast("Акаунт видалено", "success");
    setTimeout(() => {
      window.location.href = "/";
    }, 1000);
  } catch (err) {
    toast(err.message, "error");
  }
}

async function renderTestResult() {
  const el = document.getElementById("main-content");
  const testId = App.params.testId;

  const data = await api.getTestResult(App.user.id, testId);
  const qs = data.test.questions || [];
  const masks = data.answerMasks || [];

  const scoreDisplay = Number.isInteger(data.score)
    ? data.score
    : parseFloat(data.score.toFixed(2));

  const reviewHtml = qs
    .map((q, idx) => {
      const correctMask = (q.correctFlags || []).reduce(
        (acc, f, k) => (f ? acc | (1 << k) : acc),
        0,
      );
      const userMask = masks[idx] || 0;
      const isCorrect = correctMask === userMask;

      const optionsHtml = (q.options || [])
        .map((opt, k) => {
          const selected = (userMask >> k) & 1;
          const correct = (q.correctFlags || [])[k] === 1;
          let cls = "",
            prefix = "";
          if (selected && correct) {
            cls = "option-result-ok";
            prefix = "✓ ";
          } else if (selected && !correct) {
            cls = "option-result-bad";
            prefix = "✕ ";
          } else if (!selected && correct) {
            cls = "option-result-missed";
            prefix = "△ ";
          }
          return `<div class="option-label ${cls}">${prefix}${esc(opt)}</div>`;
        })
        .join("");

      return `
      <div class="question-card" style="border-left:3px solid ${isCorrect ? "var(--success)" : "var(--danger)"}">
        <div class="q-header">
          <span class="q-num" style="background:${isCorrect ? "var(--success)" : "var(--danger)"}">${idx + 1}</span>
          <span class="q-text">${esc(q.text)}</span>
          <span class="badge ${isCorrect ? "badge-success" : "badge-danger"}">${isCorrect ? "Правильно" : "Помилка"}</span>
        </div>
        ${optionsHtml}
      </div>`;
    })
    .join("");

  el.innerHTML = `
    <div class="test-page">
      <div class="test-result-summary">
        <div class="result-score">${scoreDisplay} / ${data.maxScore}</div>
        <div class="result-label">балів</div>
        <div class="result-status" style="color:${data.passed ? "var(--success)" : "var(--danger)"}">
          ${data.passed ? "✓ Тест пройдено успішно!" : "✕ Тест не пройдено"}
        </div>
        <div style="font-size:.8rem;color:var(--text-muted);margin-top:8px">
          Ви вже проходили цей тест. Повторне проходження неможливе.
        </div>
      </div>
      <div style="margin-top:4px">${reviewHtml}</div>
      <div class="test-footer" style="margin-top:8px">
        <button class="btn btn-outline btn-lg" onclick="goBack()">Назад до курсів</button>
      </div>
    </div>
  `;
}

async function renderTest() {
  const el = document.getElementById("main-content");
  const testId = App.params.testId;

  const data = await api.getTest(testId);
  const test = data.test;
  const qs = test.questions || [];
  Store.currentTest = { test, qs };

  el.innerHTML = `
    <div class="test-page">
      <h2 class="test-page-title">${esc(test.title)}</h2>
      <form id="test-form" onsubmit="submitTest(event,${test.id})">
        ${qs
          .map((q, idx) => {
            const correctCount = (q.correctFlags || []).filter(
              (f) => f === 1,
            ).length;
            const inputType = correctCount > 1 ? "checkbox" : "radio";
            return `
          <div class="question-card">
            <div class="q-header">
              <span class="q-num">${idx + 1}</span>
              <span class="q-text">${esc(q.text)}</span>
              <span class="q-score">${q.score} бал</span>
            </div>
            ${(q.options || [])
              .map(
                (opt, k) => `
              <label class="option-label">
                <input type="${inputType}" name="q_${q.id}" value="${1 << k}">
                ${esc(opt)}
              </label>
            `,
              )
              .join("")}
          </div>`;
          })
          .join("")}
        <div class="test-footer">
          <button class="btn btn-primary btn-lg" type="submit">Завершити тест</button>
        </div>
      </form>
    </div>
  `;
}

async function submitTest(e, testId) {
  e.preventDefault();
  const form = document.getElementById("test-form");
  const { qs } = Store.currentTest || {};

  const maskMap = {};
  form.querySelectorAll("input:checked").forEach((inp) => {
    const id = parseInt(inp.name.split("_")[1]);
    maskMap[id] = (maskMap[id] || 0) | parseInt(inp.value);
  });

  const qCards = form.querySelectorAll(".question-card");
  const orderedMasks = Array.from(qCards).map((card) => {
    const inp = card.querySelector(
      'input[type="radio"], input[type="checkbox"]',
    );
    if (!inp) return 0;
    return maskMap[parseInt(inp.name.split("_")[1])] || 0;
  });

  try {
    const res = await api.submitTest(App.user.id, testId, orderedMasks);
    const el = document.getElementById("main-content");
    const scoreDisplay = Number.isInteger(res.score)
      ? res.score
      : parseFloat(res.score.toFixed(2));

    const reviewHtml = (qs || [])
      .map((q, idx) => {
        const correctMask = (q.correctFlags || []).reduce(
          (acc, f, k) => (f ? acc | (1 << k) : acc),
          0,
        );
        const userMask = orderedMasks[idx] || 0;
        const isCorrect = correctMask === userMask;

        const optionsHtml = (q.options || [])
          .map((opt, k) => {
            const selected = (userMask >> k) & 1;
            const correct = (q.correctFlags || [])[k] === 1;
            let cls = "",
              prefix = "";
            if (selected && correct) {
              cls = "option-result-ok";
              prefix = "✓ ";
            } else if (selected && !correct) {
              cls = "option-result-bad";
              prefix = "✕ ";
            } else if (!selected && correct) {
              cls = "option-result-missed";
              prefix = "△ ";
            }
            return `<div class="option-label ${cls}">${prefix}${esc(opt)}</div>`;
          })
          .join("");

        return `
        <div class="question-card" style="border-left:3px solid ${isCorrect ? "var(--success)" : "var(--danger)"}">
          <div class="q-header">
            <span class="q-num" style="background:${isCorrect ? "var(--success)" : "var(--danger)"}">${idx + 1}</span>
            <span class="q-text">${esc(q.text)}</span>
            <span class="badge ${isCorrect ? "badge-success" : "badge-danger"}">${isCorrect ? "Правильно" : "Помилка"}</span>
          </div>
          ${optionsHtml}
        </div>`;
      })
      .join("");

    el.innerHTML = `
      <div class="test-page">
        <div class="test-result-summary">
          <div class="result-score">${scoreDisplay} / ${res.maxScore}</div>
          <div class="result-label">балів</div>
          <div class="result-status" style="color:${res.passed ? "var(--success)" : "var(--danger)"}">
            ${res.passed ? "✓ Тест пройдено успішно!" : "✕ Тест не пройдено"}
          </div>
        </div>
        <div style="margin-top:4px">${reviewHtml}</div>
        <div class="test-footer" style="margin-top:8px">
          <button class="btn btn-outline btn-lg" onclick="goBack()">Назад до курсів</button>
        </div>
      </div>
    `;
  } catch (err) {
    toast(err.message, "error");
  }
}
