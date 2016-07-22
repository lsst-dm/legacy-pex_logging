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

#include "lsst/pex/logging/ScreenLog.h"

using namespace lsst::pex::logging;

namespace py = pybind11;

PYBIND11_PLUGIN(_screenLog) {
    py::module mod("_screenLog", "Access to the classes from the pex logging ScreenLog library");

    py::class_<ScreenLog> cls(mod, "ScreenLog", py::base<Log>());

    cls.def(py::init<bool, int>(),
        py::arg("verbose")=false, py::arg("threshold")=Log::INFO);
    cls.def(py::init<const lsst::daf::base::PropertySet&, bool, int>(),
        py::arg("preamble"), py::arg("verbose")=false, py::arg("threshold")=Log::INFO);
    cls.def("getScreenThreshold", &ScreenLog::getScreenThreshold);
    cls.def("setScreenThreshold", &ScreenLog::setScreenThreshold);
    cls.def("setScreenVerbose", &ScreenLog::setScreenVerbose);
    cls.def("isScreenVerbose", &ScreenLog::isScreenVerbose);
    cls.def_static("createDefaultLog", (void (*)(bool, int)) &ScreenLog::createDefaultLog,
        py::arg("verbose")=false, py::arg("threshold")=Log::INFO);
    cls.def_static("createDefaultLog", (void (*)(const lsst::daf::base::PropertySet&, bool, int)) &ScreenLog::createDefaultLog,
        py::arg("preamble"), py::arg("verbose")=false, py::arg("threshold")=Log::INFO);

    return mod.ptr();
}
