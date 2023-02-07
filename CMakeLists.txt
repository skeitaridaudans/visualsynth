cmake_minimum_required(VERSION 3.16)

project(QtQuickTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/home/gilli/Qt/6.4.2/gcc_64")

if("${CMAKE_SYSTEM}" MATCHES "Linux")
	message(STATUS ">>> Linux")
	set(dir ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
	set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
	set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
	set(CMAKE_BUILD_FILES_DIRECTORY ${dir})
	set(CMAKE_BUILD_DIRECTORY ${dir})
	set(CMAKE_BINARY_DIR  ${dir})
	SET(EXECUTABLE_OUTPUT_PATH ${dir})
	SET(LIBRARY_OUTPUT_PATH ${dir}lib)
	SET(CMAKE_CACHEFILE_DIR ${dir})
else()
	set(CMAKE_PREFIX_PATH "C:\\Qt\\6.4.2\\mingw_64")
endif()


find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(OpenGL REQUIRED)

qt_add_executable(appQtQuickTest
		src/main.cpp
		src/main.h src/SinWaveView/SinWaveRenderer.cpp src/SinWaveView/SinWaveRenderer.h src/SinWaveView/SinWaveItem.cpp src/SinWaveView/SinWaveItem.h src/BoxView/BoxView.cpp src/BoxView/BoxView.h src/BoxView/BoxViewRenderer.cpp src/BoxView/BoxViewRenderer.h src/BoxView/Box.cpp src/BoxView/Box.h src/BoxView/NewBox.h src/BoxView/NewBox.cpp)

qt_add_qml_module(appQtQuickTest
    URI QtQuickTest
    VERSION 1.0
    QML_FILES src/main.qml
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
    BUNDLE DESTINATION ${dir}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
