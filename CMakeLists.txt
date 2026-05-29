cmake_minimum_required(VERSION 3.21.1)
project(chess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Shared core logic (no Qt) ───────────────────────────────────────────────
add_library(chess_core STATIC
    piece.h       piece.cpp
    board.h       board.cpp
    game.h        game.cpp
    move.h
    coordinate.h
    bishop.h      bishop.cpp
    king.h        king.cpp
    knight.h      knight.cpp
    pawn.h        pawn.cpp
    queen.h       queen.cpp
    rook.h        rook.cpp
    Color.h
    MoveType.h
    pieceType.h
    gameStatus.h
    pieceUtils.h
    grid.h
    game_state.h
    drawCause.h
    move_generator.h  move_generator.cpp
    IGameObserver.h
    algebraicNotationTranslator.h  algebraicNotationTranslator.cpp
    InvalidSANException.h
)
target_include_directories(chess_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ─── GUI target ───────────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appchess
    main.cpp
)

qt_add_qml_module(appchess
    URI chess
    QML_FILES
        qml/Main.qml
        qml/Board.qml
        qml/MoveList.qml
        qml/Piece.qml
        qml/PromotionDialog.qml
        qml/PieceButton.qml
        qml/GameOverDialog.qml
    SOURCES


        boardmodel.h      boardmodel.cpp
        gamecontroller.h  gamecontroller.cpp
        chessEnums.h
    RESOURCES
        qml/pieces/w_bishop.svg
        qml/pieces/w_king.svg
        qml/pieces/w_knight.svg
        qml/pieces/w_pawn.svg
        qml/pieces/w_queen.svg
        qml/pieces/w_rook.svg
        qml/pieces/b_bishop.svg
        qml/pieces/b_king.svg
        qml/pieces/b_knight.svg
        qml/pieces/b_pawn.svg
        qml/pieces/b_queen.svg
        qml/pieces/b_rook.svg
)

target_link_libraries(appchess PRIVATE chess_core Qt6::Quick)

set_target_properties(appchess PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ─── Console target ───────────────────────────────────────────────────────────
add_executable(appchess_console
    main_console.cpp
    consolegameui.h  consolegameui.cpp
)

target_link_libraries(appchess_console PRIVATE chess_core)

# ─── Install ──────────────────────────────────────────────────────────────────
install(TARGETS appchess appchess_console
    BUNDLE  DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)