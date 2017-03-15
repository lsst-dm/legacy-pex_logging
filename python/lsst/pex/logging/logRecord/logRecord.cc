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

#include "lsst/pex/logging/LogRecord.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace pex {
namespace logging {

PYBIND11_PLUGIN(logRecord) {
    py::module mod("logRecord");

    py::class_<LogRecord, std::shared_ptr<LogRecord>> cls(mod, "LogRecord");

    cls.def(py::init<int, int, bool>(), "threshold"_a, "importance"_a,
            "showAll"_a = false);
    cls.def(py::init<int, int, const lsst::daf::base::PropertySet&, bool>(), "threshold"_a,
            "importance"_a, "preamble"_a, "showAll"_a = false);

    cls.def("addComment", (void (LogRecord::*)(const std::string&)) & LogRecord::addComment);
    cls.def("addPropertyInt", (void (LogRecord::*)(const std::string&, const int&)) & LogRecord::addProperty);
    cls.def("addPropertyLong",
            (void (LogRecord::*)(const std::string&, const long&)) & LogRecord::addProperty);
    cls.def("addPropertyLongLong",
            (void (LogRecord::*)(const std::string&, const long long&)) & LogRecord::addProperty);
    cls.def("addPropertyFloat",
            (void (LogRecord::*)(const std::string&, const float&)) & LogRecord::addProperty);
    cls.def("addPropertyDouble",
            (void (LogRecord::*)(const std::string&, const double&)) & LogRecord::addProperty);
    cls.def("addPropertyBool",
            (void (LogRecord::*)(const std::string&, const bool&)) & LogRecord::addProperty);
    cls.def("addPropertyString",
            (void (LogRecord::*)(const std::string&, const std::string&)) & LogRecord::addProperty);
    cls.def("addProperties",
            (void (LogRecord::*)(const lsst::daf::base::PropertySet::Ptr&)) & LogRecord::addProperties);
    cls.def("getProperties", (lsst::daf::base::PropertySet & (LogRecord::*)()) & LogRecord::getProperties,
            py::return_value_policy::reference_internal);

    return mod.ptr();
}

}  // logging
}  // pex
}  // lsst
