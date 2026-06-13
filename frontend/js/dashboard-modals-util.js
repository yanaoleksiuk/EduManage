async function deleteCourse(courseId) {
  if (!confirm("Видалити курс? Це незворотньо.")) return;
  try {
    await api.deleteCourse(courseId);
    toast("Курс видалено", "success");
    renderCourses();
  } catch (err) {
    toast(err.message, "error");
  }
}

async function removeStudentFromTeacher(studentId, studentName) {
  if (!confirm(`Видалити ${studentName} зі списку студентів?`)) return;
  try {
    await api.removeStudentFromTeacher(App.user.id, studentId);
    toast("Студента видалено зі списку", "success");
    renderStudents();
  } catch (err) {
    toast(err.message, "error");
  }
}

async function deleteTheme(teacherId, themeId) {
  if (!confirm("Видалити тему разом з усіма її уроками? Це незворотньо."))
    return;
  try {
    await api.deleteTheme(teacherId, themeId);
    toast("Тему видалено", "success");
    renderContent();
  } catch (err) {
    toast(err.message, "error");
  }
}

async function deleteLesson(teacherId, lessonId, courseId) {
  if (!confirm("Видалити урок?")) return;
  try {
    await api.deleteLesson(teacherId, lessonId);
    toast("Урок видалено", "success");
    renderContent();
  } catch (err) {
    toast(err.message, "error");
  }
}

async function deleteTest(teacherId, testId, courseId) {
  if (!confirm("Видалити тест?")) return;
  try {
    await api.deleteTest(teacherId, testId);
    toast("Тест видалено", "success");
    renderContent();
  } catch (err) {
    toast(err.message, "error");
  }
}

function showModal(id, html, large = false) {
  let overlay = document.getElementById(id);
  if (!overlay) {
    overlay = document.createElement("div");
    overlay.className = "modal-overlay";
    overlay.id = id;
    overlay.innerHTML = `<div class="modal${large ? " modal-lg" : ""}"></div>`;
    overlay.addEventListener("click", (e) => {
      if (e.target === overlay) overlay.classList.remove("open");
    });
    document.body.appendChild(overlay);
  }
  overlay.querySelector(".modal").innerHTML = html;
  requestAnimationFrame(() => overlay.classList.add("open"));
}

function switchTab(btnEl, prefix, tab) {
  btnEl
    .closest(".tab-row")
    .querySelectorAll(".tab-btn")
    .forEach((b) => b.classList.remove("active"));
  btnEl.classList.add("active");
  document.querySelectorAll(`[id^="${prefix}-pane-"]`).forEach((pane) => {
    pane.classList.toggle("hidden", !pane.id.endsWith(tab));
  });
}

