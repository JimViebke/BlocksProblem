#pragma once

#include <vector>

#include "generator.h"

class block
{
public:
	size_t element_above;
	size_t bucket;
	size_t element_below;

private:
	block operator=(const block&) = delete;
};

class world
{
public:
	world(const size_t size) : world_(size, block()), bottom_lookup(size, 0), NOTHING(size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			world_[i].element_above = NOTHING;
			world_[i].bucket = i;
			world_[i].element_below = NOTHING;

			bottom_lookup[i] = i;
		}
	}

	void execute_command(const command & command)
	{
		switch (command.instruction)
		{
		case instruction::move_over:
			move_over(command.arg1, command.arg2);
			break;
		case instruction::move_onto:
			move_onto(command.arg1, command.arg2);
			break;
		case instruction::pile_over:
			pile_over(command.arg1, command.arg2);
			break;
		case instruction::pile_onto:
			pile_onto(command.arg1, command.arg2);
			break;
		case instruction::quit:
			break;
		}

		//audit_world();
	}

	void print() const { print(world_.size()); }
	void print(const size_t n) const
	{
		// for each bucket
		for (size_t bucket = 0; bucket < n; ++bucket)
		{
			std::cout << bucket << ':';

			size_t element = bottom_lookup[bucket];

			// print each element from the bottom to the top
			while (element != NOTHING)
			{
				std::cout << ' ' << element;
				element = world_[element].element_above;
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}

private:

	// Our four basic operations
	void move_onto(const size_t value_1, const size_t value_2)
	{
		if (!is_valid(value_1, value_2)) return;

		return_elements_above(value_1);
		return_elements_above(value_2);
		move(value_1, value_2);
	}
	void move_over(const size_t value_1, const size_t value_2)
	{
		if (!is_valid(value_1, value_2)) return;

		return_elements_above(value_1);
		move(value_1, value_2);
	}
	void pile_onto(const size_t value_1, const size_t value_2)
	{
		if (!is_valid(value_1, value_2)) return;

		return_elements_above(value_2);
		pile(value_1, value_2);
	}
	void pile_over(const size_t value_1, const size_t value_2)
	{
		if (!is_valid(value_1, value_2)) return;

		pile(value_1, value_2);
	}

	// Three basic helper functions
	void move(const size_t value_1, const size_t value_2)
	{
		block& moving_block = world_[value_1];

		// if there is no element below this one
		if (moving_block.element_below == NOTHING)
		{
			// ...the bucket it is leaving now has no bottom
			bottom_lookup[moving_block.bucket] = NOTHING;
		}
		else
		{
			// note that the element below will have nothing above it
			world_[moving_block.element_below].element_above = NOTHING;
		}

		// find the top of the destination bucket
		const size_t destination_top_block = top_of(world_[value_2]);

		// this element is now in the same bucket as the destination bucket
		moving_block.bucket = world_[destination_top_block].bucket;

		// note that the moving element and the destination are next to each other
		moving_block.element_below = destination_top_block;
		world_[destination_top_block].element_above = value_1;
	}
	void pile(const size_t value_1, const size_t value_2)
	{
		// find the start of the elements to move
		block& moving_block = world_[value_1];

		// if there is nothing below this...
		if (moving_block.element_below == NOTHING)
		{
			// ...then the bucket we're moving out of will be empty
			bottom_lookup[moving_block.bucket] = NOTHING;
		}
		else // if there is an element below this
		{
			// note that the element below will have nothing above it
			world_[moving_block.element_below].element_above = NOTHING;
		}

		// find the top of the destination bucket
		const size_t destination_top = top_of(world_[value_2]);
		// record the new neighbors
		moving_block.element_below = destination_top;
		world_[destination_top].element_above = value_1;

		// note that we're in a new bucket
		const size_t target_bucket = world_[value_2].bucket;
		moving_block.bucket = target_bucket;

		// move up the bucket, noting that each element is now in a new bucket
		size_t next_block = value_1;
		while (world_[next_block].element_above != NOTHING)
		{
			next_block = world_[next_block].element_above;
			world_[next_block].bucket = target_bucket;
		}
	}
	void return_elements_above(const size_t value)
	{
		// to return element N,
		// -- find the element at the bottom of bucket N
		// -- declare N to be the new bottom of bucket N

		size_t current_top = top_of(world_[value]);

		// work down the bucket until the passed argument is the top
		while (current_top != value)
		{
			block& current_block = world_[current_top];

			// find any element in the bucket that is the value of the current top element
			const size_t old_bottom = bottom_lookup[current_top];

			// if there is at least one element in this element's original bucket
			if (old_bottom != NOTHING)
			{
				// set the moving block below the previously bottom block
				world_[old_bottom].element_below = current_top;
			}

			// whatever was on the bottom (including nothing) is now above this

			// cache what will be the new top of the working stack
			const size_t next_element = current_block.element_below;

			// update what is above, and the current bucket, and what is below
			current_block.element_above = old_bottom;
			current_block.bucket = current_top;
			current_block.element_below = NOTHING;

			// note that the element is now the bottom of its original bucket
			bottom_lookup[current_top] = current_top;

			// use the cached value for the next iteration
			current_top = next_element;
		}

		// finally, note that there is no longer any element above the top
		world_[current_top].element_above = NOTHING;
	}

	// Three utility functions
	size_t top_of(const block & block) const
	{
		size_t current_element = block.bucket;

		// while this element has an element above it
		while (world_[current_element].element_above != NOTHING)
		{
			current_element = world_[current_element].element_above;
		}

		return current_element;
	}

	bool is_valid(const size_t value_1, const size_t value_2) const
	{
		return value_1 != value_2 && world_[value_1].bucket != world_[value_2].bucket;
	}

	void audit_world() const
	{
		for (size_t element = 0; element < world_.size(); ++element)
		{
			const size_t above = world_[element].element_above;
			const size_t below = world_[element].element_below;

			if (above != NOTHING)
			{
				if (world_[above].element_below != element)
				{
					std::cout << "Corruption detected. " << element << " declares " << above
						<< " to be above itself, but " << above << " declares " << world_[above].element_below
						<< " to be below itself.";
					return;
				}
			}

			if (below != NOTHING)
			{
				if (world_[below].element_above != element)
				{
					std::cout << "Corruption detected. " << element << " declares " << below
						<< " to be below itself, but " << below << " declares " << world_[below].element_above
						<< " to be above itself.";
					return;
				}
			}
		}
	}

	std::vector<block> world_;
	std::vector<size_t> bottom_lookup; // maps a bucket to the element at the bottom of the bucket

	const size_t NOTHING;
};
