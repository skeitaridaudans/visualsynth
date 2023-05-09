cmake_minimum_required(VERSION 3.16)

project(VisualSynth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ANDROID)
	add_definitions(-DANDROID)
endif ()


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

else()
	set(CMAKE_PREFIX_PATH "C:\\Qt\\6.4.2\\mingw_64")
endif()

add_subdirectory(./lib/QtAwesome QtAwesome)

include_directories(./lib/json)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_add_executable(appVisualSynth
		src/main.cpp
		src/main.h src/Views/OperatorView/OperatorView.cpp src/Views/OperatorView/OperatorView.h src/Views/OperatorView/OperatorDrawer.cpp src/Views/OperatorView/OperatorDrawer.h
		src/Views/OperatorView/AddOperatorBox.h src/Views/OperatorView/AddOperatorBox.cpp src/Controllers/Controller/Controller.h src/Controllers/Controller/Controller.cpp src/Controllers/Controller/Operator.cpp
		src/Controllers/Controller/Operator.h src/Controllers/Controller/Api.cpp src/Controllers/Controller/Api.h src/Utils/Utils.h src/Controllers/Alert/AlertController.cpp src/Controllers/Alert/AlertController.h
		src/Utils/FontAwesome.h src/Views/OperatorView/DeleteOperatorBox.h src/Views/OperatorView/DeleteOperatorBox.cpp src/Utils/FontAwesome.cpp
		src/Views/AmpEnvelope/AmpGraph/AmpEnvGraphView.h src/Views/AmpEnvelope/AmpGraph/AmpEnvGraphView.cpp lib/LoveCommunicationTcp/LoveCommunicationTcp.cpp lib/LoveCommunicationTcp/LoveCommunicationTcp.h
		lib/LoveCommunicationTcp/floating_fix.cpp lib/LoveCommunicationTcp/floating_fix.h lib/json/json.hpp src/Views/OperatorPresetsView/OperatorPresetsView.cpp
		src/Views/OperatorPresetsView/OperatorPresetsView.h src/Views/OperatorPresetsView/OperatorPresetView.cpp src/Views/OperatorPresetsView/OperatorPresetView.h
		src/Utils/Animations/TweenAnimation.cpp src/Utils/Animations/TweenAnimation.h src/Utils/Animations/ColorTweenAnimation.cpp
		src/Utils/Animations/ColorTweenAnimation.h src/Controllers/Dialog/DialogController.cpp src/Controllers/Dialog/DialogController.h src/Utils/TouchPoint.h
		src/Utils/Animations/PointTweenAnimation.cpp src/Utils/Animations/PointTweenAnimation.h src/Controllers/Controller/Preset.h src/Controllers/Controller/AmpEnvValue.h
		src/Views/OutputWaveView/OutputWaveView.cpp src/Views/OutputWaveView/OutputWaveView.h src/Controllers/Controller/LfoWaveType.h src/Views/OperatorWaveView/OperatorWaveView.h src/Views/OperatorWaveView/OperatorWaveView.cpp
		src/Views/AmpEnvelope/AmpGraph/DraggingTouchPoint.h src/Views/OperatorView/PresetButton.cpp src/Views/OperatorView/PresetButton.h
)

qt_add_qml_module(appVisualSynth
    URI VisualSynth
    VERSION 1.0
    QML_FILES src/main.qml src/UiComponents/Core/ToggleButton.qml src/UiComponents/Sections/TextInputDialog.qml
		src/UiComponents/Sections/ConnectionSection.qml src/UiComponents/Sections/BottomAlertView.qml src/UiComponents/Sections/GlobalLfoOptionsSection.qml
		src/UiComponents/Sections/OperatorInfoSection.qml src/UiComponents/Sections/OutputWaveSection.qml src/UiComponents/Sections/OperatorViewSection.qml
		src/UiComponents/Sections/AmpEnvSection.qml src/UiComponents/Core/CustomBorder.qml
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

target_link_libraries(appVisualSynth PRIVATE QtAwesome)
target_link_libraries(appVisualSynth PRIVATE Qt6::Network)

install(TARGETS appVisualSynth
    BUNDLE DESTINATION ${dir}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
