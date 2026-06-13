# EduManage

Веб-платформа для управління навчальним процесом з розмежуванням ролей викладача та студента.

## Стек технологій

| Шар | Технологія |
|---|---|
| Бекенд | C++17, [Crow](https://crowcpp.org/) REST API |
| Фронтенд | HTML5, CSS3 (Custom Properties), Vanilla JS |
| Зберігання даних | Бінарні файли через шаблонний клас `BinaryStorage<T>` |
| Збірка | CMake 3.16+ (FetchContent для залежностей) |

## Функціонал

### Роль: Викладач
- Створення та редагування курсів, тем і уроків
- Складання тестів із довільною кількістю варіантів відповіді
- Управління студентами та групами
- Зарахування студентів/груп на курси
- Перегляд прогресу кожного студента
- Розклад занять

### Роль: Студент
- Перегляд курсів та навчальних матеріалів
- Проходження тестів із автоматичною перевіркою
- Відстеження особистого прогресу та досягнень
- Персональний розклад

## Запуск проекту

### Вимоги
- CMake ≥ 3.16
- Компілятор з підтримкою C++17 (MSVC / GCC / Clang)
- Підключення до інтернету для першого завантаження залежностей (Crow, Asio)

### Збірка

mkdir build && cd build
cmake ..
cmake --build . --config Release
Запуск
./EduManage        # Linux/macOS
EduManage.exe      # Windows
Сервер запуститься на http://localhost:18080.
Відкрийте браузер і перейдіть за цією адресою.

Структура проекту
EduManage/
├── api/               # Маршрути REST API (Crow)
│   ├── ApiServer.h/cpp
│   ├── AuthRoutes.h
│   ├── CourseRoutes.h
│   ├── GroupRoutes.h
│   ├── TestRoutes.h
│   └── ...
├── services/          # Бізнес-логіка
│   ├── CourseService.h/cpp
│   └── AuthService.h/cpp
├── models/            # Структури даних (Teacher, Student, Course, …)
├── storage/           # BinaryStorage<T> — шаблон бінарного зберігання
├── common/            # Валідатор, константи, ліміти
├── frontend/
│   ├── index.html     # Лендінг, реєстрація, вхід
│   ├── dashboard.html # SPA-дашборд
│   ├── css/main.css
│   └── js/            # Модульний JS (api, core, sections, modals)
└── CMakeLists.txt

API (основні ендпоінти)
POST   /api/auth/register
POST   /api/auth/login
GET    /api/teachers/:id/courses
POST   /api/teachers/:tid/courses
DELETE /api/courses/:id
POST   /api/teachers/:tid/courses/:cid/themes
POST   /api/teachers/:tid/themes/:thid/lessons
POST   /api/teachers/:tid/courses/:cid/tests
GET    /api/tests/:id
POST   /api/students/:sid/tests/:tid/submit
POST   /api/teachers/:tid/courses/:cid/enroll
POST   /api/teachers/:id/groups
DELETE /api/teachers/:tid/groups/:gid
GET    /api/students/:id/progress
GET    /api/students/:id/schedule
Особливості реалізації
BinaryStorage<T> — власний шаблонний клас для зберігання даних у бінарних .dat файлах без зовнішніх СУБД
CourseService — фасад бізнес-логіки, ізолює маршрути від прямої роботи зі сховищем
SPA-навігація — переходи між розділами без перезавантаження сторінки через стек навігації
Теми оформлення — світла/темна тема через CSS Custom Properties, збереження у localStorage
