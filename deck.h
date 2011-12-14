// deck.h
// Names: Alex Benjamin
// Email: abenjamin@wustl.edu
//

#ifndef DECK_H
#define DECK_H

#include "card.h"
#include "hand.h"
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
using namespace std;

class Hand;

class Deck {
public:
	Deck();
	//Deck(string,bool);
	//Compiler supplied destructor is fine since we use STL container
	int add_card(Card);
	//int load(string);
	void reseed();
	void shuffle();
	void print();
	size_t size() const;
	Card operator[] (const size_t) const;
	void eraseCard(size_t index);
	friend ostream & operator<< (ostream &, const Deck &);
	friend void operator<< (Hand &h, Deck &d);
private:
	vector<Card> deckVec;
};

ostream & operator<< (ostream &, const Deck &);

#endif
