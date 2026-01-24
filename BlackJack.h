#pragma once
#include "Window.h"
#include "Game.h"
class BlackJack
{
private:
	Window window;
	Game game;
public:
	BlackJack();
	~BlackJack();
	void startGame();
};

