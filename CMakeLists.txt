cmake_minimum_required(VERSION 3.16)

project(tp2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(apptp2
    main.cpp
    AppState.cpp
    AppSensor.cpp
    AppSensorList.cpp
    OscilloscopeItem.cpp

    AppState.hpp
    AppSensor.hpp
    AppSensorList.hpp
    OscilloscopeItem.hpp
)

qt_add_qml_module(apptp2
    URI tp2
    VERSION 1.0
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptp2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptp2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptp2
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS apptp2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
