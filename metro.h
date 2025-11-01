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

// Структура для хранения узла в очереди
struct QueueNode {
    int distance;
    BaseStation* station;

    QueueNode(int d, BaseStation* s) : distance(d), station(s) {}

    // Компаратор для приоритетной очереди (минимальная куча)
    bool operator>(const QueueNode& other) const {
        return distance > other.distance;
    }
};

class Metro {
public:
    Metro() {}

    void AddLine(Line* line) {
        lines_.push_back(line);

        // Строим карту станций для быстрого поиска
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

        // Алгоритм Дейкстры
        std::map<BaseStation*, int> distances;
        std::map<BaseStation*, BaseStation*> previous;
        std::set<BaseStation*> visited;

        // Приоритетная очередь
        std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode> > pq;

        // Инициализация
        distances[from] = 0;
        pq.push(QueueNode(0, from));

        while (!pq.empty()) {
            QueueNode current = pq.top();
            pq.pop();

            BaseStation* currentStation = current.station;
            int currentDist = current.distance;

            // Если уже посещали эту станцию, пропускаем
            if (visited.find(currentStation) != visited.end()) {
                continue;
            }
            visited.insert(currentStation);

            if (currentStation == to) {
                break;
            }

            // Получаем всех соседей текущей станции
            std::vector<std::pair<BaseStation*, int> > neighbors = GetAllNeighbors(currentStation);

            for (size_t i = 0; i < neighbors.size(); ++i) {
                BaseStation* nextStation = neighbors[i].first;
                int travelTime = neighbors[i].second;

                // Пропускаем уже посещенные станции
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

        // Восстанавливаем путь
        Route route;
        if (distances.find(to) == distances.end()) {
            // Путь не найден
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

        // Соседи на той же линии
        std::map<BaseStation*, std::vector<Line*> >::iterator it = stationToLines_.find(station);
        if (it != stationToLines_.end()) {
            for (size_t i = 0; i < it->second.size(); ++i) {
                Line* line = it->second[i];
                std::vector<std::pair<BaseStation*, int> > lineNeighbors = line->GetNeighbors(station);
                allNeighbors.insert(allNeighbors.end(), lineNeighbors.begin(), lineNeighbors.end());
            }
        }

        // Пересадки на другие линии (TransferStation)
        TransferStation* transferStation = dynamic_cast<TransferStation*>(station);
        if (transferStation) {
            std::vector<Transfer>& transfers = transferStation->GetTransfers();
            for (size_t i = 0; i < transfers.size(); ++i) {
                allNeighbors.push_back(std::make_pair(transfers[i].toStation, transfers[i].transferTime));
            }
        }

        // Кросс-платформенные пересадки (CrossPlatformStation)
        CrossPlatformStation* crossPlatformStation = dynamic_cast<CrossPlatformStation*>(station);
        if (crossPlatformStation) {
            // Для кросс-платформенных станций время пересадки равно 0
            // Находим все линии, на которых находится эта станция
            std::vector<std::pair<int, int> >& lines = crossPlatformStation->GetLines();
            for (size_t i = 0; i < lines.size(); ++i) {
                int lineNumber = lines[i].first;
                // Находим линию по номеру
                for (size_t j = 0; j < lines_.size(); ++j) {
                    if (lines_[j]->GetLineNumber() == lineNumber) {
                        // Получаем соседей на этой линии
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
