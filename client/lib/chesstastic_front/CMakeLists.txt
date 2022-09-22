# https://github.com/RoboTeamTwente/roboteam_suite/blob/development/cmake_modules/LocateQt5.cmake
# Authored by Dawid Kulikowski (member of this group) for RoboTeam Twente and reused here

if (APPLE)
    if (EXISTS /usr/local/opt/qt@5)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt@5")
    elseif (EXISTS /usr/local/opt/qt5)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
    else ()
        message(FATAL_ERROR "Could not find Qt5. Please install with `brew install qt@5`.")
    endif ()
elseif (UNIX AND NOT APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt5")
else ()
    message(FATAL_ERROR "Can't auto-locate Qt5 for this platform")
endif ()

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.10.4)

set(HTTPLIB_REQUIRE_OPENSSL on)
set(OPENSSL_USE_STATIC_LIBS on)
if (APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@1.1/1.1.1l_1)
endif ()
FetchContent_Declare(cpp-httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
        GIT_TAG v0.9.7)
FetchContent_MakeAvailable(cpp-httplib json)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

add_library(chesstastic_lib
        src/widgets/ChessBoardWidget.cpp
        src/renderers/ChessBoardRenderer.cpp
        src/camera/CameraFeedback.cpp
        src/camera/CameraHandler.cpp
        src/widgets/CameraCalibratorWidget.cpp
        src/models/CameraDetectionSettings.cpp
        src/widgets/CameraImageRenderer.cpp
        src/widgets/CameraDataSetGenerator.cpp
        src/widgets/ChesstasticWindow.cpp
        src/widgets/ChesstasticSettingsWindow.cpp
        src/networking/ChesstasticAPIService.cpp
        src/chess/ChessGame.cpp
        src/networking/ChesstasticAPIMiddleman.cpp
        src/networking/ChesstasticAPILinkingMiddleman.cpp
        src/widgets/ChesstasticLinkWidget.cpp
        src/networking/NetworkingEndpointDataModels.cpp
        src/widgets/ChessStatusWidget.cpp
        src/widgets/ChessPlayerStatusWidget.cpp
        src/camera/CameraLogger.cpp
        src/chess/ChessMovePlausibilityFilter.cpp

        # Headers for AUTOMOC
        include/chesstastic_lib/ChessBoardWidget.h
        include/chesstastic_lib/CameraCalibratorWidget.h
        include/chesstastic_lib/CameraDetectionSettings.h
        include/chesstastic_lib/CameraImageRenderer.h
        include/chesstastic_lib/CameraDataSetGenerator.h
        include/chesstastic_lib/ChesstasticWindow.h
        include/chesstastic_lib/ChesstasticSettingsWindow.h
        include/chesstastic_lib/NetworkingEndpointDataModels.h
        include/chesstastic_lib/ChessGame.h
        include/chesstastic_lib/ChesstasticAPIMiddleman.h
        include/chesstastic_lib/ChesstasticAPILinkingMiddleman.h
        include/chesstastic_lib/ChesstasticLinkWidget.h
        include/chesstastic_lib/ChessStatusWidget.h
        include/chesstastic_lib/ChessPlayerStatusWidget.h)

target_include_directories(chesstastic_lib
        PRIVATE include/chesstastic_lib
        PRIVATE ${HeaderFileList}
        INTERFACE include/
        )

target_link_libraries(chesstastic_lib PUBLIC Qt5::Widgets ${OpenCV_LIBS} Threads::Threads chesstastic_chess_lib nlohmann_json::nlohmann_json httplib)