async function showStudentProgressModal(studentId, name) {
  showModal(
    "modal-student-progress",
    `
    <div class="modal-header">
      <div>
        <div class="modal-title">Прогрес студента</div>
        <div style="font-size:.82rem;color:var(--text-muted)">${esc(name)}</div>
      </div>
      <button class="modal-close" onclick="closeModal('modal-student-progress')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  try {
    const [progressData, testResultsData] = await Promise.all([
      api.getProgress(studentId),
      api.getStudentTestResults(studentId).catch(() => ({ results: [] })),
    ]);
    const courses = progressData.courses || [];
    const testResults = testResultsData.results || [];

    const coursesHtml =
      courses.length === 0
        ? `<p style="text-align:center;color:var(--text-muted);padding:20px;font-size:.88rem">
           Студент не записаний на жоден курс.</p>`
        : courses
            .map((c) => {
              const pct = Math.round(c.progressPercent || 0);
              const color =
                pct >= 70
                  ? "var(--success)"
                  : pct >= 30
                    ? "var(--warning)"
                    : "var(--danger)";
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
            })
            .join("");

    const testsHtml =
      testResults.length === 0
        ? `<p style="color:var(--text-muted);font-size:.84rem;padding:8px 0">Студент ще не проходив жодного тесту.</p>`
        : testResults
            .map((r) => {
              const scoreDisp = Number.isInteger(r.score)
                ? r.score
                : parseFloat(r.score.toFixed(2));
              const pct =
                r.maxScore > 0 ? Math.round((r.score / r.maxScore) * 100) : 0;
              return `
            <div style="display:flex;align-items:center;justify-content:space-between;gap:12px;
                        padding:9px 0;border-bottom:1px solid var(--border)">
              <div style="min-width:0">
                <div style="font-weight:500;font-size:.88rem;overflow:hidden;text-overflow:ellipsis;white-space:nowrap">
                  ${esc(r.testTitle)}
                </div>
                <div style="font-size:.76rem;color:var(--text-muted)">${scoreDisp} / ${r.maxScore} балів (${pct}%)</div>
              </div>
              <div style="display:flex;align-items:center;gap:8px;flex-shrink:0">
                <span class="badge ${r.passed ? "badge-success" : "badge-danger"}" style="font-size:.7rem">
                  ${r.passed ? "Здано" : "Не здано"}
                </span>
                <button class="btn btn-sm btn-outline"
                  onclick="showStudentTestResultModal(${studentId},'${esc(name)}',${r.testId})">
                  Деталі
                </button>
              </div>
            </div>`;
            })
            .join("");

    const overallPct = Math.round(progressData.totalProgress || 0);
    document.querySelector("#modal-student-progress .modal").innerHTML = `
      <div class="modal-header">
        <div>
          <div class="modal-title">Прогрес студента</div>
          <div style="font-size:.82rem;color:var(--text-muted)">${esc(name)}</div>
        </div>
        <button class="modal-close" onclick="closeModal('modal-student-progress')">✕</button>
      </div>

      <div style="display:flex;align-items:center;gap:14px;padding:14px 0 16px;border-bottom:1px solid var(--border);margin-bottom:18px">
        <div class="student-avatar" style="width:44px;height:44px;font-size:1rem">${(name || "?")[0]}</div>
        <div style="flex:1">
          <div style="font-weight:600;margin-bottom:4px">${esc(name)}</div>
          <div class="progress-bar-wrap" style="margin-bottom:3px">
            <div class="progress-bar" style="width:${overallPct}%"></div>
          </div>
          <div style="font-size:.75rem;color:var(--primary)">Загальний прогрес: ${overallPct}%</div>
        </div>
      </div>

      <div style="font-weight:600;font-size:.9rem;margin-bottom:12px">Уроки по курсах</div>
      <div>${coursesHtml}</div>

      <div style="font-weight:600;font-size:.9rem;margin-top:20px;margin-bottom:10px;
                  padding-top:16px;border-top:1px solid var(--border)">
        ${Icon.clipboard} Результати тестів
      </div>
      <div>${testsHtml}</div>
    `;
  } catch (err) {
    document.querySelector("#modal-student-progress .modal").innerHTML = `
      <div class="modal-header">
        <span class="modal-title">Прогрес: ${esc(name)}</span>
        <button class="modal-close" onclick="closeModal('modal-student-progress')">✕</button>
      </div>
      <p style="color:var(--danger);font-size:.9rem">${esc(err.message)}</p>`;
  }
}

async function showStudentTestResultModal(studentId, studentName, testId) {
  showModal(
    "modal-test-result-detail",
    `
    <div class="modal-header">
      <span class="modal-title">Завантаження…</span>
      <button class="modal-close" onclick="closeModal('modal-test-result-detail')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  try {
    const data = await api.getTestResult(studentId, testId);
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
            return `<div class="option-label ${cls}" style="font-size:.83rem;padding:6px 10px">${prefix}${esc(opt)}</div>`;
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

    document.querySelector("#modal-test-result-detail .modal").innerHTML = `
      <div class="modal-header">
        <div>
          <div class="modal-title">${esc(data.test.title)}</div>
          <div style="font-size:.8rem;color:var(--text-muted)">${esc(studentName)}</div>
        </div>
        <button class="modal-close" onclick="closeModal('modal-test-result-detail')">✕</button>
      </div>
      <div style="display:flex;align-items:center;gap:16px;padding:12px 0 14px;
                  border-bottom:1px solid var(--border);margin-bottom:14px">
        <div style="font-size:1.6rem;font-weight:800;color:var(--primary)">${scoreDisplay} / ${data.maxScore}</div>
        <span class="badge ${data.passed ? "badge-success" : "badge-danger"}" style="font-size:.8rem">
          ${data.passed ? "Тест здано" : "Тест не здано"}
        </span>
      </div>
      <div style="max-height:55vh;overflow-y:auto">${reviewHtml}</div>
    `;
  } catch (err) {
    document.querySelector("#modal-test-result-detail .modal").innerHTML = `
      <div class="modal-header">
        <span class="modal-title">Помилка</span>
        <button class="modal-close" onclick="closeModal('modal-test-result-detail')">✕</button>
      </div>
      <p style="color:var(--danger)">${esc(err.message)}</p>`;
  }
}

