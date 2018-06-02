#include <iostream>
#include <fstream>
#include <sstream>

unsigned random_int_from(const unsigned & min, const unsigned & max)
{
	return min + (rand() % (max - min + 1));
}

int main()
{
	unsigned number_of_buckets = 0;
	std::cout << "Number of buckets: ";
	std::cin >> number_of_buckets;

	unsigned number_of_steps = 0;
	std::cout << "Number of steps: ";
	std::cin >> number_of_steps;

	std::stringstream filename;
	filename << "testdata-" << number_of_buckets << "buckets-" << number_of_steps << "steps.txt";

	std::ofstream file(filename.str());

	if (!file.is_open())
	{
		std::cout << "File is not open.";
	}

	file << number_of_buckets << '\n';

	for (unsigned i = 0; i < number_of_steps; ++i)
	{
		file << ((random_int_from(0, 1) == 0) ? "pile " : "move ");
		file << random_int_from(0, number_of_buckets - 1) << " ";
		file << ((random_int_from(0, 1) == 0) ? "over " : "onto ");
		file << random_int_from(0, number_of_buckets - 1) << "\n";
	}
	file << "quit\n";
}
