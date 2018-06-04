#pragma once

#include <vector>

#include "generator.h"

class world
{
public:
	world(const size_t size) : world_(size, std::vector<size_t>())
	{
		lookup.reserve(size);

		for (size_t i = 0; i < size; ++i)
		{
			world_[i].reserve(size);
			world_[i].push_back(i);

			lookup.push_back(i);
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
	}

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

	void print() const
	{
		for (size_t bucket = 0; bucket < world_.size(); ++bucket)
		{
			std::cout << bucket << ':';

			for (auto value : world_[bucket])
			{
				std::cout << ' ' << value;
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}

	void print(const size_t n)
	{
		for (size_t bucket = 0; bucket < n; ++bucket)
		{
			std::cout << bucket << ':';

			for (auto value : world_[bucket])
			{
				std::cout << ' ' << value;
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}

private:
	void move(const size_t value_1, const size_t value_2)
	{
		const size_t start_bucket_index = lookup[value_1];
		const size_t target_bucket_index = lookup[value_2];

		world_[target_bucket_index].push_back(value_1);
		world_[start_bucket_index].erase(--world_[start_bucket_index].end());

		// note that value 1 is now in the same bucket as value 2
		lookup[value_1] = target_bucket_index;
	}

	void pile(const size_t value_1, const size_t value_2)
	{
		const size_t start_bucket_index = lookup[value_1];
		const size_t target_bucket_index = lookup[value_2];

		auto& start_bucket = world_[start_bucket_index];
		auto& target_bucket = world_[target_bucket_index];

		// find the start of the elements to move
		auto it = std::find(start_bucket.begin(), start_bucket.end(), value_1);

		// note that the elements in range have moved to the target bucket
		for (auto it2 = it; it2 != start_bucket.end(); ++it2)
		{
			lookup[*it2] = target_bucket_index;
		}

		// insert the range of elements to the target bucket
		target_bucket.insert(target_bucket.end(), it, start_bucket.end());

		// remove the range of elements from the starting bucket
		start_bucket.erase(it, start_bucket.end());
	}

	bool is_valid(const size_t value_1, const size_t value_2) const
	{
		return value_1 != value_2 && lookup[value_1] != lookup[value_2];
	}

	void return_elements_above(const size_t value)
	{
		// find this element's bucket
		auto& bucket = world_[lookup[value]];

		// while there are elements above this value
		while (top_of(bucket) != value)
		{
			// get the value to move (the value at the top of this bucket)
			const size_t returning_value = top_of(bucket);
			// copy the value to the front of its original bucket
			world_[returning_value].insert(world_[returning_value].begin(), returning_value);
			// note that the value is in its original bucket
			lookup[returning_value] = returning_value;
			// remove this value from the bucket
			bucket.erase(--bucket.end());
		}
	}

	size_t top_of(const std::vector<size_t> & bucket) const
	{
		return bucket[bucket.size() - 1];
	}

	std::vector<std::vector<size_t>> world_;

	// maps a value (pos) to its bucket (value).
	std::vector<size_t> lookup;
};
