cmake_minimum_required(VERSION 3.16)

project(QtQuickTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENABLE_INTERSYNTH FALSE)

if (ENABLE_INTERSYNTH)
	add_definitions(-DUSE_INTERSYNTH)
endif()


if("${CMAKE_SYSTEM}" MATCHES "Linux")
	message(STATUS ">>> Linux")

	if (EXISTS /home/gilli/Qt)
		set(CMAKE_PREFIX_PATH "/home/gilli/Qt/6.4.2/gcc_64")

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
		set(CMAKE_THREAD_LIBS_INIT "-lpthread")

		set(CMAKE_PREFIX_PATH "/opt/Qt/6.4.2/gcc_64")
	endif()

	# To build with intersynth
	if (ENABLE_INTERSYNTH)
		SET(GCC_COVERAGE_LINK_FLAGS    "-lrtmidi")
		set(CMAKE_HAVE_THREADS_LIBRARY 1)
		set(CMAKE_USE_WIN32_THREADS_INIT 0)
		set(CMAKE_USE_PTHREADS_INIT 1)
		set(THREADS_PREFER_PTHREAD_FLAG ON)
	endif()
else()
	set(CMAKE_PREFIX_PATH "C:\\Qt\\6.4.2\\mingw_64")
endif()


find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(OpenGL REQUIRED)

qt_add_executable(appQtQuickTest
		src/main.cpp
		src/main.h src/SinWaveView/SinWaveRenderer.cpp src/SinWaveView/SinWaveRenderer.h src/SinWaveView/SinWaveItem.cpp src/SinWaveView/SinWaveItem.h src/BoxView/BoxView.cpp src/BoxView/BoxView.h src/BoxView/BoxViewRenderer.cpp src/BoxView/BoxViewRenderer.h src/BoxView/OperatorDrawer.cpp src/BoxView/OperatorDrawer.h src/BoxView/NewBox.h src/BoxView/NewBox.cpp src/Controller/Controller.h src/Controller/Controller.cpp src/Controller/Operator.cpp src/Controller/Operator.h src/Controller/Api.cpp src/Controller/Api.h src/Controller/Intersynth/floating_fix.h src/Controller/Intersynth/floating_fix.cpp src/Controller/Intersynth/intersynth.h src/Controller/Intersynth/intersynth.cpp)

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

if(ENABLE_INTERSYNTH)
target_link_libraries(appQtQuickTest PRIVATE rtmidi)
endif()

install(TARGETS appQtQuickTest
    BUNDLE DESTINATION ${dir}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
