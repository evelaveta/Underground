#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "metroall.h"

namespace py = pybind11;

PYBIND11_MODULE(pymetro, m) {
    py::class_<MetroAll>(m, "Metro")
        .def(py::init<>())

        .def("addLine", &MetroAll::addLine)
        .def("addUsualStation", &MetroAll::addUsualStation)
        .def("addTransferStation", &MetroAll::addTransferStation)
        .def("addCrossPlatformStation", &MetroAll::addCrossPlatformStation)

        .def("addStationToLine", &MetroAll::addStationToLine)
        .def("addSpan", &MetroAll::addSpan)
        .def("addTransfer", &MetroAll::addTransfer)

        .def("shortestRoute", &MetroAll::shortestRoute);
}
