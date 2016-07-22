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

#include "lsst/pex/logging/BlockTimingLog.h"

using namespace lsst::pex::logging;

namespace py = pybind11;

PYBIND11_PLUGIN(_blockTimingLog) {
    py::module mod("_blockTimingLog", "Access to the classes from the pex logging BlockTimingLog library");

    py::class_<BlockTimingLog> cls(mod, "BlockTimingLog", py::base<Log>());

    py::enum_<BlockTimingLog::usageData>(cls, "usageData")
        .value("NOUDATA", BlockTimingLog::usageData::NOUDATA)
        .value("UTIME", BlockTimingLog::usageData::UTIME)
        .value("STIME", BlockTimingLog::usageData::STIME)
        .value("SUTIME", BlockTimingLog::usageData::SUTIME)
        .value("MEMSZ", BlockTimingLog::usageData::MEMSZ)
        .value("NSWAP", BlockTimingLog::usageData::NSWAP)
        .value("BLKIN", BlockTimingLog::usageData::BLKIN)
        .value("BLKOUT", BlockTimingLog::usageData::BLKOUT)
        .value("BLKIO", BlockTimingLog::usageData::BLKIO)
        .value("MINFLT", BlockTimingLog::usageData::MINFLT)
        .value("MAJFLT", BlockTimingLog::usageData::MAJFLT)
        .value("LINUXUDATA", BlockTimingLog::usageData::LINUXUDATA)
        .value("ALLUDATA", BlockTimingLog::usageData::ALLUDATA)
        .value("PARENTUDATA", BlockTimingLog::usageData::PARENTUDATA)
        .export_values();

    cls.def(py::init<const Log&, const std::string&, int, int, const std::string&>(),
        py::arg("parent"), py::arg("name"), py::arg("tracelev")=BlockTimingLog::INSTRUM, py::arg("usageFlags")=BlockTimingLog::PARENTUDATA, py::arg("funcName")="");

    cls.def_readonly_static("INSTRUM", &BlockTimingLog::INSTRUM);
    cls.def_readonly_static("STATUS", &BlockTimingLog::STATUS);
    cls.def_readonly_static("START", &BlockTimingLog::START);
    cls.def_readonly_static("END", &BlockTimingLog::END);

    cls.def("getUsageFlags", &BlockTimingLog::getUsageFlags);
    cls.def("setUsageFlags", &BlockTimingLog::setUsageFlags);
    cls.def("addUsageFlags", &BlockTimingLog::addUsageFlags);
    cls.def("createForBlock", &BlockTimingLog::createForBlock,
        py::arg("name"), py::arg("tracelev")=Log::INHERIT_THRESHOLD, py::arg("funcName")="");
    cls.def("start", (void (BlockTimingLog::*)(void)) &BlockTimingLog::start);
    cls.def("start", (void (BlockTimingLog::*)(const std::string&)) &BlockTimingLog::start);
    cls.def("done", &BlockTimingLog::done);
    cls.def("getInstrumentationLevel", &BlockTimingLog::getInstrumentationLevel);
    cls.def("getFunctionName", &BlockTimingLog::getFunctionName);
    cls.def("addUsageProps", &BlockTimingLog::addUsageProps);

    return mod.ptr();
}
