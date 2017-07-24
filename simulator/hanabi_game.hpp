#ifndef HANABI_GAME_HPP_
#define HANABI_GAME_HPP_

#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include <iostream>

enum color_t {red, green, blue, white, yellow, null};

struct card{
	color_t color;
	int value;
	card() : color(null), value(0) {}
	card(color_t c, int v) : color(c), value(v) {}
	card(const card& c) : color(c.color), value(c.value) {}
	//card& operator=(const card& c) { //TODO }
	~card() {}
	bool is_null() const {return color==null || value==0;}
	std::string print() const {
		std::stringstream ss;
		switch(color) {
			case null   : ss << "N"; break;
			case red    : ss << "R"; break;
			case green  : ss << "G"; break;
			case blue   : ss << "B"; break;
			case white  : ss << "W"; break;
			case yellow : ss << "Y"; break;
			default : ss << "?";
		}
		ss << value;
		return ss.str();
	}
	std::string print_color() const {
		std::stringstream ss;
		switch(color) {
			case null   : ss << "N"; break;
			case red    : ss << "R"; break;
			case green  : ss << "G"; break;
			case blue   : ss << "B"; break;
			case white  : ss << "W"; break;
			case yellow : ss << "Y"; break;
			default : ss << "?";
		}
		return ss.str();
	}
};

class hanabi_game {
	std::array<color_t,5> colors;
	/* attributes */
	/** Number of players */
	int nb_players;
	/** Number of cards per player */
	int nb_cards_per_player;
	/** Current state of the deck */
	std::vector<card> deck;
	/** Current hand for each player */
	std::vector<std::vector<card> > players_hands;
	/** Value of the biggest card in each color */
	std::vector<int> stacks;
	/** Current state of the discard stack */
	std::vector<card> discard_stack;
	/** Number of remaining blue tokens */
	int blue_tokens;
	/** Number of remaining red tokens */
	int red_tokens;
	/* methods */
public:
	/** Constructor (default = 2 players) */
	hanabi_game(int nb_players_=2);
	/** Copy constructor */
	hanabi_game(const hanabi_game& g);
	/** Destructor */
	virtual ~hanabi_game();
	// /** Initialize game from observation */
	// void init_from_observation(/* defausse, mains opposées, piles */);
	/** Play a card from a player's hand */
	int play(int player, int card_number);
	/** Discard a card from a player's hand */
	void discard(int player, int card_number);
	/** Spend a blue token to give an information */
	int give_info();
	/** Computes the current score of the board's stacks */
	int score() const;
	/** Returns true if all players have empty hands */
	bool all_hands_empty() const;
	/** Returns true if one of the termination conditions for the game is met */
	bool game_over() const;
	/** Get number of players */
	int get_number_of_players() const { return nb_players; }
	/** Get number of cards per player (depends on the number of players) */
	int get_nb_cards_per_player() const {return nb_cards_per_player; }
	/** Gets a (const) reference on player p's hand */
	const std::vector<card>& get_player_hand(int p) const {
		if(p>=0 && p<=nb_players) {	return players_hands.at(p); }
	}
	/** Gets a (const) reference on the board's stacks */
	const std::vector<int>& get_stacks() const { return stacks; }
	/** Gets a (const) reference on the discard stack */
	const std::vector<card>& get_discard_stack() const {return discard_stack;}
	/** Gets array of colors */
	const std::array<color_t,5>& get_colors() const {return colors;}
	/** Gets the number of remaining blue tokens */
	int get_blue_tokens() const {return blue_tokens;}
	/** Gets the number of remaining red tokens */
	int get_red_tokens() const {return red_tokens;}
	/** Gets a (const) reference on the deck */
	const std::vector<card>& get_deck() const {return deck;}
};

hanabi_game::hanabi_game(int nb_players_)
: colors {red, green, blue, white, yellow},
  nb_players(nb_players_),
  nb_cards_per_player(5),
  deck(),
  players_hands(nb_players_),
  stacks(5,0),
  discard_stack(),
  blue_tokens(8),
  red_tokens(3) {
	/* populate deck */
	deck.reserve(50);
	for(int col=0; col<colors.size(); ++col) {
		for (int i=0; i<3; ++i) { deck.push_back(card(colors.at(col),1)); }
		for (int i=0; i<2; ++i) { deck.push_back(card(colors.at(col),2)); }
		for (int i=0; i<2; ++i) { deck.push_back(card(colors.at(col),3)); }
		for (int i=0; i<2; ++i) { deck.push_back(card(colors.at(col),4)); }
		for (int i=0; i<1; ++i) { deck.push_back(card(colors.at(col),5)); }
	}
	std::random_shuffle(deck.begin(), deck.end());
	/* deal cards */
	if(nb_players>=4) {nb_cards_per_player=4;}
	players_hands = std::vector<std::vector<card> >(nb_players_, std::vector<card>(nb_cards_per_player));
	for(int p=0; p<nb_players_; ++p) {
		for(int c=0; c<nb_cards_per_player; ++c) {
			players_hands.at(p).at(c) = deck.back();
			deck.pop_back();
		}
	}
}

hanabi_game::~hanabi_game() {}

int hanabi_game::play(int player, int card_number) {
	int val=1;
	card& c=players_hands.at(player).at(card_number);
	// check card is real
	if(c.is_null()) {std::cout << "card is null" << std::endl;/* throw */}
	// play (legal move)
	if(stacks.at(c.color) == c.value-1) {++(stacks.at(c.color)); val=0;}
	// play (illegal move)
	else { 
		discard_stack.push_back(c);
		--red_tokens;
		val = 1;
	}
	// draw card (if possible)
	if(deck.empty()) {
		c.color=null;
		c.value=0;
	}
	else {
		c = deck.back();
		deck.pop_back();
	}
	return val;
}

void hanabi_game::discard(int player, int card_number) {
	card& c=players_hands.at(player).at(card_number);
	// check card is real
	if(c.is_null()) {/* throw */}
	// discard card
	discard_stack.push_back(c);
	// draw card (if possible)
	if(deck.empty()) {
		c.color=null;
		c.value=0;
	}
	else {
		c = deck.back();
		deck.pop_back();
	}
	// get token (if possible)
	if(blue_tokens<8) { ++blue_tokens; }
}

int hanabi_game::give_info() {
	if(blue_tokens > 0) {
		--blue_tokens;
		return 0;
	}
	else { return 1; }
}

int hanabi_game::score() const {
	int sum=0;
	for(int col=0; col<5; ++col) { sum += stacks.at(col); }
	return sum;
}

bool hanabi_game::all_hands_empty() const {
	bool all_hands_empty = true;
	for(int p=0; p<nb_players; ++p) {
		all_hands_empty = all_hands_empty && players_hands.at(p).empty();
	}
	return all_hands_empty;
}

bool hanabi_game::game_over() const {
	if(red_tokens == 0) {return true;}
	if(score()==25) {return true;}
	if(all_hands_empty()) {return true;}
	return false;
}

#endif
