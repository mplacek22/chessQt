#include "consolegameui.h"
#include "Game.h"
#include "game_mediator.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Game game;
    ConsoleGameUI ui;
    GameMediator mediator(&game, &ui);

    ui.run();

    return 0;
}