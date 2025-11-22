#pragma once

#include <cmath>
#include <cstdint>

class CounterAbstract {
protected:
    enum : std::uint8_t {
        MAX_VALUE = 255
    };

public:
    enum Direction : bool {
        FORWARD = true,
        REVERSE = false,
    };

    enum Status : bool {
        ENABLED = true,
        DISABLED = false,
    };
};

class Counter : public  CounterAbstract {
protected:
    Status isEnabled = Status::ENABLED;
    Direction dir = Direction::FORWARD;

    unsigned int firstStep = 0;
    unsigned int counter = 0;
    unsigned int maxValue = MAX_VALUE;
    unsigned int presetValue = 0;

public:
    Counter() = default;
    Counter(int n) { this->maxValue = n; }

    virtual auto direction() -> Direction { return this->dir; }
    virtual auto getDirection() -> Direction { return this->dir; }
    virtual auto value() -> unsigned int { return this->counter; }
    virtual auto set(unsigned int value) -> unsigned int;
    virtual auto setMaxValue(unsigned int maxValue) -> unsigned int;
    virtual auto forward() -> unsigned int;
    virtual auto preset() -> unsigned int;
    virtual auto enable() -> Status;
    virtual auto setPreset(unsigned int presetValue) -> unsigned int;
    virtual auto reset() -> unsigned int;
    virtual auto back() -> unsigned int;
    virtual auto step() -> unsigned int;
    virtual auto setDirection(Direction newDirection) -> Direction;
    virtual auto toggleDirection() -> Direction;

    virtual ~Counter() = default;
};
