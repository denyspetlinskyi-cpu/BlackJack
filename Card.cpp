#include "Card.h"
Card::Card(master s, rank v) {
	this->suit = s;
	this->value = v;
}
Card::~Card() {}