#include "Game.h"
#include <iostream>
Game::Game(){}
Game::~Game(){}

const int rank[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 1};

int Game::SizeOfDeck(){
	return this->deck.size();
}

void Game::shuffleDeck(){}

void Game::DealInitialHands(){
	selectedHand = 0;
	canDoubleDown = true;
	DealerHand.clear();
	DealerHand.push_back(deck.back());
	deck.pop_back();
	DealerHand.push_back(deck.back());
	deck.pop_back();

	PlayerHands.clear();
	PlayerHands.push_back(std::vector<Card>());
	PlayerHands[selectedHand].push_back(deck.back());
	deck.pop_back();
	PlayerHands[selectedHand].push_back(deck.back());
	deck.pop_back();
}

bool Game::GetBet(int betAmount){
	if (betAmount > money || betAmount <= 0)
		return false;

	currentBet = betAmount;
	money -= betAmount;
	bets.push_back(betAmount);
	return true;
}

std::vector<Card> Game::GetDealerHand()
{
	return DealerHand;
}

std::vector<std::vector<Card>> Game::GetPlayerHands()
{
	return PlayerHands;
}



std::vector<std::vector<int>> Game::GetHandsValue(){
	std::vector<std::vector<int>> AllValues = {};
	std::vector<int> Values = {};
	for (int hand = 0; hand < PlayerHands.size(); hand++) {
		int temp = 0;
		for (const auto& card : PlayerHands[hand]) {
			temp += rank[card.value];
		}
		Values.push_back(temp);
		for (const auto& card : PlayerHands[hand]) {

			if (card.value == 12) {
				if (temp + 10 > 21)
					break;
				temp += 10;
				Values.push_back(temp);
				break;
			}
		}
		AllValues.push_back(Values);
		Values.clear();
	}
	return AllValues;
}

int Game::GetSelectedHand()
{
	return selectedHand;
}

void Game::stand(){
	canDoubleDown = false;
	selectedHand++;
}

void Game::hit(){
	canDoubleDown = false;
	PlayerHands[selectedHand].push_back(deck.back());
	deck.pop_back();
}

void Game::doubleDown()
{
	canDoubleDown = false;
	if (currentBet > money) {
		return;
	}
	else {
		money -= currentBet;
		bets[selectedHand] += currentBet;
		hit();
		stand();
	}
}

void Game::split(){
	if (money >= bets[selectedHand]) {
		canDoubleDown = false;
		if (PlayerHands[selectedHand].size() == 2 && PlayerHands[selectedHand][0].value == PlayerHands[selectedHand][1].value) {
			std::vector<std::vector<Card>> temp;
			for (int i = 0; i < PlayerHands.size(); i++) {
				if (i == selectedHand) {
					temp.push_back({ PlayerHands[i][0] });
					temp.push_back({ PlayerHands[i][1] });
				}
				else {
					temp.push_back(PlayerHands[i]);
				}
			}
			bets.push_back(bets[selectedHand]);
			money -= bets[selectedHand];
			PlayerHands = temp;
		}
	}
}

int Game::GetDealerHandValue()
{
	int temp = 0;
	std::vector<int> Values = {};
	for (const auto& card : DealerHand) {
		temp += rank[card.value];
	}
	Values.push_back(temp);
	for (const auto& card : DealerHand) {
		if (card.value == 12) {
			if (temp + 10 > 21)
				break;
			temp += 10;
			Values.push_back(temp);
			break;
		}
	}
	return Values.back();
}

bool Game::DealerTakeCards(){
	if (GetDealerHandValue() < 17) {
		DealerHand.push_back(deck.back());
		deck.pop_back();
		return true;
	}
	return false;
}

void Game::CalculateResults()
{
	std::vector<std::vector<int>> playerHandsValues = GetHandsValue();
	int dealerValue = GetDealerHandValue();
	for (int i = 0; i < playerHandsValues.size(); i++) {
		int maxPlayerValue = playerHandsValues[i][0];
		if (playerHandsValues[i].size() == 2) {
			if (playerHandsValues[i][1] > maxPlayerValue && playerHandsValues[i][1] <= 21) {
				maxPlayerValue = playerHandsValues[i][1];
			}
		}

		if (maxPlayerValue > 21) {
			continue;
		}
		if (dealerValue > 21 || maxPlayerValue > dealerValue) {
			money += bets[i] * 2;
		}
		else if (maxPlayerValue == dealerValue) {
			money += bets[i];
		}
	}
}

int Game::GetBalance()
{
	return money;
}
