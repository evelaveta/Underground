#pragma once
#include <vector>
#include <utility>
#include "usual_station.h"
class TransferStation; // станция пересадки 

struct Transfer { // пересадка (они связаны со станцией, так что отдельный файл не создавала)
    TransferStation* toStation;
    int transferTime;
    int averageTraffic;

    Transfer(TransferStation* to, int time, int traffic)
        : toStation(to), transferTime(time), averageTraffic(traffic) {}
};

class TransferStation : public UsualStation {
public:
    TransferStation(const std::string& name, int averageTraffic, int lineNumber, int stationNumber)
        : UsualStation(name, averageTraffic, lineNumber) {}

    void AddTransfer(TransferStation* station, int transferTime, int averageTraffic) { // заполняем массив пересадок
        transfers_.push_back(Transfer(station, transferTime, averageTraffic)); 
    }

    std::vector<Transfer>& GetTransfers() { // доступ к массиву
        return transfers_;
    }

    std::vector<Transfer> transfers_; // набор пересадок на другие станции (возможно, надо было сделать прайват)
};
