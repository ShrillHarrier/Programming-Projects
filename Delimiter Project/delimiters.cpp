#include <iostream>
#include "project_1.hpp"

int main();
void print_program();
bool does_delimiter_match(char delimiter);
bool do_delimiters_nest();

int main(){
	print_program();
	std::cout << does_delimiter_match( '(' ) << std::endl;
	std::cout << does_delimiter_match( '[' ) << std::endl;
	std::cout << does_delimiter_match( '{' ) << std::endl;
	std::cout << do_delimiters_nest() << std::endl;

	return 0;
}

void print_program(){

	ece150::start_reading_file();

	while(ece150::at_eof() == false){
			std::cout<<ece150::get_next_character();
		}
	std::cout<<std::endl;
}

bool does_delimiter_match(char delimiter){
	ece150::start_reading_file();

	if(delimiter == '('){
		bool result = true;
		int open = 0;
		int closed = 0;

		while(ece150::at_eof() == false){
			char character = ece150::get_next_character();

			//std::cout<<"The character was a "<<character<<std::endl;

			if(character == '('){
				++open;
			}
			else if(character == ')'){
				++closed;
			}

			if(closed > open){
				std::cout<<"Unmatched )"<<std::endl;
				--closed;
				result = false;
			}
		}

		if(open > closed){
			std::cout<<"Missing ";
			for(int i = 0; i < (open - closed); ++i){
				std::cout<<")";
			}
			std::cout<<std::endl;
			return false;
		}
		else{
			return result;
		}
	}
	else if(delimiter == '['){
		bool result = true;
				int open = 0;
				int closed = 0;

				while(ece150::at_eof() == false){
					char character = ece150::get_next_character();

					if(character == delimiter){
						++open;
					}
					else if(character == ']'){
						++closed;
					}

					if(closed > open){
						std::cout<<"Unmatched ]"<<std::endl;
						--closed;
						result = false;
					}
				}

				if(open > closed){
					std::cout<<"Missing ";
							for(int i = 0; i < (open - closed); ++i){
								std::cout<<"]";
							}
							std::cout<<std::endl;
							return false;
				}
				else{
					return result;
				}
	}
	else if(delimiter == '{'){
				bool result = true;
				int open = 0;
				int closed = 0;

				while(ece150::at_eof() == false){
					char character = ece150::get_next_character();

					if(character == delimiter){
						++open;
					}
					else if(character == '}'){
						++closed;
					}

					if(closed > open){
						std::cout<<"Unmatched }"<<std::endl;
						--closed;
						result = false;
					}
				}

				if(open > closed){
					std::cout<<"Missing ";
							for(int i = 0; i < (open - closed); ++i){
								std::cout<<"}";
							}
							std::cout<<std::endl;
							return false;
				}
				else{
					return result;
				}
	}
	else{
		return false;
	}
}

bool do_delimiters_nest(){
	ece150::start_reading_file();
	ece150::initialize_table();
	bool nested = true;

	while(ece150::at_eof() == false){
		char character = ece150::get_next_character();

		if(character == '(' || character == '[' || character == '{'){
			ece150::place_character(character);
		}
		else if(character == ')'){
			if(ece150::get_right_most_character() == '('){
				ece150::erase_right_most_character();
			}
			else{
				std::cout<<"Unmatched )"<<std::endl;
				nested = false;
			}
		}
		else if(character == ']'){
			if(ece150::get_right_most_character() == '['){
						ece150::erase_right_most_character();
					}
					else{
						std::cout<<"Unmatched ]"<<std::endl;
						nested = false;
					}
		}
		else if(character == '}'){
			if(ece150::get_right_most_character() == '{'){
								ece150::erase_right_most_character();
							}
							else{
								std::cout<<"Unmatched }"<<std::endl;
								nested = false;
							}
		}
	}

	if(ece150::is_table_empty() && nested == true){
		return true;
	}
	else{
		std::cout<<"Missing ";
		while(ece150::is_table_empty() == false){
			char character = ece150::get_right_most_character();
			if(character == '('){
				std::cout<<')';
			}
			else if(character == '['){
				std::cout<<']';
			}
			else if(character == '{'){
				std::cout<<'}';
			}
			ece150::erase_right_most_character();
		}
		std::cout<<std::endl;
		return false;
	}

}

