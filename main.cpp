#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>
#include "usual_station.h"
#include "transfer_station.h"
#include "cross_platform_station.h"
#include "span.h"
#include "line.h"
#include "metro.h"

void TestBasicFunctionality() {
    std::cout << "=== Test 1: Basic Station Functionality ===" << std::endl;

    BaseStation* station1 = new BaseStation("Station 1", 1000, 1); // назваине, траффик, линия
    BaseStation* station2 = new BaseStation("Station 2", 1200, 1);
    BaseStation* station3 = new BaseStation("Station 3", 900, 2);

    assert(station1->GetStationNumber() == 1);
    assert(station2->GetStationNumber() == 2);
    assert(station3->GetStationNumber() == 3);

    assert(station1->GetName() == "Station 1");
    assert(station2->GetAverageTraffic() == 1200);
    assert(station3->GetLineNumber() == 2);

    station1->SetAverageTraffic(1100);
    assert(station1->GetAverageTraffic() == 1100);

    std::cout << "All basic tests passed!" << std::endl;

    delete station1;
    delete station2;
    delete station3;
}

void TestCircularLine() {
    std::cout << "\n=== Test 2: Circular Line ===" << std::endl;

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

    line.GetStationIndices()[&s1] = 0;
    line.GetStationIndices()[&s2] = 1;
    line.GetStationIndices()[&s3] = 2;
    line.GetStationIndices()[&s4] = 3;

    int travelTimeClockwise = line.GetTravelTime(&s1, &s3);
    int travelTimeCounter = line.GetTravelTime(&s3, &s1);

    assert(travelTimeClockwise == 5);
    assert(travelTimeCounter == 5);

    travelTimeClockwise = line.GetTravelTime(&s4, &s2);
    assert(travelTimeClockwise == 3);

    std::cout << "Circular line tests passed!" << std::endl;
}

void TestShortestPath() {
    std::cout << "\n=== Test 3: Shortest Path Finding ===" << std::endl;

    /*
     * Создаём простую сеть метро:
     *
     * Линия 1 (Красная): A -- 5 мин -- B -- 3 мин -- C
     * Линия 2 (Синяя):   D -- 4 мин -- E -- 2 мин -- F
     *
     * Пересадка: C <--> D (время пересадки: 2 мин)
     *
     * Проверяем путь A -> F
     * Ожидаемый маршрут: A -> B -> C -> D -> E -> F
     * Ожидаемое время: 5 + 3 + 2 + 4 + 2 = 16 минут
     */

    // Создаём станции линии 1
    UsualStation* stationA = new UsualStation("A", 1000, 1);
    UsualStation* stationB = new UsualStation("B", 1100, 1);
    TransferStation* stationC = new TransferStation("C", 1200, 1, 3);

    // Создаём станции линии 2
    TransferStation* stationD = new TransferStation("D", 900, 2, 1);
    UsualStation* stationE = new UsualStation("E", 950, 2);
    UsualStation* stationF = new UsualStation("F", 800, 2);

    // Настраиваем пересадку
    stationC->AddTransfer(stationD, 2, 500); // С -> D, 2 минуты
    stationD->AddTransfer(stationC, 2, 500); // D -> C, 2 минуты

    // Создаём линию 1
    Line* line1 = new Line(1, "Red Line", false);
    line1->AddStation(stationA);
    line1->AddStation(stationB);
    line1->AddStation(stationC);

    line1->GetStationIndices()[stationA] = 0;
    line1->GetStationIndices()[stationB] = 1;
    line1->GetStationIndices()[stationC] = 2;

    line1->AddSpan(stationA, stationB, 5);
    line1->AddSpan(stationB, stationC, 3);

    // Создаём линию 2
    Line* line2 = new Line(2, "Blue Line", false);
    line2->AddStation(stationD);
    line2->AddStation(stationE);
    line2->AddStation(stationF);

    line2->GetStationIndices()[stationD] = 0;
    line2->GetStationIndices()[stationE] = 1;
    line2->GetStationIndices()[stationF] = 2;

    line2->AddSpan(stationD, stationE, 4);
    line2->AddSpan(stationE, stationF, 2);

    // Создаём метрополитен
    Metro metro;
    metro.AddLine(line1);
    metro.AddLine(line2);

    // Ищем кратчайший путь от A до F
    Route route = metro.FindShortestRoute(stationA, stationF);

    std::cout << "Route from " << stationA->GetName() << " to " << stationF->GetName() << ":" << std::endl;
    std::cout << "Total time: " << route.totalTime << " minutes" << std::endl;
    std::cout << "Path: ";
    for (size_t i = 0; i < route.stations.size(); ++i) {
        std::cout << route.stations[i]->GetName();
        if (i < route.stations.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;

    // Проверяем результат
    assert(route.totalTime == 16);
    assert(route.stations.size() == 6);
    assert(route.stations[0] == stationA);
    assert(route.stations[5] == stationF);

    std::cout << "Shortest path test passed!" << std::endl;

    // Очистка памяти
    delete stationA;
    delete stationB;
    delete stationC;
    delete stationD;
    delete stationE;
    delete stationF;
    delete line1;
    delete line2;
}

void TestCrossPlatformStation() {
    std::cout << "\n=== Test 4: Cross-Platform Station ===" << std::endl;

    /*
     * Создаём сеть с кросс-платформенной станцией:
     *
     * Линия 1: A -- 3 мин -- X
     * Линия 2: X -- 4 мин -- B
     *
     * X - кросс-платформенная станция (переход мгновенный)
     *
     * Путь A -> B должен занять 3 + 0 + 4 = 7 минут
     */

    UsualStation* stationA = new UsualStation("A", 1000, 1);
    CrossPlatformStation* stationX = new CrossPlatformStation("X", 2000);
    UsualStation* stationB = new UsualStation("B", 1000, 2);

    stationX->AddLine(1, 2);
    stationX->AddLine(2, 1);

    // Линия 1
    Line* line1 = new Line(1, "Line 1", false);
    line1->AddStation(stationA);
    line1->AddStation(stationX);
    line1->GetStationIndices()[stationA] = 0;
    line1->GetStationIndices()[stationX] = 1;
    line1->AddSpan(stationA, stationX, 3);

    // Линия 2
    Line* line2 = new Line(2, "Line 2", false);
    line2->AddStation(stationX);
    line2->AddStation(stationB);
    line2->GetStationIndices()[stationX] = 0;
    line2->GetStationIndices()[stationB] = 1;
    line2->AddSpan(stationX, stationB, 4);

    Metro metro;
    metro.AddLine(line1);
    metro.AddLine(line2);

    Route route = metro.FindShortestRoute(stationA, stationB);

    std::cout << "Route from " << stationA->GetName() << " to " << stationB->GetName() << ":" << std::endl;
    std::cout << "Total time: " << route.totalTime << " minutes" << std::endl;
    std::cout << "Path: ";
    for (size_t i = 0; i < route.stations.size(); ++i) {
        std::cout << route.stations[i]->GetName();
        if (i < route.stations.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;

    assert(route.totalTime == 7);

    std::cout << "Cross-platform station test passed!" << std::endl;

    delete stationA;
    delete stationX;
    delete stationB;
    delete line1;
    delete line2;
}

int main() {
    TestBasicFunctionality();
    TestCircularLine();
    TestShortestPath();
    TestCrossPlatformStation();

    std::cout << "\n=== All tests passed successfully! ===" << std::endl;

    return 0;
}
