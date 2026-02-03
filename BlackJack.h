#pragma once
#include <iostream>
#include "Game.h"
#include <thread>
#include <algorithm>
#include <random>
#include <chrono>
#include <mutex>
#include <atomic>
#include <QString>

class Window;

class BlackJack
{
private:
	std::unique_ptr<Window> window;
	Game game;
	std::thread gameThread;
	std::mutex gameMutex;
	std::atomic<int> gameState;
	int currentAction;

public:
	bool IsDoubleDownAllowed = true;
	bool NaturalBlackjackpayout32 = false;
	bool PerfectPairsSideBet = true;

	BlackJack();
	~BlackJack();
	void startGame();
	void gameLoop();
	void roundLoop();

	void PlaceBet(int betAmount);
	void Hit();
	void Stand();
	void DoubleDown();
	void Split();

	void ShowDealerHand(std::vector<Card> DealerHand, bool HideSecondCard);
	void ShowPlayerHands(std::vector<std::vector<Card>> PlayerHands);
	void ShowBalance(int balance);
	void ShowGameOver(QString message);
	void SetSeed();
	void SetCustomSeed(int seed);
};

