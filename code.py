import sys
import pymetro

m = pymetro.Metro()

m.addLine(1, "Red", False)

m.addUsualStation("A", 100, 1)
m.addUsualStation("B", 120, 1)
m.addUsualStation("C", 90, 1)

m.addStationToLine(1, "A")
m.addStationToLine(1, "B")
m.addStationToLine(1, "C")

m.addSpan(1, "A", "B", 2)
m.addSpan(1, "B", "C", 3)

path, time = m.shortestRoute("A", "C")

print("Маршрут:", path)
print("Время:", time)
