#pragma once
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <set>
#include "line.h"
#include "transfer_station.h"
#include "cross_platform_station.h"

struct Route {
    std::vector<BaseStation*> stations;
    int totalTime;

    Route() : totalTime(0) {}
};

// узел
struct QueueNode {
    int distance;
    BaseStation* station;

    QueueNode(int d, BaseStation* s) : distance(d), station(s) {}
    bool operator>(const QueueNode& other) const {
        return distance > other.distance;
    }
};

class Metro {
public:
    Metro() {}

    void AddLine(Line* line) {
        lines_.push_back(line);

        std::vector<BaseStation*>& stations = line->GetStations();
        for (size_t i = 0; i < stations.size(); ++i) {
            stationToLines_[stations[i]].push_back(line);
        }
    }

    Route FindShortestRoute(BaseStation* from, BaseStation* to) {
        if (from == to) {
            Route route;
            route.stations.push_back(from);
            route.totalTime = 0;
            return route;
        }

        // дейкстра
        std::map<BaseStation*, int> distances;
        std::map<BaseStation*, BaseStation*> previous;
        std::set<BaseStation*> visited;

        // приоритетная очередь, используем std::greater
        std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode> > pq;

        // первая вершина
        distances[from] = 0;
        pq.push(QueueNode(0, from));

        while (!pq.empty()) {
            QueueNode current = pq.top();
            pq.pop();

            BaseStation* currentStation = current.station;
            int currentDist = current.distance;

            // скип станции если она была посещена
            if (visited.find(currentStation) != visited.end()) {
                continue;
            }
            visited.insert(currentStation);

            if (currentStation == to) {
                break;
            }

            // получение соседей
            std::vector<std::pair<BaseStation*, int> > neighbors = GetAllNeighbors(currentStation);

            for (size_t i = 0; i < neighbors.size(); ++i) {
                BaseStation* nextStation = neighbors[i].first;
                int travelTime = neighbors[i].second;

                // если станция уже посещена - скип
                if (visited.find(nextStation) != visited.end()) {
                    continue;
                }

                int newDist = currentDist + travelTime;

                if (distances.find(nextStation) == distances.end() || newDist < distances[nextStation]) {
                    distances[nextStation] = newDist;
                    previous[nextStation] = currentStation;
                    pq.push(QueueNode(newDist, nextStation));
                }
            }
        }

        // восстановление ответа
        Route route;
        if (distances.find(to) == distances.end()) {
            // нету пути
            return route;
        }

        route.totalTime = distances[to];

        BaseStation* current = to;
        while (current != NULL) {
            route.stations.push_back(current);
            std::map<BaseStation*, BaseStation*>::iterator it = previous.find(current);
            if (it != previous.end()) {
                current = it->second;
            } else {
                break;
            }
        }

        std::reverse(route.stations.begin(), route.stations.end());

        return route;
    }

    // Геттеры
    const std::vector<Line*>& GetLines() const { return lines_; }

private:
    std::vector<Line*> lines_;
    std::map<BaseStation*, std::vector<Line*> > stationToLines_;

    std::vector<std::pair<BaseStation*, int> > GetAllNeighbors(BaseStation* station) {
        std::vector<std::pair<BaseStation*, int> > allNeighbors;

        // соседние вершины на той же линии
        std::map<BaseStation*, std::vector<Line*> >::iterator it = stationToLines_.find(station);
        if (it != stationToLines_.end()) {
            for (size_t i = 0; i < it->second.size(); ++i) {
                Line* line = it->second[i];
                std::vector<std::pair<BaseStation*, int> > lineNeighbors = line->GetNeighbors(station);
                allNeighbors.insert(allNeighbors.end(), lineNeighbors.begin(), lineNeighbors.end());
            }
        }

        // пересадка на другие линии
        TransferStation* transferStation = dynamic_cast<TransferStation*>(station);
        if (transferStation) {
            std::vector<Transfer>& transfers = transferStation->GetTransfers();
            for (size_t i = 0; i < transfers.size(); ++i) {
                allNeighbors.push_back(std::make_pair(transfers[i].toStation, transfers[i].transferTime));
            }
        }

        // кросс-платформенные пересадки
        CrossPlatformStation* crossPlatformStation = dynamic_cast<CrossPlatformStation*>(station);
        if (crossPlatformStation) {
            // находим все линии для данной станции
            std::vector<std::pair<int, int> >& lines = crossPlatformStation->GetLines();
            for (size_t i = 0; i < lines.size(); ++i) {
                int lineNumber = lines[i].first;
                // поиск линии по номеру
                for (size_t j = 0; j < lines_.size(); ++j) {
                    if (lines_[j]->GetLineNumber() == lineNumber) {
                        // получение соседей
                        std::vector<std::pair<BaseStation*, int> > lineNeighbors =
                            lines_[j]->GetNeighbors(crossPlatformStation);
                        allNeighbors.insert(allNeighbors.end(), lineNeighbors.begin(), lineNeighbors.end());
                    }
                }
            }
        }

        return allNeighbors;
    }
};
