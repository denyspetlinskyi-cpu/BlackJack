#pragma once
enum master
{
	Hearts,
	Diamonds,
	Clubs,
	Spades
};
enum rank
{
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace
};
class Card
{
public:
	master suit;
	rank value;
	Card(master s, rank v);
	~Card();
};