function showGroupModal(groupId) {
  const groups = Store.groups || [];
  const students = Store.students || [];
  const group = groups.find((g) => g.id === groupId);
  if (!group) {
    toast("Групу не знайдено", "error");
    return;
  }

  const members = (group.studentIds || []).map((id) => {
    const s = students.find((st) => st.id === id);
    return s || { id, fullName: `Студент #${id}`, email: "" };
  });

  const membersHtml =
    members.length === 0
      ? `<div style="padding:20px;text-align:center;color:var(--text-muted);font-size:.85rem">Група порожня.</div>`
      : members
          .map(
            (s) => `
        <div class="student-item">
          <div class="student-avatar">${(s.fullName || "?")[0]}</div>
          <div class="student-info">
            <div class="student-name">${esc(s.fullName)}</div>
            ${s.email ? `<div class="student-email">${esc(s.email)}</div>` : ""}
          </div>
        </div>`,
          )
          .join("");

  showModal(
    "modal-group",
    `
    <div class="modal-header">
      <div>
        <div class="modal-title">${esc(group.name)}</div>
        <div style="font-size:.8rem;color:var(--text-muted)">${members.length} студент(ів)</div>
      </div>
      <div style="display:flex;align-items:center;gap:8px">
        <button class="btn btn-sm btn-ghost" style="color:var(--danger,#e74c3c)"
          onclick="closeModal('modal-group'); deleteGroup(${group.id},'${esc(group.name)}')">
          ${Icon.trash} Видалити групу
        </button>
        <button class="modal-close" onclick="closeModal('modal-group')">✕</button>
      </div>
    </div>
    <div class="section-card" style="box-shadow:none;padding:0">
      ${membersHtml}
    </div>
  `,
  );
}

async function deleteGroup(groupId, groupName) {
  if (
    !confirm(
      `Видалити групу "${groupName}"?\nСтуденти зі списку не будуть видалені.`,
    )
  )
    return;
  try {
    await api.deleteGroup(App.user.id, groupId);
    toast(`Групу "${groupName}" видалено`, "success");
    renderStudents();
  } catch (ex) {
    toast(ex.message || "Помилка видалення групи", "error");
  }
}

async function showEditLessonModal(teacherId, lessonId) {
  showModal(
    "modal-edit-lesson",
    `
    <div class="modal-header">
      <span class="modal-title">Редагувати урок</span>
      <button class="modal-close" onclick="closeModal('modal-edit-lesson')">✕</button>
    </div>
    <div class="loading-wrap"><div class="spinner"></div></div>
  `,
    true,
  );

  const data = await api.getLesson(lessonId).catch(() => null);
  if (!data) {
    toast("Урок не знайдено", "error");
    closeModal("modal-edit-lesson");
    return;
  }
  const l = data.lesson || data;

  document.querySelector("#modal-edit-lesson .modal").innerHTML = `
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
  const err = document.getElementById("edit-lesson-err");
  try {
    await api.updateLesson(teacherId, lessonId, {
      title: document.getElementById("edit-lesson-title").value.trim(),
      material: document.getElementById("edit-lesson-material").value.trim(),
      date: document.getElementById("edit-lesson-date").value.trim(),
      time: document.getElementById("edit-lesson-time").value.trim(),
      homeworkLink: document.getElementById("edit-lesson-hw").value.trim(),
    });
    closeModal("modal-edit-lesson");
    toast("Урок оновлено", "success");
    renderContent();
  } catch (ex) {
    err.textContent = ex.message;
    err.classList.remove("hidden");
  }
}
