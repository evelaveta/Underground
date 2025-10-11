#pragma once
#include "base_station.h"

class Span { // перегон
public:
    Span(BaseStation* station1, BaseStation* station2, int travelTime)
        : station1_(station1), station2_(station2), travelTime_(travelTime) {}

    BaseStation* station1_;
    BaseStation* station2_;
    int travelTime_;
};
