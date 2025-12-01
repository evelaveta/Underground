#include "metroall.h"
#include <iostream>

MetroAll::MetroAll() {}

// Линии
void MetroAll::addLine(int number, const std::string& name, bool circular) {
    Line* line = new Line(number, name, circular);
    lines[number] = line;
    metro.AddLine(line);
}

// Станции
void MetroAll::addUsualStation(const std::string& name, int traffic, int lineNumber) {
    stations[name] = new UsualStation(name, traffic, lineNumber);
}

void MetroAll::addTransferStation(const std::string& name, int traffic, int lineNumber, int platformCount) {
    stations[name] = new TransferStation(name, traffic, lineNumber, platformCount);
}

void MetroAll::addCrossPlatformStation(const std::string& name, int traffic, int lineNumber) {
    stations[name] = new CrossPlatformStation(name, traffic);
}

// Привязка станций к линиям
void MetroAll::addStationToLine(int lineNumber, const std::string& stationName) {
    Line* line = lines[lineNumber];
    BaseStation* station = stations[stationName];
    
    line->AddStation(station);
    
    line->GetStationIndices()[station] = line->GetStations().size() - 1;
    
    metro.AddStationToLine(line, station);
}

// Перегоны
void MetroAll::addSpan(int lineNumber, const std::string& from, const std::string& to, int time) {
    lines[lineNumber]->AddSpan(stations[from], stations[to], time);
}

// Пересадки
void MetroAll::addTransfer(const std::string& from, const std::string& to, int time, int averageTraffic) {
    auto* ts = dynamic_cast<TransferStation*>(stations[from]);
    auto* target = dynamic_cast<TransferStation*>(stations[to]);
    if (ts && target)
        ts->AddTransfer(target, time, averageTraffic);
}

// Маршрут
std::pair<std::vector<std::string>, int>
MetroAll::shortestRoute(const std::string& from, const std::string& to)
{
    try {
        if (stations.find(from) == stations.end()) {
            throw std::runtime_error("Станция '" + from + "' не найдена");
        }
        
        if (stations.find(to) == stations.end()) {
            throw std::runtime_error("Станция '" + to + "' не найдена");
        }

        if (from == to) {
            throw std::runtime_error("Начальная и конечная станции совпадают");
        }
        
        Route route = metro.FindShortestRoute(stations[from], stations[to]);

        std::vector<std::string> names;

        for (auto* s : route.stations)
            names.push_back(s->GetName());

        return {names, route.totalTime};
        
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Ошибка поиска маршрута: ") + e.what());
    }
}
