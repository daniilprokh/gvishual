set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Qt5_DIR "D:/Soft/Programming/Qt/Qt5.14.2/5.14.2/msvc2017_64/lib/cmake/Qt5")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)

set(MAIN_SOURCES
    main.cpp
)

set(HEADERS
    edgewidget.h
    graphcreator.h
    graphwidget.h
)

set(SOURCES
    edgewidget.cpp
    graphcreator.cpp
    graphwidget.cpp
)

qt5_wrap_ui(UI_FILES
    edgewidget.ui
    graphcreator.ui
    graphwidget.ui
)

qt5_add_resources(QRC_WRAPPERS resources/res.qrc OPTIONS -no-compress)

add_executable(${PROJECT_NAME}
    ${MAIN_SOURCES} ${SOURCES} ${HEADERS}
    ${UI_FILES} ${QRC_WRAPPERS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets)

install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if(WIN32)
    install(DIRECTORY "${Qt5_DIR}/../../../plugins/platforms" DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()