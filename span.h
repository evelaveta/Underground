#pragma once
#include "base_station.h"

class Span { // перегон
public:
    Span(BaseStation* station1, BaseStation* station2, int travelTime)
        : station1_(station1), station2_(station2), travelTime_(travelTime) {}

    BaseStation* GetStation1() const { return station1_; }
    BaseStation* GetStation2() const { return station2_; }
    int GetTravelTime() const { return travelTime_; }

private:
    BaseStation* station1_;
    BaseStation* station2_;
    int travelTime_;
};
