#include <iostream>
#include <string>
#include <iomanip>
#include "usual_station.h"
#include "transfer_station.h"
#include "cross_platform_station.h"
#include "span.h"
#include "line.h"

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

    Line line(1, "Green Circle", true);

    BaseStation s1("S1", 80, 1);
    BaseStation s2("S2", 90, 1);
    BaseStation s3("S3", 100, 1);
    BaseStation s4("S4", 110, 1);

    line.AddStation(&s1);
    line.AddStation(&s2);
    line.AddStation(&s3);
    line.AddStation(&s4);

    line.AddSpan(&s1, &s2, 2);
    line.AddSpan(&s2, &s3, 3);
    line.AddSpan(&s3, &s4, 4);
    line.AddSpan(&s4, &s1, 1);

    line.stationIndices_[&s1] = 0;
    line.stationIndices_[&s2] = 1;
    line.stationIndices_[&s3] = 2;
    line.stationIndices_[&s4] = 3;

    int travelTimeClockwise = line.GetTravelTime(&s1, &s3);
    int travelTimeCounter = line.GetTravelTime(&s3, &s1);

    assert(travelTimeClockwise == 5);
    assert(travelTimeCounter == 5);

    travelTimeClockwise = line.GetTravelTime(&s4, &s2);
    assert(travelTimeClockwise == 3);

    return 0;
}
