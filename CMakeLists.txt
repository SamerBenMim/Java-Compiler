cmake_minimum_required(VERSION 3.5)

project(SSA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

# Core Module Configuration START

set(LEX flex)

set(YACC bison)

set(CORE_DIR ${CMAKE_SOURCE_DIR}/SSA-core)

include_directories( SSA-core )

link_directories( SSA-core )

add_custom_command(OUTPUT ${CORE_DIR}/java_parser.cc ${CORE_DIR}/java_parser.hh ${CORE_DIR}/location.hh
        COMMAND ${YACC}  -r cex -o java_parser.cc java_parser.yy
        WORKING_DIRECTORY "${CORE_DIR}"
        COMMENT "Generate java parser"
        DEPENDS ${CORE_DIR}/java_parser.yy)

add_custom_command(OUTPUT ${CORE_DIR}/java_lexer.cc
        COMMAND ${LEX} -o java_lexer.cc java_lexer.ll
        WORKING_DIRECTORY "${CORE_DIR}"
        COMMENT "Generate java lexer"
        DEPENDS ${CORE_DIR}/java_lexer.ll)

set(CORE_SOURCES
        SSA-core/java_lexer.cc SSA-core/java_lexer.hh
        SSA-core/java_parser.cc SSA-core/java_parser.hh
        SSA-core/driver.cc SSA-core/driver.hh
        SSA-core/java_semantics.cc SSA-core/java_semantics.hh
        SSA-core/utils.hh
        SSA-core/location.hh
        SSA-core/java_lexer.ll
        SSA-core/java_parser.yy
        SSA-core/code_generator.cc SSA-core/code_generator.hh SSA-core/opcode.hh SSA-core/opcode.cpp SSA-core/instruction.cc SSA-core/instruction.hh)


# Core Module Configuration END

set(TS_FILES SSA-gui/SSA_fr_FR.ts)

set(GUI_SOURCES
        SSA-gui/main.cpp
        SSA-gui/mainwindow.h SSA-gui/mainwindow.cpp SSA-gui/menubar.cpp
        SSA-gui/maindocument.h SSA-gui/maindocument.cpp
        SSA-gui/sidebar.h SSA-gui/sidebar.cpp
        SSA-gui/tabholder.h SSA-gui/tabholder.cpp
        SSA-gui/bottombar.h SSA-gui/bottombar.cpp
        SSA-gui/compileoutput.h SSA-gui/compileoutput.cpp
        ${TS_FILES}
        SSA-gui/syntax/java_highlighter.cpp SSA-gui/syntax/java_highlighter.h
        SSA-gui/error-handling/gui_error_handler.cc SSA-gui/error-handling/gui_error_handler.hh
        SSA-gui/linenumberarea.cpp SSA-gui/linenumberarea.h)

set(GUI_RESOURCES ${PROJECT_SOURCE_DIR}/SSA-gui/syntax/keywords ${PROJECT_SOURCE_DIR}/SSA-gui/syntax/defaultClasses ${PROJECT_SOURCE_DIR}/SSA-gui/syntax/defaultInterfaces)

set(PROJECT_SOURCES
        ${GUI_SOURCES}
        ${CORE_SOURCES}
)

# Copy Resources files to binary file directory
file(COPY ${GUI_RESOURCES} DESTINATION ${PROJECT_BINARY_DIR})


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SSA
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SSA APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(SSA SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SSA
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(SSA PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(SSA PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SSA)
endif()
