#pragma once
#include <vector>
#include <string>
#include <map>
#include "base_station.h"
#include "span.h"

class Line { // линия
public:
    Line(int lineNumber, const std::string& name, bool isCircular = false) // фолс дефолтный
        : lineNumber_(lineNumber), name_(name), isCircular_(isCircular) {}

    void AddStation(BaseStation* station) { // заполняем линию станциями
        stations_.push_back(station);
    }

    void AddSpan(BaseStation* station1, BaseStation* station2, int travelTime) { // и перегонами
        spans_.push_back(Span(station1, station2, travelTime));
    }

    std::vector<std::pair<BaseStation*, int> > GetNeighbors(BaseStation* station) { // ищем соседей для станции
        std::vector<std::pair<BaseStation*, int> > neighbors;

        std::map<BaseStation*, int>::iterator it = stationIndices_.find(station);
        if (it == stationIndices_.end()) return neighbors; // нет такого элемента

        int index = it->second; // индекс на линии

        if (index > 0) { // для не первой станции на линии левый сосед
            int travelTime = GetTravelTimeBetween(station, stations_[index - 1]);
            neighbors.push_back(std::make_pair(stations_[index - 1], travelTime));
        } else if (isCircular_) { // для круговой неважно
            int travelTime = GetTravelTimeBetween(station, stations_.back());
            neighbors.push_back(std::make_pair(stations_.back(), travelTime));
        }

        if (index < static_cast<int>(stations_.size()) - 1) { // для не последней станции на линии правый сосед
            int travelTime = GetTravelTimeBetween(station, stations_[index + 1]);
            neighbors.push_back(std::make_pair(stations_[index + 1], travelTime));
        } else if (isCircular_) {
            int travelTime = GetTravelTimeBetween(station, stations_.front());
            neighbors.push_back(std::make_pair(stations_.front(), travelTime));
        }

        return neighbors; // в соседе (соседях) номер и время в пути
    }

    int GetTravelTime(BaseStation* from, BaseStation* to) { // время в пути
        std::map<BaseStation*, int>::iterator fromIt = stationIndices_.find(from);
        std::map<BaseStation*, int>::iterator toIt = stationIndices_.find(to);

        if (fromIt == stationIndices_.end() || toIt == stationIndices_.end())
            return -1;

        int fromIndex = fromIt->second;
        int toIndex = toIt->second;

        int directPath = 0;
        int min_idx = std::min(fromIndex, toIndex); // реализация "двусвязности" (если начальная станция на линии > конечной)
        int max_idx = std::max(fromIndex, toIndex);

        for (int i = min_idx; i < max_idx; ++i) {
            directPath += GetTravelTimeBetween(stations_[i], stations_[i + 1]);
        }

        if (isCircular_) {
            int circlePath = 0;
            for (int i = max_idx; i < static_cast<int>(stations_.size()) - 1; ++i) {
                circlePath += GetTravelTimeBetween(stations_[i], stations_[i + 1]);
            }
            circlePath += GetTravelTimeBetween(stations_.back(), stations_.front());
            for (int i = 0; i < min_idx; ++i) {
                circlePath += GetTravelTimeBetween(stations_[i], stations_[i + 1]);
            }

            return std::min(directPath, circlePath); // минимальное время в пути (по часовой стрелке или против)
        }

        return directPath;
    }

    // Геттеры для доступа к приватным полям
    int GetLineNumber() const { return lineNumber_; }
    const std::string& GetName() const { return name_; }
    bool IsCircular() const { return isCircular_; }
    const std::vector<BaseStation*>& GetStations() const { return stations_; }
    std::vector<BaseStation*>& GetStations() { return stations_; }
    std::map<BaseStation*, int>& GetStationIndices() { return stationIndices_; }
    const std::map<BaseStation*, int>& GetStationIndices() const { return stationIndices_; }

private:
    int lineNumber_; // возможно, надо было сделать прайват, но я об этом подумаю после написания класса metro
    std::string name_;
    bool isCircular_;
    std::vector<BaseStation*> stations_;
    std::vector<Span> spans_;
    std::map<BaseStation*, int> stationIndices_;

    int GetTravelTimeBetween(BaseStation* station1, BaseStation* station2) { // время в пути между соседними станциями
        for (size_t i = 0; i < spans_.size(); ++i) {
            if ((spans_[i].GetStation1() == station1 && spans_[i].GetStation2() == station2) ||
                (spans_[i].GetStation1() == station2 && spans_[i].GetStation2() == station1)) {
                return spans_[i].GetTravelTime();
            }
        }
        return -1;
    }
};
