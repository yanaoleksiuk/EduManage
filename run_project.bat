@echo off
setlocal

call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%

if not exist build mkdir build
if exist build\EduManage.exe del /q build\EduManage.exe

cl /std:c++17 /EHsc /nologo /Febuild\EduManage.exe /Fobuild\ /Fdbuild\EduManage.pdb ^
    main.cpp ^
    services\AuthService.cpp ^
    services\CourseService.cpp ^
    storage\StudentStorage.cpp ^
    storage\TeacherStorage.cpp ^
    ui\ConsoleApp.cpp ^
    /I .
if errorlevel 1 exit /b %errorlevel% 
if not exist build\EduManage.exe exit /b 1

build\EduManage.exe
