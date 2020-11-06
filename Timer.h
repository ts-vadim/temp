#pragma once

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>
#include <cstdio>


class Timer
{
public:
    Timer()
    {
        start_time = std::chrono::_V2::high_resolution_clock::now();
        title = "Timer";
    }
    Timer(char* _title)
    {
        start_time = std::chrono::_V2::high_resolution_clock::now();
        title = _title;
    }

    void setTitle(char* _title)
    {
        title = _title;
    }

    void reset()
    {
        start_time = std::chrono::_V2::high_resolution_clock::now();
    }

    double getElapsedTime()
    {
        auto end_time = std::chrono::_V2::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

        auto duration = end - start;

        double ms = duration * 0.001;

        return ms;
    }

    void show()
    {
        printf("%s: %fms\n", title, getElapsedTime());
    }

private:
    char* title;
    std::chrono::time_point<std::chrono::_V2::high_resolution_clock> start_time;
};



#endif // TIMER_H_INCLUDED
