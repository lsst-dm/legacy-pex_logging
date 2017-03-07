/*
 * LSST Data Management System
 * Copyright 2008-2016  AURA/LSST.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <https://www.lsstcorp.org/LegalNotices/>.
 */

#include "pybind11/pybind11.h"

#include "lsst/pex/logging/Debug.h"

using namespace lsst::pex::logging;

namespace py = pybind11;

PYBIND11_PLUGIN(_debug) {
    py::module mod("_debug", "Access to the classes from the pex logging Debug library");

    py::class_<Debug> cls(mod, "Debug", py::base<Log>());

    cls.def(py::init<const std::string&, int>(), py::arg("name"),
            py::arg("verbosity") = -1 * Log::INHERIT_THRESHOLD);

    cls.def(py::init<const Log&, const std::string&, int>(), py::arg("parent"), py::arg("name"),
            py::arg("verbosity") = -1 * Log::INHERIT_THRESHOLD);

    cls.def("debug", (void (Debug::*)(int, const std::string&)) & Debug::debug);

    return mod.ptr();
}
