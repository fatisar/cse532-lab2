// CSE571 - game.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Game class responsible for managing a game of poker.

#include <iostream>
#include <utility>
#include <algorithm>

#include "game.h"
#include "player.h"
#include "deck.h"

const int PLAYERS_REQUIRED = 3;

Game::Game() {
  scores_received = 0;
  Card c;
  //Fill up our deck with one of each card in a standard deck
  for(unsigned int suit = Card::club; suit <= Card::spade; ++suit){
    c.c_suit = (Card::suit)suit;
    for(unsigned int rank = Card::two; rank <= Card::ace; ++rank){
      c.c_rank = (Card::rank)rank;
      deck.add_card(c);
    }
  }
  //reset PRNG
  deck.reseed();
  //shuffle deck
  deck.shuffle();
  running = false;
}

void Game::score_added() {
  ++scores_received;
  if ((unsigned int) scores_received == players.size()) {
    sort(players.begin(), players.end(), player_ptr_less());
    std::cout << "Scores for " << name << ":" << std::endl;
    for (unsigned int i = 0; i < players.size(); ++i) {
      std::cout << players[i]->name << ": " << players[i]->score <<
    	" (" << players[i]->hand << ")" << std::endl;
    }
  }
}

const unsigned int NUM_CARDS = 5;

void Game::send_hand(Player* player) {
  player->active_game = this;
  for(unsigned int i = 0; i < NUM_CARDS; ++i) {
    int last_card = deck.size() - 1;
    player->send_card(deck[last_card]);
    player->hand.add_card(deck[last_card]);
    deck.eraseCard(last_card);
  }
}

void Game::new_player_added() {
  // If we have just added a player and now have 5, then the game is about to
  // begin so we should send cards to all players.
  if (players.size() == (unsigned int) PLAYERS_REQUIRED) {
    running = true;
    for (unsigned int i = 0; i < players.size(); ++i) {
      send_hand(players[i]);
    }

  } else if (players.size() > (unsigned int) PLAYERS_REQUIRED) {
    // If we have more than 5 players, the game has already begun but we
    // might still be able to include the new player, so check if the game
    // is still running.
    if (running) {
      send_hand(players.back());
    }
  }
}
