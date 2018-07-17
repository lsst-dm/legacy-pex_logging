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

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace pex {
namespace logging {

PYBIND11_MODULE(screenLog, mod) {
    py::module::import("lsst.daf.base");

    py::class_<ScreenLog, std::shared_ptr<ScreenLog>, Log> cls(mod, "ScreenLog");

    cls.def(py::init<bool, int>(), "verbose"_a = false, "threshold"_a = Log::INFO);
    cls.def(py::init<const lsst::daf::base::PropertySet&, bool, int>(), "preamble"_a,
            "verbose"_a = false, "threshold"_a = Log::INFO);
    cls.def("getScreenThreshold", &ScreenLog::getScreenThreshold);
    cls.def("setScreenThreshold", &ScreenLog::setScreenThreshold);
    cls.def("setScreenVerbose", &ScreenLog::setScreenVerbose);
    cls.def("isScreenVerbose", &ScreenLog::isScreenVerbose);
    cls.def_static("createDefaultLog", (void (*)(bool, int)) & ScreenLog::createDefaultLog,
                   "verbose"_a = false, "threshold"_a = Log::INFO);
    cls.def_static("createDefaultLog",
                   (void (*)(const lsst::daf::base::PropertySet&, bool, int)) & ScreenLog::createDefaultLog,
                   "preamble"_a, "verbose"_a = false, "threshold"_a = Log::INFO);
}

}  // logging
}  // pex
}  // lsst
