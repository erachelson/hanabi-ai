#include <../simulator/hanabi_game.hpp>
#include <iostream>

void line_in_box(int size, char fill, const std::string& text) {
	std::cout << "| ";
	std::cout.width(size);
	std::cout.fill(fill);
	std::cout << std::left;
	std::cout << text << " |" << std::endl;
}

void multline_in_box(int max_size, std::string spacer, const std::vector<card> &stack) {
	std::stringstream ss;
	int index=0;
	while(index < stack.size()) {
		std::string word = stack.at(index).print();
		ss << word;
		++index;
		bool endofline=false;
		while(!endofline && index < stack.size()) {
			std::string word = stack.at(index).print();
			if(ss.str().size()+word.size()+spacer.size() < max_size) {
				ss << spacer << word;
				++index;
			}
			else { endofline = true; }
		}
		line_in_box(max_size, ' ', ss.str());
		ss.clear();
		ss.str(std::string());
	}
}

/** Displays the board as seen from a given player's perpective. If player_number is negative, displays the full board. */
void display(const hanabi_game& g, int player_number) {
	if (player_number > g.get_number_of_players()) { 
		std::cout << "player does not exist" << std::endl;
		return;
	}
	
	// top line
	int line_size=76;
	std::cout << "|";
	std::cout.width(line_size+3);
	std::cout.fill('-');
	std::cout << "|" << std::endl;
	
	std::stringstream ss;
	// players
	for (int p=0; p<g.get_number_of_players(); ++p) {
		ss.str(std::string());
		ss << "### Player " << p;
		line_in_box(line_size, ' ', ss.str());
		if(p==player_number) {
			ss.str(std::string());
			for(int c=0; c<g.get_nb_cards_per_player(); ++c) {
				ss << "XX  ";
			}
			line_in_box(line_size, ' ', ss.str());
		}
		else {
			multline_in_box(line_size, " ", g.get_player_hand(p));
		}
		line_in_box(line_size, ' ', " ");
	}
	
	// board
	ss.str(std::string());
	ss << "### Board";
	line_in_box(line_size, ' ', ss.str());
	const std::vector<int>& stacks = g.get_stacks();
	const std::array<color_t,5> colors = g.get_colors();
	ss.str(std::string());
	for(int col=0; col<colors.size(); ++col) {
		card c(colors.at(col),stacks.at(col));
		ss << c.print() << "  ";
	}
	line_in_box(line_size, ' ', ss.str());
	line_in_box(line_size, ' ', " ");
	
	// discard stack
	ss.str(std::string());
	ss << "### Discard stack";
	line_in_box(line_size, ' ', ss.str());
	multline_in_box(line_size, " ", g.get_discard_stack());
	line_in_box(line_size, ' ', " ");
	
	// tokens and deck
	ss.str(std::string());
	ss << "### Blue: " << g.get_blue_tokens();
	ss << " / Red: " << g.get_red_tokens();
	ss << " / Deck: " << g.get_deck().size();
	ss << " / Score: " << g.score();
	line_in_box(line_size, ' ', ss.str());
	
	// bottom line
	std::cout << "|";
	std::cout.width(line_size+3);
	std::cout.fill('-');
	std::cout << std::right << "|" << std::endl;
}

void print_syntax() {
	std::cout << "|---------------------------------------------------------|" << std::endl;
	std::cout << "| Syntax (warning, card/player numbering starts at zero): |" << std::endl;
	std::cout << "| p4   -> play card #4 (5th card)                         |" << std::endl;
	std::cout << "| d2   -> discard card #2 (3rd card)                      |" << std::endl;
	std::cout << "| c02  -> give color info on card #0 of player 2          |" << std::endl;
	std::cout << "| v13  -> give value info on card #1 of player 3          |" << std::endl;
	std::cout << "| q    -> quit                                            |" << std::endl;
	std::cout << "| h    -> displays this message                           |" << std::endl;
	std::cout << "|---------------------------------------------------------|" << std::endl;
}
