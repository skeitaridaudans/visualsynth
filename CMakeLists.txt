cmake_minimum_required(VERSION 3.16)

project(VisualSynth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Intersynth
set(ENABLE_INTERSYNTH FALSE)
if (ENABLE_INTERSYNTH)
	add_definitions(-DUSE_INTERSYNTH)
endif()



# Linux support
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

add_subdirectory(./lib/QtAwesome QtAwesome)

include_directories(./lib/json)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_add_executable(appVisualSynth
		src/main.cpp
		src/main.h src/OperatorView/OperatorView.cpp src/OperatorView/OperatorView.h src/OperatorView/OperatorDrawer.cpp src/OperatorView/OperatorDrawer.h src/OperatorView/AddOperatorBox.h src/OperatorView/AddOperatorBox.cpp src/Controller/Controller.h src/Controller/Controller.cpp src/Controller/Operator.cpp src/Controller/Operator.h src/Controller/Api.cpp src/Controller/Api.h src/Controller/Intersynth/floating_fix.h src/Controller/Intersynth/floating_fix.cpp src/Controller/Intersynth/intersynth.h src/Controller/Intersynth/intersynth.cpp src/GlUtils/Utils.cpp src/GlUtils/Utils.h src/Utils/Utils.h src/Alert/AlertController.cpp src/Alert/AlertController.h src/FontAwesome.h src/OperatorView/DeleteOperatorBox.h src/OperatorView/DeleteOperatorBox.cpp src/FontAwesome.cpp src/AmpEnvelope/AmpGraph/AmpEnvGraphView.cpp src/AmpEnvelope/AmpGraph/AmpEnvParams/AmpEnvParams.cpp src/AmpEnvelope/AmpGraph/AmpEnvParams/AmpEnvParams.h src/AmpEnvelope/AmpGraph/LineBetweenParams/Line.cpp src/AmpEnvelope/AmpGraph/LineBetweenParams/Line.h src/lovecommunicationtcp/lovecommunicationtcp.cpp src/lovecommunicationtcp/lovecommunicationtcp.h lib/json/json.hpp src/OperatorPresetsView/OperatorPresetsView.cpp src/OperatorPresetsView/OperatorPresetsView.h src/OperatorPresetsView/OperatorPresetView.cpp src/OperatorPresetsView/OperatorPresetView.h src/Utils/TweenAnimation.cpp src/Utils/TweenAnimation.h src/Utils/ColorTweenAnimation.cpp src/Utils/ColorTweenAnimation.h src/Dialog/DialogController.cpp src/Dialog/DialogController.h src/Utils/TouchPoint.h src/Utils/PointTweenAnimation.cpp src/Utils/PointTweenAnimation.h src/Controller/Preset.h src/Controller/AmpEnvValue.h src/OutputWaveView/OutputWaveView.cpp src/OutputWaveView/OutputWaveView.h src/Controller/LfoWaveType.h src/OperatorWaveView/OperatorWaveView.h src/OperatorWaveView/OperatorWaveView.cpp src/OperatorView/PresetButton.cpp src/OperatorView/PresetButton.h)

qt_add_qml_module(appVisualSynth
    URI VisualSynth
    VERSION 1.0
    QML_FILES src/main.qml src/ToggleButton.qml src/TextInputDialog.qml
		src/ConnectionInfo.qml
)

set_target_properties(appVisualSynth PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appVisualSynth
    PRIVATE Qt6::Quick)

target_link_libraries(appVisualSynth PRIVATE OpenGL::GL)
target_link_libraries(appVisualSynth PRIVATE OpenGL::GLU)
target_link_libraries(appVisualSynth PRIVATE QtAwesome)
target_link_libraries(appVisualSynth PRIVATE Qt6::Network)

if(ENABLE_INTERSYNTH)
target_link_libraries(appVisualSynth PRIVATE rtmidi)
endif()

install(TARGETS appVisualSynth
    BUNDLE DESTINATION ${dir}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
