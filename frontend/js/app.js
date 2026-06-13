document.addEventListener("DOMContentLoaded", () => {
  if (sessionStorage.getItem("edu_user")) {
    window.location.href = "dashboard.html";
  }
});

function openModal(id) {
  document.getElementById(id).classList.add("open");
}

function closeModal(id) {
  document.getElementById(id).classList.remove("open");
  clearErrors();
}

function switchModal(from, to) {
  closeModal(from);
  setTimeout(() => openModal(to), 180);
}

document.addEventListener("click", (e) => {
  if (e.target.classList.contains("modal-overlay")) {
    e.target.classList.remove("open");
    clearErrors();
  }
});

let selectedRole = "student";

function selectRole(role) {
  selectedRole = role;
  document.querySelectorAll(".role-option").forEach((el) => {
    el.classList.toggle("active", el.dataset.role === role);
  });
}

function showError(elId, msg) {
  const el = document.getElementById(elId);
  if (el) {
    el.textContent = msg;
    el.classList.remove("hidden");
  }
}

function clearErrors() {
  document
    .querySelectorAll(".form-error")
    .forEach((el) => el.classList.add("hidden"));
}

async function handleLogin(e) {
  e.preventDefault();
  clearErrors();
  const email = document.getElementById("login-email").value.trim();
  const password = document.getElementById("login-password").value;
  const btn = document.getElementById("login-btn");

  btn.disabled = true;
  btn.textContent = "Вхід…";

  try {
    const data = await api.login(email, password);
    const parts = (data.fullName || "").split(" ");
    sessionStorage.setItem(
      "edu_user",
      JSON.stringify({
        id: data.userId,
        role: data.role,
        firstName: parts[0] || "",
        lastName: parts.slice(1).join(" ") || "",
        email,
      }),
    );
    window.location.href = "dashboard.html";
  } catch (err) {
    showError("login-error", err.message || "Невірний email або пароль");
    btn.disabled = false;
    btn.textContent = "Увійти";
  }
}

async function handleRegister(e) {
  e.preventDefault();
  clearErrors();

  const firstName = document.getElementById("reg-first-name").value.trim();
  const lastName = document.getElementById("reg-last-name").value.trim();
  const email = document.getElementById("reg-email").value.trim();
  const password = document.getElementById("reg-password").value;
  const btn = document.getElementById("reg-btn");

  if (!firstName || !lastName) {
    showError("register-error", "Введіть ім'я та прізвище");
    return;
  }

  btn.disabled = true;
  btn.textContent = "Реєстрація…";

  try {
    const data = await api.register(
      selectedRole,
      firstName,
      lastName,
      email,
      password,
    );
    const parts = (data.fullName || `${firstName} ${lastName}`).split(" ");
    sessionStorage.setItem(
      "edu_user",
      JSON.stringify({
        id: data.userId,
        role: data.role,
        firstName: parts[0] || firstName,
        lastName: parts.slice(1).join(" ") || lastName,
        email,
      }),
    );
    window.location.href = "dashboard.html";
  } catch (err) {
    showError("register-error", err.message || "Помилка реєстрації");
    btn.disabled = false;
    btn.textContent = "Зареєструватись";
  }
}
