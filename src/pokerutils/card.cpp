// card.cpp
// Names: Alex Benjamin
// Email: abenjamin@wustl.edu
//

#include "card.h"
#include "poker.h"

bool Card::operator< (const Card &p) const {
  return (c_rank < p.c_rank) || ((c_rank == p.c_rank) && (c_suit < p.c_suit));
}

bool Card::operator== (const Card &otherCard) const{
	return c_rank == otherCard.c_rank && c_suit == otherCard.c_suit;
}

void Card::set_values(char s, char r) {
    if (s == 'S') {
      c_suit = Card::spade;
    } else if (s == 'H') {
      c_suit = Card::heart;
    } else if (s == 'D') {
      c_suit = Card::diamond;
    } else if (s == 'C') {
      c_suit = Card::club;
    }
    if (r == '2') {
      c_rank = Card::two;
    } else if (r == '3') {
      c_rank = Card::three;
    } else if (r == '4') {
      c_rank = Card::four;
    } else if (r == '5') {
      c_rank = Card::five;
    } else if (r == '6') {
      c_rank = Card::six;
    } else if (r == '7') {
      c_rank = Card::seven;
    } else if (r == '8') {
      c_rank = Card::eight;
    } else if (r == '9') {
      c_rank = Card::nine;
    } else if (r == '1') { // "1" was the single-char encoding for 10
      c_rank = Card::ten;
    } else if (r == 'J') {
      c_rank = Card::jack;
    } else if (r == 'Q') {
      c_rank = Card::queen;
    } else if (r == 'K') {
      c_rank = Card::king;
    }    else if (r == 'A') {
      c_rank = Card::ace;
    }

}

ostream & operator<< (ostream & o, const Card & c) {
	o << printRank(c) << printSuit(c);
	return o;
}
