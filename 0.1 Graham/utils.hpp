#include <vector>
#include <string>
#include <sstream>

namespace uva
{
	std::vector<std::string> split(const std::string &str, const char &delimeter)
	{
		std::istringstream stream(str);
		std::string token;
		std::vector<std::string> tokens;
		while (getline(stream, token, delimeter))
			tokens.push_back(token);
		return tokens;
	}
}
