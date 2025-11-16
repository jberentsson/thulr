#pragma once

#include "../Counter/Counter.hpp"

class BinaryCounter : public Counter {
	public:
		BinaryCounter();
		BinaryCounter(int n);

		int get_max_value();
		int set_max_value(int n);

		using Counter::step;
		using Counter::reset;
		using Counter::enable;
		using Counter::value;
		using Counter::direction;
};
