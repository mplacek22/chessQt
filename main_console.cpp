#include "consolegameui.h"
#include "Game.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    auto game = std::make_unique<Game>();
    auto ui = std::make_shared<ConsoleGameUI>(*game);  // UI owns itself

    game->setObserver(ui);   // Game holds a weak_ptr — no cycle
    ui->run();
    return 0;
}