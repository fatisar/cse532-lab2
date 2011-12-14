// card.h
// Names: Alex Benjamin
// Email: abenjamin@wustl.edu
//
#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <ostream>
using namespace std;

class Card
{
public:
	bool operator< (const Card &) const;
	bool operator== (const Card &) const;
	enum suit {club, diamond, heart, spade};
	enum rank {two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace};
	suit c_suit;
	rank c_rank;
        void set_values(char s, char r);
};

ostream & operator<< (ostream & o, const Card & c);

#endif
