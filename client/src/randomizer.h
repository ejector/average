#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>

namespace average {

class Randomizer final
{
public:
    explicit Randomizer(int min, int max)
        : _generator(_random_device())
        , _distrib(min, max)
    {

    }
    int random()
    {
        return _distrib(_generator);
    }
protected:
    std::random_device _random_device;
    std::mt19937 _generator;
    std::uniform_int_distribution<> _distrib;
};

}

#endif // RANDOMIZER_H
