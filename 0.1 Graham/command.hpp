#include <cstddef>
#include <string>

namespace uva
{
    class command
    {
    public:
        size_t a_idx;
        size_t b_idx;
        MOVEMENT_ENUM movement;

        command(const size_t &a, const size_t &b, const char &bucket, const char &stack) : a_idx(a), b_idx(b)
        {
            std::string mov = std::string() + bucket + stack;

            if(mov == MOVE_OVER)
			    movement = MOVEMENT_ENUM::MOVE_OVER_ENUM;
		    else if(mov == MOVE_ONTO)
			    movement = MOVEMENT_ENUM::MOVE_ONTO_ENUM;
		    else if(mov == PILE_OVER)
			    movement = MOVEMENT_ENUM::PILE_OVER_ENUM;
		    else if(mov == PILE_ONTO)
			    movement = MOVEMENT_ENUM::PILE_ONTO_ENUM;
        }
    };
}