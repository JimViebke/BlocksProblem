#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <memory>
#include <chrono>

#include "box_container.hpp"
#include "utils.hpp"
#include "command.hpp"

using namespace std;
using namespace uva;

int main(int argc, char **argv)
{
	int number_of_buckets = 0;
	cin >> number_of_buckets;

	box_container boxes(number_of_buckets);

	string read_command;
	vector<string> tokens;
	vector<command> commands;
	commands.reserve(number_of_buckets);

	auto start = std::chrono::high_resolution_clock::now();
	while (getline(cin, read_command))
	{
		tokens = split(read_command, ' ');

		if(tokens.size() == 4)
		{
			// A command is illegal if box indicies are the same number.
			if(tokens[1] != tokens[3])
				commands.push_back(command(stoi(tokens[1]), stoi(tokens[3]), tokens[0][0], tokens[2][1]));
		}
		else if (tokens.size() == 1 && tokens[0][0] == 'q')
			break;
	}
	auto diff = std::chrono::high_resolution_clock::now() - start;
    auto read_commands_time = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

	start = std::chrono::high_resolution_clock::now();
	for(const auto &ready_command : commands)
	{
		switch(ready_command.movement)
		{
			case MOVEMENT_ENUM::MOVE_OVER_ENUM:
				boxes.move_over(ready_command.a_idx, ready_command.b_idx);
			case MOVEMENT_ENUM::MOVE_ONTO_ENUM:
				boxes.move_onto(ready_command.a_idx, ready_command.b_idx);
			case MOVEMENT_ENUM::PILE_OVER_ENUM:
				boxes.pile_over(ready_command.a_idx, ready_command.b_idx);
			case MOVEMENT_ENUM::PILE_ONTO_ENUM:
				boxes.pile_onto(ready_command.a_idx, ready_command.b_idx);
		}
	}
	diff = std::chrono::high_resolution_clock::now() - start;
    auto run_commands_time = std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();

	box_container::layout_type layout = boxes.get_layout();
	for (size_t i = 0, bucket_number = 0; i < layout.size(); ++i)
	{
		if(i % (number_of_buckets + 1) == 0)
			cout << "\n" << bucket_number++ << ":";

		if(layout[i] != NULL_PLACEHOLDER)
			cout << " " << layout[i];
	}

	std::cout << "Executed " << commands.size() << " steps against " << number_of_buckets << " blocks.\n";
	std::cout << "Loaded commands in " << read_commands_time << " ms.\n";
	std::cout << "Executed commands in " << run_commands_time << " ns.\n";
	std::cout << run_commands_time / commands.size() << " ns per instruction.\n";
}
