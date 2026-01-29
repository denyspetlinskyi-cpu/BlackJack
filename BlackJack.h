#pragma once
#include <iostream>
#include "Window.h"
#include "Game.h"
#include <thread>
class BlackJack
{
private:
	Window window;
	Game game;
public:
	bool IsDoubleDownAllowed = false;
	bool NaturalBlackjackpayout32 = false;
	bool PerfectPairsSideBet = false;
	BlackJack();
	~BlackJack();
	void startGame();
	void roundLoop();
	int InputBetAmount();
	int InputAction();
	void ShowDealerHand(std::vector<Card> DealerHand, bool HideSecondCard);
	void ShowPlayerHands(std::vector<std::vector<Card>> PlayerHands, int SelectedHand, std::vector<std::vector<int>> HandsValue);
	void ShowBalance(int balance);
};

