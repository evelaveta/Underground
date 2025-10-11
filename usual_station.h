#pragma once
#include "base_station.h"

class UsualStation : public BaseStation { // обычная станция, все по заданию
public:
    UsualStation(const std::string& name, int averageTraffic, int lineNumber)
        : BaseStation(name, averageTraffic, lineNumber) {}

    void RegisterTraffic(int traffic) { // изменение пассажиропотока
        SetAverageTraffic(traffic);
    }

    bool operator<(const UsualStation& other) const { // сравнение
        return GetStationNumber() < other.GetStationNumber();
    }

    bool operator==(const UsualStation& other) const {
        return GetStationNumber() == other.GetStationNumber();
    }
};
