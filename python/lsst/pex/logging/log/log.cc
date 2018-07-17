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

#include "lsst/pex/logging/Log.h"
#include "lsst/pex/logging/FileDestination.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace pex {
namespace logging {

PYBIND11_MODULE(log, mod) {
    py::module::import("lsst.daf.base");

    py::class_<Log, std::shared_ptr<Log>> cls(mod, "Log");

    cls.def_readonly_static("DEBUG", &Log::DEBUG);
    cls.def_readonly_static("INFO", &Log::INFO);
    cls.def_readonly_static("WARN", &Log::WARN);
    cls.def_readonly_static("INHERIT_THRESHOLD", &Log::INHERIT_THRESHOLD);
    cls.def_readonly_static("FATAL", &Log::FATAL);

    cls.def(py::init<const int, const std::string &>(),
            "threshold"_a = Log::INFO, "name"_a = "");
    cls.def(py::init<const Log &, const std::string &, int>(),
            "parent"_a, "childName"_a, "threshold"_a = Log::INHERIT_THRESHOLD);

    cls.def("getName", &Log::getName);
    cls.def("getThreshold", &Log::getThreshold);
    cls.def("setThreshold", &Log::setThreshold);
    cls.def("sends", &Log::sends);
    cls.def("resetThreshold", &Log::resetThreshold);
    cls.def("setThresholdFor", &Log::setThresholdFor);
    cls.def("getThresholdFor", &Log::getThresholdFor);
    cls.def("willShowAll", &Log::willShowAll);
    cls.def("setShowAll", &Log::setShowAll);
    cls.def("resetShowAll", &Log::resetShowAll);
    cls.def("addLabel", &Log::addLabel);
    cls.def("log",
            (void (Log::*)(int, const std::string &, const lsst::daf::base::PropertySet &)) & Log::log);
    cls.def("log", (void (Log::*)(int, const std::string &)) & Log::log);
    cls.def("send", &Log::send);
    cls.def("addDestination",
            [](Log &l, const std::string &filepath, bool verbose = false,
               int threshold = lsst::pex::logging::threshold::PASS_ALL) {
                std::shared_ptr<lsst::pex::logging::LogDestination> fdest(
                        new lsst::pex::logging::FileDestination(filepath, verbose, threshold));
                l.addDestination(fdest);
            },
            "filepath"_a, "verbose"_a = false,
            "threshold"_a = lsst::pex::logging::threshold::PASS_ALL);
    cls.def("markPersistent", &Log::markPersistent);
    cls.def_static("getDefaultLog", &Log::getDefaultLog);
    cls.def_static("closeDefaultLog", &Log::closeDefaultLog);
    cls.def("reset", &Log::reset);
    cls.def("logdebug",
            (void (Log::*)(const std::string &, const lsst::daf::base::PropertySet &)) & Log::logdebug);
    cls.def("logdebug", (void (Log::*)(const std::string &)) & Log::logdebug);
    cls.def("info", (void (Log::*)(const std::string &, const lsst::daf::base::PropertySet &)) & Log::info);
    cls.def("info", (void (Log::*)(const std::string &)) & Log::info);
    cls.def("warn", (void (Log::*)(const std::string &, const lsst::daf::base::PropertySet &)) & Log::warn);
    cls.def("warn", (void (Log::*)(const std::string &)) & Log::warn);
    cls.def("fatal", (void (Log::*)(const std::string &, const lsst::daf::base::PropertySet &)) & Log::fatal);
    cls.def("fatal", (void (Log::*)(const std::string &)) & Log::fatal);

    /* LogRec */
    py::class_<LogRec, std::shared_ptr<LogRec>, LogRecord> clsLogRec(mod, "LogRec");

    clsLogRec.def(py::init<Log &, int>());

    clsLogRec.def("__cpplshift__", [](LogRec &l, const std::string &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, LogRec::Manip r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<int> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<long> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<long long> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<float> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<double> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<bool> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const RecordProperty<std::string> &r) { return l << r; });
    clsLogRec.def("__cpplshift__", [](LogRec &l, const lsst::daf::base::PropertySet &r) { return l << r; });

    py::enum_<LogRec::Manip>(clsLogRec, "Manip").value("endr", LogRec::Manip::endr).export_values();
}

}  // logging
}  // pex
}  // lsst
