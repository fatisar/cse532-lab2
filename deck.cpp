// deck.cpp
// Names: Alex Benjamin
// Email: abenjamin@wustl.edu
//

#include "deck.h"
#include "poker.h"
#include "card.h"
#include "hand.h"
#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;

Deck::Deck(){
}
int Deck::add_card(Card c){
	deckVec.push_back(c);
	return 0;
}
Card Deck::operator[] (const size_t index) const {
	if(index >= size())
		throw BAD_CARD_INDEX_ERROR;
	return deckVec[index];
}
void Deck::eraseCard(size_t index){
	if(index >= size())
		throw BAD_CARD_INDEX_ERROR;
	deckVec.erase(deckVec.begin()+index);
}
size_t Deck::size() const {
	return deckVec.size();
}

void Deck::reseed() {
	srand( ((int)time(0)) );
}

void Deck::shuffle() {
	int count = rand() % 50;
	while((count--) > 0){
		random_shuffle(deckVec.begin(), deckVec.end());
	}
}

ostream & operator<< (ostream & o, const Deck & deck) {
	vector<Card> deck_cards = deck.deckVec;
	for(size_t i = 0; i < deck_cards.size(); ++i)
		o << printRank(deck_cards[i]) << printSuit(deck_cards[i]) << endl;		
	return o;
}
