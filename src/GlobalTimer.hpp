#pragma once
#include <chrono>

namespace globaltimer {
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::seconds timeLimit;
    std::chrono::high_resolution_clock::time_point endTime;

    void setLimit(std::chrono::seconds sec){
        timeLimit = sec;
        startTime = std::chrono::high_resolution_clock::now();
    }
    
    template<class Duration = std::chrono::nanoseconds>
    Duration getTime(){
        endTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<Duration> (endTime - startTime);
    }

    bool TLE() {
        return getTime() >= timeLimit;
    }

};