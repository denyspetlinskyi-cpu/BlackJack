#pragma once
#include "Card.h"
#include <vector>
class Game
{
public:
	int money = 1000;
	int currentBet = 0;
	std::vector<int> bets;
	int selectedHand = 0;
	std::vector<Card> deckStandart{ {Hearts,Two}, {Hearts,Three}, {Hearts,Four}, {Hearts,Five}, {Hearts,Six}, {Hearts,Seven}, {Hearts,Eight}, {Hearts,Nine}, {Hearts,Ten}, {Hearts,Jack}, {Hearts,Queen}, {Hearts,King}, {Hearts,Ace}, {Diamonds,Two}, {Diamonds,Three}, {Diamonds,Four}, {Diamonds,Five}, {Diamonds,Six}, {Diamonds,Seven}, {Diamonds,Eight}, {Diamonds,Nine}, {Diamonds,Ten}, {Diamonds,Jack}, {Diamonds,Queen}, {Diamonds,King}, {Diamonds,Ace},  {Clubs,Two}, {Clubs,Three}, {Clubs,Four}, {Clubs,Five}, {Clubs,Six}, {Clubs,Seven}, {Clubs,Eight}, {Clubs,Nine}, {Clubs,Ten}, {Clubs,Jack}, {Clubs,Queen}, {Clubs,King}, {Clubs,Ace}, {Spades,Two}, {Spades,Three}, {Spades,Four}, {Spades,Five}, {Spades,Six}, {Spades,Seven}, {Spades,Eight}, {Spades,Nine}, {Spades,Ten}, {Spades,Jack}, {Spades,Queen}, {Spades,King}, {Spades,Ace}, {Hearts,Two}, {Hearts,Three}, {Hearts,Four}, {Hearts,Five}, {Hearts,Six}, {Hearts,Seven}, {Hearts,Eight}, {Hearts,Nine}, {Hearts,Ten}, {Hearts,Jack}, {Hearts,Queen}, {Hearts,King}, {Hearts,Ace}, {Diamonds,Two}, {Diamonds,Three}, {Diamonds,Four}, {Diamonds,Five}, {Diamonds,Six}, {Diamonds,Seven}, {Diamonds,Eight}, {Diamonds,Nine}, {Diamonds,Ten}, {Diamonds,Jack}, {Diamonds,Queen}, {Diamonds,King}, {Diamonds,Ace},  {Clubs,Two}, {Clubs,Three}, {Clubs,Four}, {Clubs,Five}, {Clubs,Six}, {Clubs,Seven}, {Clubs,Eight}, {Clubs,Nine}, {Clubs,Ten}, {Clubs,Jack}, {Clubs,Queen}, {Clubs,King}, {Clubs,Ace}, {Spades,Two}, {Spades,Three}, {Spades,Four}, {Spades,Five}, {Spades,Six}, {Spades,Seven}, {Spades,Eight}, {Spades,Nine}, {Spades,Ten}, {Spades,Jack}, {Spades,Queen}, {Spades,King}, {Spades,Ace} ,  {Hearts,Two}, {Hearts,Three}, {Hearts,Four}, {Hearts,Five}, {Hearts,Six}, {Hearts,Seven}, {Hearts,Eight}, {Hearts,Nine}, {Hearts,Ten}, {Hearts,Jack}, {Hearts,Queen}, {Hearts,King}, {Hearts,Ace}, {Diamonds,Two}, {Diamonds,Three}, {Diamonds,Four}, {Diamonds,Five}, {Diamonds,Six}, {Diamonds,Seven}, {Diamonds,Eight}, {Diamonds,Nine}, {Diamonds,Ten}, {Diamonds,Jack}, {Diamonds,Queen}, {Diamonds,King}, {Diamonds,Ace},  {Clubs,Two}, {Clubs,Three}, {Clubs,Four}, {Clubs,Five}, {Clubs,Six}, {Clubs,Seven}, {Clubs,Eight}, {Clubs,Nine}, {Clubs,Ten}, {Clubs,Jack}, {Clubs,Queen}, {Clubs,King}, {Clubs,Ace}, {Spades,Two}, {Spades,Three}, {Spades,Four}, {Spades,Five}, {Spades,Six}, {Spades,Seven}, {Spades,Eight}, {Spades,Nine}, {Spades,Ten}, {Spades,Jack}, {Spades,Queen}, {Spades,King}, {Spades,Ace} };
	std::vector<Card> deck;
	std::vector<Card> DealerHand;
	std::vector<std::vector<Card>> PlayerHands;
	bool canDoubleDown = true;
	bool canPerfectPair = true;
	bool PerfectPairsSideBet = true;
	int seed;

	Game();
	~Game();
	void SetSeed(int newSeed);
	int SizeOfDeck();
	void shuffleDeck();
	void DealInitialHands();
	bool GetBet(int betAmount);
	std::vector<Card> GetDealerHand();
	std::vector<std::vector<Card>> GetPlayerHands();
	std::vector<std::vector<int>> GetHandsValue();
	int GetSelectedHand();
	void stand();
	void hit();
	void doubleDown();
	void split(bool PerfectPairsSideBet);
	int GetDealerHandValue();
	bool DealerTakeCards();
	void CalculateResults(bool NaturalBlackjackpayout32);
	int GetBalance();
};

