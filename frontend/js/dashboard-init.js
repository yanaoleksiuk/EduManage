function handleLogout() {
  sessionStorage.removeItem("edu_user");
  window.location.href = "index.html";
}

document.addEventListener("DOMContentLoaded", () => {
  const userData = sessionStorage.getItem("edu_user");
  if (!userData) {
    window.location.href = "index.html";
    return;
  }
  App.user = JSON.parse(userData);
  initTheme();
  renderSidebar();
  renderBreadcrumbs();
  renderContent();
});
