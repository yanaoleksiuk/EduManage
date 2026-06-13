function showCreateCourseModal() {
  showModal(
    "modal-create-course",
    `
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
  `,
  );
}

async function handleCreateCourse(e) {
  e.preventDefault();
  const name = document.getElementById("new-course-name").value.trim();
  const err = document.getElementById("create-course-err");
  try {
    await api.createCourse(App.user.id, name);
    closeModal("modal-create-course");
    toast("Курс створено", "success");
    renderCourses();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

function showCreateThemeModal(courseId) {
  showModal(
    "modal-create-theme",
    `
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
  `,
  );
}

async function handleCreateTheme(e, courseId) {
  e.preventDefault();
  const title = document.getElementById("new-theme-title").value.trim();
  const err = document.getElementById("create-theme-err");
  try {
    await api.createTheme(App.user.id, courseId, title);
    closeModal("modal-create-theme");
    toast("Тему створено", "success");
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

function showEditThemeModal(courseId, themeId) {
  const theme = Store.themeMap && Store.themeMap[themeId];
  showModal(
    "modal-edit-theme",
    `
    <div class="modal-header">
      <span class="modal-title">Редагувати тему</span>
      <button class="modal-close" onclick="closeModal('modal-edit-theme')">✕</button>
    </div>
    <form onsubmit="handleEditTheme(event,${courseId},${themeId})">
      <div class="form-group">
        <label class="form-label">Назва теми</label>
        <input class="form-input" id="edit-theme-title" value="${esc(theme ? theme.title : "")}" required autofocus>
      </div>
      <div id="edit-theme-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Зберегти</button>
    </form>
  `,
  );
}

async function handleEditTheme(e, courseId, themeId) {
  e.preventDefault();
  const title = document.getElementById("edit-theme-title").value.trim();
  const err = document.getElementById("edit-theme-err");
  try {
    await api.updateTheme(App.user.id, themeId, title);
    if (Store.themeMap && Store.themeMap[themeId])
      Store.themeMap[themeId].title = title;
    closeModal("modal-edit-theme");
    toast("Тему оновлено", "success");
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

function showCreateLessonModal(themeId, courseId) {
  showModal(
    "modal-create-lesson",
    `
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
        <input class="form-input" id="new-lesson-hw" placeholder="https://…">
      </div>
      <div id="create-lesson-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Створити урок</button>
    </form>
  `,
    true,
  );
}

async function handleCreateLesson(e, themeId, courseId) {
  e.preventDefault();
  const err = document.getElementById("create-lesson-err");
  try {
    await api.createLesson(
      App.user.id,
      themeId,
      document.getElementById("new-lesson-title").value.trim(),
      document.getElementById("new-lesson-material").value.trim(),
      document.getElementById("new-lesson-date").value.trim(),
      document.getElementById("new-lesson-time").value.trim(),
      document.getElementById("new-lesson-hw").value.trim(),
    );
    closeModal("modal-create-lesson");
    toast("Урок створено", "success");
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

// ── Create Test Modal ──────────────────────────────────────

function showCreateTestModal(courseId) {
  testQCount = 0;
  showModal(
    "modal-create-test",
    `
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
  `,
    true,
  );

  addTestQuestion();
}

let testQCount = 0;

function addTestQuestion() {
  const id = ++testQCount;
  const el = document.getElementById("test-questions-list");
  const div = document.createElement("div");
  div.className = "question-block";
  div.id = `tq-${id}`;
  div.innerHTML = `
    <div class="question-block-header">
      <span class="question-block-title">Запитання ${id}</span>
      <button type="button" class="btn btn-ghost btn-icon btn-sm"
        onclick="this.closest('.question-block').remove(); renumberCreateQuestions()">${Icon.trash}</button>
    </div>
    <div class="form-group">
      <input class="form-input" name="tq_text_${id}" placeholder="Текст запитання" required>
    </div>
    <div class="options-list" id="tq-opts-${id}">
      ${[0, 1, 2, 3]
        .map(
          (k) => `
      <div class="option-row">
        <input type="checkbox">
        <input class="form-input" placeholder="Варіант ${String.fromCharCode(65 + k)}" required>
        <button type="button" class="btn btn-ghost btn-icon btn-sm" style="min-width:28px;flex-shrink:0"
          onclick="removeOption(this)" title="Видалити варіант">✕</button>
      </div>`,
        )
        .join("")}
    </div>
    <button type="button" class="btn btn-ghost btn-sm" onclick="addCreateOption(${id})"
      style="margin-top:4px;margin-bottom:4px;font-size:.8rem;padding:4px 10px">
      ${Icon.plus} Варіант
    </button>
    <div class="form-group" style="margin-top:8px">
      <label class="form-label" style="font-weight:400;font-size:.8rem">Балів за питання</label>
      <input class="form-input" type="number" name="tq_score_${id}" value="1" min="0.1" step="0.1" style="width:100px">
    </div>
  `;
  el.appendChild(div);
  renumberCreateQuestions();
}

function renumberCreateQuestions() {
  document
    .querySelectorAll("#test-questions-list .question-block")
    .forEach((block, i) => {
      const title = block.querySelector(".question-block-title");
      if (title) title.textContent = `Запитання ${i + 1}`;
    });
}

function addCreateOption(qId) {
  const optsList = document.getElementById(`tq-opts-${qId}`);
  const k = optsList.querySelectorAll(".option-row").length;
  const row = document.createElement("div");
  row.className = "option-row";
  row.innerHTML = `
    <input type="checkbox">
    <input class="form-input" placeholder="Варіант ${String.fromCharCode(65 + k)}" required>
    <button type="button" class="btn btn-ghost btn-icon btn-sm" style="min-width:28px;flex-shrink:0"
      onclick="removeOption(this)" title="Видалити варіант">✕</button>
  `;
  optsList.appendChild(row);
}

function removeOption(btn) {
  const row = btn.closest(".option-row");
  const optsList = row.parentElement;
  if (optsList.querySelectorAll(".option-row").length > 2) {
    row.remove();
  } else {
    toast("Мінімум 2 варіанти відповіді", "warning");
  }
}

async function handleCreateTest(e, courseId) {
  e.preventDefault();
  const title = document.getElementById("new-test-title").value.trim();
  const err = document.getElementById("create-test-err");
  const qBlocks = document.querySelectorAll(
    "#test-questions-list .question-block",
  );

  const questions = [];
  for (const block of qBlocks) {
    const id = block.id.split("-")[1];
    const text = block.querySelector(`[name="tq_text_${id}"]`).value.trim();
    const optRows = block.querySelectorAll(".option-row");
    const opts = Array.from(optRows).map((row) =>
      row.querySelector("input.form-input").value.trim(),
    );
    const flags = Array.from(optRows).map((row) =>
      row.querySelector('input[type="checkbox"]').checked ? 1 : 0,
    );
    const score =
      parseFloat(block.querySelector(`[name="tq_score_${id}"]`).value) || 1;
    if (!text) continue;
    questions.push({ text, options: opts, correctFlags: flags, score });
  }

  if (questions.length === 0) {
    err.textContent = "Додайте хоча б одне запитання";
    err.classList.remove("hidden");
    return;
  }

  try {
    await api.createTest(App.user.id, courseId, title, questions);
    closeModal("modal-create-test");
    toast("Тест створено", "success");
    testQCount = 0;
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}

// ── Test detail modal (teacher read-only preview) ──────────

async function showTestDetailModal(testId) {
  showModal(
    "modal-test-detail",
    `
    <div class="modal-header">
      <span class="modal-title">Завантаження…</span>
      <button class="modal-close" onclick="closeModal('modal-test-detail')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  const data = await api.getTest(testId).catch(() => null);
  if (!data) {
    toast("Тест не знайдено", "error");
    closeModal("modal-test-detail");
    return;
  }
  const test = data.test || data;
  const qs = test.questions || [];

  const questionsHtml =
    qs.length === 0
      ? `<p style="color:var(--text-muted);font-size:.86rem;padding:12px 0">Запитань ще немає.</p>`
      : qs
          .map(
            (q, idx) => `
        <div class="question-card" style="margin-bottom:12px">
          <div class="q-header">
            <span class="q-num">${idx + 1}</span>
            <span class="q-text">${esc(q.text)}</span>
            <span class="q-score">${q.score} бал</span>
          </div>
          ${(q.options || [])
            .map((opt, k) => {
              const correct = q.correctFlags && q.correctFlags[k];
              return `<div class="option-label" style="${correct ? "color:var(--success);font-weight:600" : ""}">
              ${correct ? "✓ " : ""}${esc(opt)}
            </div>`;
            })
            .join("")}
        </div>`,
          )
          .join("");

  document.querySelector("#modal-test-detail .modal").innerHTML = `
    <div class="modal-header">
      <span class="modal-title" style="overflow:hidden;text-overflow:ellipsis;white-space:nowrap;max-width:360px">${esc(test.title)}</span>
      <button class="modal-close" onclick="closeModal('modal-test-detail')">✕</button>
    </div>
    <div style="font-size:.84rem;color:var(--text-muted);margin-bottom:16px">
      ${qs.length} запит(ань) · Макс. балів: ${test.maxScore || qs.reduce((a, q) => a + (q.score || 0), 0)}
    </div>
    ${questionsHtml}
  `;
}

// ── Edit Test Modal ────────────────────────────────────────

let editTestQCount = 0;

async function showEditTestModal(testId) {
  showModal(
    "modal-edit-test",
    `
    <div class="modal-header">
      <span class="modal-title">Завантаження…</span>
      <button class="modal-close" onclick="closeModal('modal-edit-test')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  const data = await api.getTest(testId).catch(() => null);
  if (!data) {
    toast("Тест не знайдено", "error");
    closeModal("modal-edit-test");
    return;
  }
  const test = data.test || data;
  const qs = test.questions || [];

  editTestQCount = qs.length;
  const questionsHtml = qs
    .map((q, idx) => buildEditQuestionBlock(idx + 1, q))
    .join("");

  document.querySelector("#modal-edit-test .modal").innerHTML = `
    <div class="modal-header">
      <span class="modal-title">Редагувати тест</span>
      <button class="modal-close" onclick="closeModal('modal-edit-test')">✕</button>
    </div>
    <form onsubmit="handleEditTest(event,${App.user.id},${testId})">
      <div class="form-group">
        <label class="form-label">Назва тесту</label>
        <input class="form-input" id="edit-test-title" value="${esc(test.title)}" required autofocus>
      </div>
      <div id="edit-test-questions-list">${questionsHtml}</div>
      <button type="button" class="btn btn-outline btn-sm" onclick="addEditTestQuestion()" style="margin-bottom:14px">
        ${Icon.plus} Додати запитання
      </button>
      <div id="edit-test-err" class="form-error hidden"></div>
      <button class="btn btn-primary w-full" type="submit">Зберегти тест</button>
    </form>
  `;
}

function buildEditQuestionBlock(id, q) {
  const rawOpts = q.options || [];
  const opts =
    rawOpts.length >= 2
      ? rawOpts
      : [...rawOpts, ...Array(2 - rawOpts.length).fill("")];
  const flags = q.correctFlags || [];
  const score = q.score || 1;

  return `
    <div class="question-block" id="etq-${id}">
      <div class="question-block-header">
        <span class="question-block-title">Запитання ${id}</span>
        <button type="button" class="btn btn-ghost btn-icon btn-sm"
          onclick="this.closest('.question-block').remove(); renumberEditQuestions()">${Icon.trash}</button>
      </div>
      <div class="form-group">
        <input class="form-input" name="etq_text_${id}" placeholder="Текст запитання"
               value="${esc(q.text || "")}" required>
      </div>
      <div class="options-list" id="etq-opts-${id}">
        ${opts
          .map(
            (opt, k) => `
        <div class="option-row">
          <input type="checkbox" ${flags[k] ? "checked" : ""}>
          <input class="form-input" placeholder="Варіант ${String.fromCharCode(65 + k)}"
                 value="${esc(opt)}" required>
          <button type="button" class="btn btn-ghost btn-icon btn-sm" style="min-width:28px;flex-shrink:0"
            onclick="removeOption(this)" title="Видалити варіант">✕</button>
        </div>`,
          )
          .join("")}
      </div>
      <button type="button" class="btn btn-ghost btn-sm" onclick="addEditOption(${id})"
        style="margin-top:4px;margin-bottom:4px;font-size:.8rem;padding:4px 10px">
        ${Icon.plus} Варіант
      </button>
      <div class="form-group" style="margin-top:8px">
        <label class="form-label" style="font-weight:400;font-size:.8rem">Балів за питання</label>
        <input class="form-input" type="number" name="etq_score_${id}" value="${score}" min="0.1" step="0.1" style="width:100px">
      </div>
    </div>`;
}

function renumberEditQuestions() {
  document
    .querySelectorAll("#edit-test-questions-list .question-block")
    .forEach((block, i) => {
      const title = block.querySelector(".question-block-title");
      if (title) title.textContent = `Запитання ${i + 1}`;
    });
}

function addEditOption(qId) {
  const optsList = document.getElementById(`etq-opts-${qId}`);
  const k = optsList.querySelectorAll(".option-row").length;
  const row = document.createElement("div");
  row.className = "option-row";
  row.innerHTML = `
    <input type="checkbox">
    <input class="form-input" placeholder="Варіант ${String.fromCharCode(65 + k)}" required>
    <button type="button" class="btn btn-ghost btn-icon btn-sm" style="min-width:28px;flex-shrink:0"
      onclick="removeOption(this)" title="Видалити варіант">✕</button>
  `;
  optsList.appendChild(row);
}

function addEditTestQuestion() {
  const id = ++editTestQCount;
  const el = document.getElementById("edit-test-questions-list");
  const div = document.createElement("div");
  div.innerHTML = buildEditQuestionBlock(id, {});
  el.appendChild(div.firstElementChild);
  renumberEditQuestions();
}

async function handleEditTest(e, teacherId, testId) {
  e.preventDefault();
  const title = document.getElementById("edit-test-title").value.trim();
  const err = document.getElementById("edit-test-err");
  const qBlocks = document.querySelectorAll(
    "#edit-test-questions-list .question-block",
  );

  const questions = [];
  for (const block of qBlocks) {
    const id = block.id.split("-")[1];
    const text = block.querySelector(`[name="etq_text_${id}"]`).value.trim();
    const optRows = block.querySelectorAll(".option-row");
    const opts = Array.from(optRows).map((row) =>
      row.querySelector("input.form-input").value.trim(),
    );
    const flags = Array.from(optRows).map((row) =>
      row.querySelector('input[type="checkbox"]').checked ? 1 : 0,
    );
    const score =
      parseFloat(block.querySelector(`[name="etq_score_${id}"]`).value) || 1;
    if (!text) continue;
    questions.push({ text, options: opts, correctFlags: flags, score });
  }

  if (questions.length === 0) {
    err.textContent = "Додайте хоча б одне запитання";
    err.classList.remove("hidden");
    return;
  }

  try {
    await api.updateTest(teacherId, testId, title, questions);
    closeModal("modal-edit-test");
    toast("Тест оновлено", "success");
    editTestQCount = 0;
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}
