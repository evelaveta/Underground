#pragma once
#include <string>
#include <cassert>

class BaseStation {
public:
    BaseStation(const std::string& name, int averageTraffic, int lineNumber)
        : name_(name), averageTraffic_(averageTraffic), lineNumber_(lineNumber), number_(++counter_) {
            assert(lineNumber > 0 && "lineNumber < 0");
            assert(averageTraffic >= 0 && "averageTraffic < 0");
        }

    virtual ~BaseStation() = default;

    const std::string& GetName() const { return name_; }
    int GetAverageTraffic() const { return averageTraffic_; }
    int GetLineNumber() const { return lineNumber_; }
    int GetStationNumber() const { return number_; }

    void SetAverageTraffic(int traffic) { averageTraffic_ = traffic; }

    private:
    std::string name_;
    int averageTraffic_;
    int lineNumber_;
    int number_;

    static int counter_;
};

int BaseStation::counter_ = 0;