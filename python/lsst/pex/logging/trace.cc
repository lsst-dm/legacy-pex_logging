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

#include "lsst/pex/logging/Trace.h"

using namespace lsst::pex::logging;

namespace py = pybind11;

PYBIND11_PLUGIN(_trace) {
    py::module mod("_trace", "Access to the classes from the pex logging Trace library");

    py::class_<Trace> cls(mod, "Trace");

#if !LSST_NO_TRACE
    cls.def(py::init<const std::string&, const int, const char *>());
#endif

    cls.def_static("setVerbosity", (void (*)(const std::string&)) &Trace::setVerbosity);
    cls.def_static("setVerbosity", (void (*)(const std::string&, const int)) &Trace::setVerbosity);
    cls.def_static("getVerbosity", &Trace::getVerbosity);

    return mod.ptr();
}
