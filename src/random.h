#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <cassert>

template <typename IntType = unsigned int>
struct RandomGenerator {
    RandomGenerator(IntType max = std::numeric_limits<IntType>::max());

    // Generate a value in the range [min, max)
    IntType generate(IntType min, IntType max);

    // Generate a value in the range [0, max)
    IntType generate(IntType max);

    // Generate a value in the range [0, max)
    IntType generate();

    RandomGenerator(RandomGenerator const &) = delete;
    RandomGenerator & operator=(RandomGenerator const &) = delete;

private:
    std::random_device device_;
    std::default_random_engine engine_;
    std::uniform_int_distribution<IntType> uniform_;
};

/*******************************************************************/
template <typename IntType>
RandomGenerator<IntType>::RandomGenerator(IntType max)
    : device_()
    , engine_(device_())
    , uniform_(0, max)
{
}

/*******************************************************************/
template <typename IntType>
IntType RandomGenerator<IntType>::generate(IntType min, IntType max) {
    assert(max <= uniform_.max());
    auto const range = max - min;
    return (uniform_(engine_) % range) + min;
}

/*******************************************************************/
template <typename IntType>
IntType RandomGenerator<IntType>::generate(IntType max) {
    assert(max <= uniform_.max());
    return uniform_(engine_) % max;
}

/*******************************************************************/
template <typename IntType>
IntType RandomGenerator<IntType>::generate() {
    return uniform_(engine_);
}

#endif
