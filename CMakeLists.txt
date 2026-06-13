cmake_minimum_required(VERSION 3.16)
project(EduManage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        v1.2.0
)
FetchContent_GetProperties(crow)
if(NOT crow_POPULATED)
    FetchContent_Populate(crow)
endif()

FetchContent_Declare(asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
)
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()

add_library(Crow INTERFACE)
add_library(Crow::Crow ALIAS Crow)
target_include_directories(Crow INTERFACE
    ${crow_SOURCE_DIR}/include
    ${asio_SOURCE_DIR}/asio/include
)
target_compile_definitions(Crow INTERFACE ASIO_STANDALONE)

FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
endif()

add_library(doctest INTERFACE)
add_library(doctest::doctest ALIAS doctest)
target_include_directories(doctest INTERFACE ${doctest_SOURCE_DIR})

set(COMMON_SOURCES
    services/AuthService.cpp
    services/CourseService.cpp
    storage/UserStorage.cpp
    storage/StudentStorage.cpp
    storage/TeacherStorage.cpp
)

add_executable(EduManage
    main.cpp
    ${COMMON_SOURCES}
    ui/ConsoleApp.cpp
)
target_include_directories(EduManage PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(EduManageAPI
    api_main.cpp
    ${COMMON_SOURCES}
    api/ApiServer.cpp
)
target_include_directories(EduManageAPI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(EduManageAPI PRIVATE Crow::Crow)

if(WIN32)
    target_link_libraries(EduManageAPI PRIVATE ws2_32 mswsock)
endif()

add_executable(EduManageTests
    tests/test_main.cpp
    tests/test_auth.cpp
    tests/test_course.cpp
    ${COMMON_SOURCES}
)
target_include_directories(EduManageTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(EduManageTests PRIVATE doctest::doctest)
