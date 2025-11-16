#pragma once

#include "../Counter/Counter.hpp"

class NCounter : public Counter {
	public:
		NCounter(int n);

		using Counter::step;
		using Counter::reset;
		using Counter::enable;
		using Counter::value;
};
