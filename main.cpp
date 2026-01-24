#include "BlackJack.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	BlackJack blackjack;
    //Window window;
    //window.show();
	blackjack.startGame();
    return app.exec();
}
