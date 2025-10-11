#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>
#include "base_station.h"

int main() {
    BaseStation* station1 = new BaseStation("Station 1", 1000, 1); // назваине, траффик, линия
    BaseStation* station2 = new BaseStation("Station 2", 1200, 1);
    BaseStation* station3 = new BaseStation("Station 3", -900, 2);

    assert(station1->GetStationNumber() == 1);
    assert(station2->GetStationNumber() == 2);
    assert(station3->GetStationNumber() == 3);

    assert(station1->GetName() == "Station 1");
    assert(station2->GetAverageTraffic() == 1200);
    assert(station3->GetLineNumber() == 2);

    station1->SetAverageTraffic(1100);
    assert(station1->GetAverageTraffic() == 1100);

    std::cout << "Hello, World!" << std::endl; // все пройдено

    delete station1;
    delete station2;
    delete station3;

    return 0;
}