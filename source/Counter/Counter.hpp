#pragma once

#include <cmath>

// TODO: Find a better number.
#define MAX_VALUE 10

class Counter {
    protected:
        unsigned int is_enabled = 1;
        unsigned int dir = 1;
        unsigned int first_step = 0;
        unsigned int counter = 0;
        unsigned int max_value = MAX_VALUE;
        unsigned int preset_value = 0;

    public:
        Counter(){};
        Counter(int n){ this->max_value = n; }

        virtual int direction(){ return this->dir; }
        virtual int get_direction() { return this->dir; }
        virtual int value(){ return this->counter; }
        virtual int set(int val);
        virtual int set_max_value(int m);
        virtual int forward();
        virtual int preset();
        virtual int enable();
        virtual int set_preset(int p);
        virtual int reset();
        virtual int back();
        virtual int step();
        virtual int set_direction(int d);
        virtual int toggle_direction();
        
        virtual ~Counter() = default;
};
