#include <hanabi_game.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

void output_in_box(int size, char fill, const std::string& text) {
	std::cout << "| ";
	std::cout.width(size);
	std::cout.fill(fill);
	std::cout << std::left;
	std::cout << text << " |" << std::endl;
}

void output_in_box(int max_size, std::string spacer, const std::vector<card> &stack) {
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
		output_in_box(max_size, ' ', ss.str());
		ss.clear();
		ss.str(std::string());
	}
}


int main() {
	std::vector<std::string> vec = {"G1", "G1","G1", "G1","G1", "G1","G1", "G1","G1", "G1","G1", "G1","G1", "G1","G1", "G1"};
	std::cout << vec.size() << std::endl;
	
	std::string text("hello world");
	output_in_box(76, ' ', text);
	
	std::vector<card> stack;
	stack.reserve(100);
	for(int i=1; i<100; ++i) { stack.push_back(card(red,1)); }
	std::stringstream ss;
	for(int i=0; i<stack.size(); ++i) {
		ss << stack[i].print() << "     ";
	}
	output_in_box(76, ' ', ss.str());
	
	std::cout << " #### " << std::endl;
	
	output_in_box(76, " ", stack);
	
}



