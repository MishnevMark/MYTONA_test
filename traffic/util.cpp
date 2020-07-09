#include "util.hpp"

int16_t uniformRandomNumber(int16_t rightBorder)
{
    std::random_device rd{};
    std::default_random_engine generator{rd()};;
    std::uniform_int_distribution<int16_t> distribution(0, rightBorder);
    return distribution(generator);
}
