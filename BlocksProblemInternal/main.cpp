
#include <iostream>
#include <chrono>

#include "world.h"

using time_t = long long;

time_t current_time_in_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main()
{
	std::cout << "Enter block count: ";
	size_t block_count = 0;
	std::cin >> block_count;

	std::cout << "Enter instruction count: ";
	size_t instruction_count = 0;
	std::cin >> instruction_count;

	world world(block_count);



	const time_t start_load = current_time_in_ms();
	auto instructions = generate(block_count, instruction_count);
	const time_t finish_load = current_time_in_ms();

	const time_t start_execution = current_time_in_ms();
	for (auto instruction : instructions)
		world.execute_command(instruction);
	const time_t finish_execution = current_time_in_ms();



	world.print();

	std::cout << "Executed " << instructions.size()-1 << " steps against " << block_count << " blocks.\n";
	std::cout << "Loaded commands in " << finish_load - start_load << "ms.\n";
	std::cout << "Executed commands in " << finish_execution - start_execution << "ms.\n";
}
