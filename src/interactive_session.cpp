#include <hanabi_game.hpp>
#include <interactive_session.hpp>
#include <iostream>

int main() {
	hanabi_game g;
	print_syntax();
	bool display_help = true;
	while(!g.game_over()) {
		// display game
		display(g, -1);
		// human player's input
		bool invalid_input = true;
		bool quit=false;
		while(invalid_input) {
			std::cout << "Your move (h for help)?" << std::endl;
			std::string human_input;
			std::cin >> human_input;
			if(human_input.size()==1) {
				if(human_input == "q") { // exit on demand
					invalid_input=false;
					quit=true;
				}
				else if(human_input == "h") { // displays help
					print_syntax();
				}
			}
			else if(human_input.size()==2) {
				if(human_input.at(0)=='p') { // play
					int card_number = human_input.at(1) - '0';
					if(card_number<5) {
						invalid_input=false;
						int res = g.play(0, card_number);
						std::cout << "Player 0 plays card #" << card_number << ". Result = ";
						if(res == 0) {std::cout << "success." << std::endl;}
						else {std::cout << "ouch, red token!" << std::endl;}
					}
				}
				else if (human_input.at(0)=='d') { // discard
					int card_number = human_input.at(1) - '0';
					if(card_number<5) {
						invalid_input=false;
						std::cout << "Player 0 discards card #" << card_number << std::endl;
						g.discard(0, card_number);
					}
				}
			}
			else if(human_input.size()==3) {
				if(human_input.at(0)=='c') { // give info on color
					int card_number = human_input.at(1) - '0';
					int player_number = atoi(&human_input.at(2));
					//std::cout << human_input.at(0) << " // " << human_input.at(1)  << " " << atoi(&human_input.at(1))  << " " << beep << " // " << human_input.at(2) << std::endl;
					std::cout << human_input << " card " << card_number << " player " << player_number << std::endl;
					if(player_number!=0 && player_number<g.get_number_of_players() && card_number<g.get_nb_cards_per_player()) {
						invalid_input=false;
						std::cout << "Player 0 gives '" << g.get_player_hand(player_number).at(card_number).print_color() << "' info to player " << player_number << std::endl;
						g.give_info();
					}
				}
				else if(human_input.at(0)=='v') { // give info on value
					int card_number = human_input.at(1) - '0';
					int player_number = atoi(&human_input.at(2));
					if(player_number!=0 && player_number<g.get_number_of_players() && card_number<g.get_nb_cards_per_player()) {
						invalid_input=false;
						std::cout << "Player 0 gives '" << g.get_player_hand(player_number).at(card_number).value << "' info to player " << player_number << std::endl;
						g.give_info();
					}
				}
			}
		}
		if(quit) { std::cout << "Exiting." << std::endl; break; }
		// play artifical players
		for(int p=1; p<g.get_number_of_players(); ++p) {
			/*
			std::cout << "Player " << p << " discards card #0" << std::endl;
			g.discard(p,0);
			*/
			int res = g.play(p, 0);
			std::cout << "Player " << p << " plays card #0. Result = ";
			if(res == 0) {std::cout << "success." << std::endl;}
			else {std::cout << "ouch, red token!" << std::endl;}
		}
	}
	std::cout << "Game over. Final board:" << std::endl;
	display(g, -1);
	
	return 0;
}
