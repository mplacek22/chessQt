cmake_minimum_required (VERSION 3.21.1)

project(chess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appchess
    main.cpp
)

qt_add_qml_module(appchess
    URI chess
    QML_FILES
    qml/Board.qml
    SOURCES
        chessmodel.h
        chessmodel.cpp
        QML_FILES qml/Main.qml
        QML_FILES qml/MoveList.qml
        QML_FILES qml/Piece.qml
        SOURCES
        SOURCES chessboard.h chessboard.cpp
        SOURCES piece.h piece.cpp
        SOURCES board.h board.cpp
        SOURCES game.h game.cpp
        SOURCES move.h move.cpp
        SOURCES coordinate.h
        SOURCES bishop.h bishop.cpp
        SOURCES king.h king.cpp
        SOURCES knight.h knight.cpp
        SOURCES pawn.h pawn.cpp
        SOURCES queen.h queen.cpp
        SOURCES rook.h rook.cpp
        SOURCES Color.h
        SOURCES MoveType.h
        SOURCES pieceType.h
        SOURCES gameStatus.h
        SOURCES boardmodel.h boardmodel.cpp
        SOURCES gamecontroller.h gamecontroller.cpp
        QML_FILES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appchess PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appchess
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appchess
    PRIVATE Qt6::Quick
)

install(TARGETS appchess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
