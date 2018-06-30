#pragma once

#include <vector>

namespace detail
{
	size_t random_int_from(const size_t min, const size_t max)
	{
		return min + (rand() % (max - min + 1));
	}
}

enum instruction : size_t
{
	move_over,
	move_onto,
	pile_over,
	pile_onto,
	quit
};

class command
{
public:
	command(const instruction instruction = quit,
		const size_t arg1 = 0, const size_t arg2 = 0) :
		instruction(instruction), arg1(arg1), arg2(arg2) {}

	instruction instruction;
	size_t arg1, arg2;

	void print() const
	{
		if (instruction == quit) { std::cout << "quit\n"; return; }

		switch (instruction)
		{
		case move_over: std::cout << "move_over("; break;
		case move_onto:	std::cout << "move_onto("; break;
		case pile_over:	std::cout << "pile_over("; break;
		case pile_onto: std::cout << "pile_onto("; break;
		}

		std::cout << arg1 << ", " << arg2 << ")\n";
	}
};

std::vector<command> generate(const size_t world_size, const size_t instruction_count)
{
	// Giving rand() the same seed per call to generate() should make this
	// function deterministic.
	srand(1);

	std::vector<command> commands(instruction_count + 1, command());

	for (size_t i = 0; i < instruction_count; ++i)
	{
		commands[i].instruction = (instruction)detail::random_int_from(0, 3);
		commands[i].arg1 = (instruction)detail::random_int_from(0, world_size - 1);
		commands[i].arg2 = (instruction)detail::random_int_from(0, world_size - 1);
	}

	return commands;
}
