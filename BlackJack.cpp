#include "BlackJack.h"


const std::string rank[] = {
	"2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K", "A"
};

const std::string master[] = {
	"Hearts", "Diamonds", "Clubs", "Spades"
};


BlackJack::BlackJack()
{
}

BlackJack::~BlackJack()
{
}

void BlackJack::startGame()
{
	//window.show();
	game.shuffleDeck();
	while(game.money > 0) {
		roundLoop();
	}
}

void BlackJack::roundLoop(){
	ShowBalance(game.GetBalance());
	int bet = InputBetAmount();
	if (game.GetBet(bet)) {
		game.shuffleDeck();
		game.DealInitialHands();

		std::vector<Card> dealerHand = game.GetDealerHand();
		ShowDealerHand(dealerHand, true);
		int SelectedHand = 0;
		do {
			std::vector<std::vector<Card>> playerHands = game.GetPlayerHands();
			std::vector<std::vector<int>> handsValue = game.GetHandsValue();
			ShowPlayerHands(playerHands, SelectedHand, handsValue);
			int action = InputAction();
			switch (action)
			{
				case 1:
					game.hit();
					break;
				case 2:
					game.stand();
					break;
				case 3:
					if(IsDoubleDownAllowed)
						game.doubleDown();
					break;
				case 4:
					game.split(PerfectPairsSideBet);
					break;
			}
			SelectedHand = game.GetSelectedHand();
		} while (SelectedHand < game.PlayerHands.size());

		do {
			dealerHand = game.GetDealerHand();
			ShowDealerHand(dealerHand, false);
			std::cout << game.GetDealerHandValue() << std::endl;
		} while (game.DealerTakeCards());

		game.CalculateResults(NaturalBlackjackpayout32);
	}
}

int BlackJack::InputBetAmount()
{
	int betAmount;
	std::cin >> betAmount;
	return betAmount;
}

int BlackJack::InputAction()
{
	int action;
	std::cin >> action;
	return action;
}

void BlackJack::ShowDealerHand(std::vector<Card> DealerHand, bool HideSecondCard)
{
	std::cout << "Dealer hand: ";
	std::cout << "{";
	if (!HideSecondCard) {
		for (const auto& card : DealerHand) {
			std::cout << rank[card.value] << " " << master[card.suit] << ", ";
		}
	}
	else {
		std::cout << rank[DealerHand[0].value] << " " << master[DealerHand[0].suit] << ", ";
		std::cout << "Hidden Card";
	}
	std::cout << "}" << std::endl;
}

void BlackJack::ShowPlayerHands(std::vector<std::vector<Card>> PlayerHands, int SelectedHand, std::vector<std::vector<int>> HandsValue){
	std::cout << "Selected Hand: " << game.selectedHand << std::endl;
	std::cout << "Your hands: " << std::endl;
	for (int hand = 0; hand < PlayerHands.size(); hand++) {
		std::cout << "{";
		for (const auto& card : PlayerHands[hand]) {
			std::cout << rank[card.value] << " " << master[card.suit] << ", ";
		}
		std::cout << "} Values: ";
		for (const auto& value : HandsValue[hand]) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
}

void BlackJack::ShowBalance(int balance){
	std::cout << "Your money: " << balance << std::endl;
}
