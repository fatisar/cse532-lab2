// hand.h
// Names: Alex Benjamin
// Email: abenjamin@wustl.edu
//

#ifndef HAND_H
#define HAND_H

#include "deck.h"
#include <vector>
#include <ostream>
#include <iostream>
#include <string>
using namespace std;

class Deck;

class Hand {
public:
	Hand();
	Hand(const Hand &);
	//default constructor is fine since we use STL container
	Hand & operator= (const Hand &);
	Card operator[] (const size_t) const;
	bool operator== (const Hand &) const;
	bool operator< (const Hand &);
	int remove_card(const size_t);
        void add_card(Card card);
	string asString() const;
	size_t size() const;
	vector<Card> getHand() const;
	friend ostream & operator<< (ostream &, const Hand &);
	friend void operator<< (Hand &h, Deck &d);
private:
	vector<Card> handVec;
};

ostream & operator<< (ostream &, const Hand &);
void operator<< (Hand &h, Deck &d);


#endif
