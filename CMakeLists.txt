cmake_minimum_required(VERSION 3.16)

project(QtQuickTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.4.2\\mingw_64")

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(OpenGL REQUIRED)

qt_add_executable(appQtQuickTest
    main.cpp
        main.h)

qt_add_qml_module(appQtQuickTest
    URI QtQuickTest
    VERSION 1.0
    QML_FILES main.qml 
)

set_target_properties(appQtQuickTest PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtQuickTest
    PRIVATE Qt6::Quick)

target_link_libraries(appQtQuickTest PRIVATE OpenGL::GL)
target_link_libraries(appQtQuickTest PRIVATE OpenGL::GLU)

install(TARGETS appQtQuickTest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
