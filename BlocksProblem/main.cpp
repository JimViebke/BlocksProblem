
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

#include "world.hpp"

void print_commands(const std::vector<command> & commands)
{
	for (size_t i = 0; i < commands.size(); ++i)
	{
		std::cout << i << ": "
			<< commands[i].string1 << ' '
			<< commands[i].num1 << ' '
			<< commands[i].string2 << ' '
			<< commands[i].num2 << '\n';
	}

	std::cout << "quit\n\n";
}

using time_t = long long;

time_t current_time_in_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


int main()
{
	const time_t start_load = current_time_in_ms();

	unsigned block_count = 0;
	std::cin >> block_count;

	std::vector<command> commands;

	size_t count = 0;
	while (true)
	{
		command command;

		std::cin >> command.string1;
		if (command.string1 == "quit") break;
		std::cin >> command.num1 >> command.string2 >> command.num2;

		commands.push_back(command);
	}

	const time_t finish_load = current_time_in_ms();

	const time_t start_execution = current_time_in_ms();

	// std::cout << "Printing commands...\n\n";
	// print_commands(commands);

	world world(block_count);
	int counter = 0;
	for (const command & command : commands)
	{
		world.execute_command(command);

		//if (++counter == 1000)
		//{
		//	world.print(10);
		//	counter = 0;
		//}
	}

	const time_t finish_execution = current_time_in_ms();

	world.print();

	std::cout << "Executed " << commands.size() << " steps against " << block_count << " blocks.\n";
	std::cout << "Loaded commands in " << finish_load - start_load << " ms.\n";
	std::cout << "Executed commands in " << finish_execution - start_execution << " ms.\n";
	std::cout << (finish_execution - start_execution) * 1'000'000 / commands.size() << " ns per instruction.\n";
}
