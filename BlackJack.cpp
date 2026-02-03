#include "BlackJack.h"
#include "Window.h"

const std::string rank[] = {
	"2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K", "A"
};

const std::string master[] = {
	"Hearts", "Diamonds", "Clubs", "Spades"
};

BlackJack::BlackJack()
{
	window = std::make_unique<Window>();
	gameState = 0;
	currentAction = 0;
}

BlackJack::~BlackJack()
{
	if (gameThread.joinable()) {
		gameThread.join();
	}
}

void BlackJack::startGame()
{
	window->show();
	this->window->blackjack = this;
	SetSeed();
	gameThread = std::thread(&BlackJack::gameLoop, this);
}

void BlackJack::gameLoop()
{
	roundLoop();
}

void BlackJack::roundLoop()
{
	while (game.money > 0) {
		ShowBalance(game.GetBalance());

		gameState = 0;
		while (gameState == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		game.DealInitialHands();
		ShowDealerHand(game.GetDealerHand(), true);
		ShowPlayerHands(game.GetPlayerHands());

		gameState = 1;
		int selectedHand = 0;
		while (selectedHand < game.PlayerHands.size()) {
			currentAction = 0;
			window->ShowHandValues(game.GetHandsValue());

			while (currentAction == 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			switch (currentAction) {
			case 1:
				game.hit();
				break;
			case 2:
				game.stand();
				break;
			case 3:
				game.doubleDown();
				break;
			case 4:
				game.split(PerfectPairsSideBet);
				break;
			}

			ShowPlayerHands(game.GetPlayerHands());
			selectedHand = game.GetSelectedHand();
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}

		gameState = 2;
		while (game.DealerTakeCards()) {
			ShowDealerHand(game.GetDealerHand(), false);
			window->ShowDealerValue(game.GetDealerHandValue());
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		ShowDealerHand(game.GetDealerHand(), false);
		window->ShowDealerValue(game.GetDealerHandValue());
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		game.CalculateResults(NaturalBlackjackpayout32);
		ShowBalance(game.GetBalance());

		gameState = 3;

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

}

void BlackJack::PlaceBet(int betAmount)
{
	std::lock_guard<std::mutex> lock(gameMutex);
	if (game.GetBet(betAmount)) {
		gameState = 1;
	}
}

void BlackJack::Hit()
{
	if (gameState == 1) {
		currentAction = 1;
	}
}

void BlackJack::Stand()
{
	if (gameState == 1) {
		currentAction = 2;
	}
}

void BlackJack::DoubleDown()
{
	if (gameState == 1) {
		currentAction = 3;
	}
}

void BlackJack::Split()
{
	if (gameState == 1) {
		currentAction = 4;
	}
}

void BlackJack::ShowDealerHand(std::vector<Card> DealerHand, bool HideSecondCard)
{
	window->SetDealerHand(DealerHand, HideSecondCard);
}

void BlackJack::ShowPlayerHands(std::vector<std::vector<Card>> PlayerHands)
{
	window->SetPlayerHands(PlayerHands);
}

void BlackJack::ShowBalance(int balance)
{
	window->SetBalance(balance);
}

void BlackJack::ShowGameOver(QString message)
{
	window->ShowGameOver(message);
}

void BlackJack::SetSeed()
{
	srand(time(0));
	game.SetSeed(rand());
}

void BlackJack::SetCustomSeed(int seed)
{
	srand(seed);
	game.SetSeed(seed);
}
