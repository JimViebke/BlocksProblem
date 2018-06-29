#include <string>
#include <limits>

namespace uva
{
    const std::string MOVE_OVER = "mv";
    const std::string MOVE_ONTO = "mn";
    const std::string PILE_OVER = "pv";
    const std::string PILE_ONTO = "pn";
    const size_t NULL_PLACEHOLDER = std::numeric_limits<size_t>::max() - 1;

    enum class MOVEMENT_ENUM
    {
        MOVE_OVER_ENUM,
        MOVE_ONTO_ENUM,
        PILE_OVER_ENUM,
        PILE_ONTO_ENUM
    };
}