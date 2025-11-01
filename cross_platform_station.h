#pragma once
#include <vector>
#include <utility>
#include "transfer_station.h"

class CrossPlatformStation : public TransferStation { // кросс-платформеннвя станция
public:
    CrossPlatformStation(const std::string& name, int averageTraffic)
        : TransferStation(name, averageTraffic, -1, -1) {} // последние параметры -1, т.к. станция не принадлежит одной линии

    void AddLine(int lineNumber, int stationNumber) {
        lines_.push_back(std::make_pair(lineNumber, stationNumber));
        if (GetLineNumber() == -1) {
            SetLineNumber(lineNumber); // но оставлять ее без номера линии и номера станции неудобно, поэтому приваиваем первый попавшийся
            SetNumber(stationNumber);
        }
    }

    std::vector<std::pair<int, int> >& GetLines() { // доступ
        return lines_;
    }

    const std::vector<std::pair<int, int> >& GetLines() const {
        return lines_;
    }

private:
    std::vector<std::pair<int, int> > lines_; // набор номеров линий и станций, на которых базируется станция (возможно, надо было сделать прайват)
};
