#pragma once
#include <string>
#include <vector>
#include <map>

#include "metro.h"
#include "usual_station.h"
#include "transfer_station.h"
#include "cross_platform_station.h"

class MetroAll {
public:
    MetroAll();

    void addLine(int number, const std::string& name, bool circular);

    void addUsualStation(const std::string& name, int traffic, int lineNumber);
    void addTransferStation(const std::string& name, int traffic, int lineNumber, int platformCount);
    void addCrossPlatformStation(const std::string& name, int traffic, int lineNumber);

    void addStationToLine(int lineNumber, const std::string& stationName);

    void addSpan(int lineNumber, const std::string& from, const std::string& to, int time);

    void addTransfer(const std::string& from, const std::string& to, int time, int averageTraffic);

    std::pair<std::vector<std::string>, int>
    shortestRoute(const std::string& from, const std::string& to);

private:
    Metro metro;
    std::map<std::string, BaseStation*> stations;
    std::map<int, Line*> lines;
};
