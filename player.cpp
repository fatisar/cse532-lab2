// CSE571 - player.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Player class holds data for each player/connection on the server side.

#include <iostream>
#include "stdio.h"
#include <map>
#include <iterator>
#include <utility>

#include "player.h"
#include "card.h"
#include "poker.h"

// Player destructor.  We need to close up all our streams gracefully.
Player::~Player() {
  handler.shutdown();
}

void Player::send_card(Card card) {
  handler.enqueue_card_for_output(printSuit(card)[0], printRank(card)[0]);
}
