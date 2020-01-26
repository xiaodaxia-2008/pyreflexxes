/*
 * Copyright 2019 Kim Lindberg Schwaner <kils@mmmi.sdu.dk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "makelist.h"
#include "positiontrajectory.h"
#include "rmlerror.h"
#include "vector_type_caster.h"

#include <ReflexxesAPI.h>

#include <pybind11/pybind11.h>

#if defined(RML_TYPE_II)
#define MODULE_NAME rml_type_ii
#elif defined(RML_TYPE_IV)
#define MODULE_NAME rml_type_iv
#endif

PYBIND11_MODULE(MODULE_NAME, m)
{
    namespace py = pybind11;
    using namespace pybind11::literals;

    py::register_exception<RMLError>(m, "RMLError");

    // ReflexxesAPI
    auto reflexxes_api = py::class_<ReflexxesAPI>(m, "ReflexxesAPI")
        .def(py::init<unsigned, double>())
        .def("RMLPosition", &ReflexxesAPI::RMLPosition)
        .def("RMLPositionAtAGivenSampleTime", &ReflexxesAPI::RMLPositionAtAGivenSampleTime)
        .def("RMLVelocity", &ReflexxesAPI::RMLVelocity)
        .def("RMLVelocityAtAGivenSampleTime", &ReflexxesAPI::RMLVelocityAtAGivenSampleTime)
#if defined(RML_TYPE_IV)
        .def(py::init<unsigned, double, unsigned>())
        .def(py::init<unsigned, double, unsigned, double>())
        .def("SetupOverrideFilter", &ReflexxesAPI::SetupOverrideFilter)
#endif
    ;
    py::enum_<ReflexxesAPI::RMLResultValue>(reflexxes_api, "RMLResultValue")
        .value("RML_WORKING", ReflexxesAPI::RML_WORKING)
        .value("RML_FINAL_STATE_REACHED", ReflexxesAPI::RML_FINAL_STATE_REACHED)
        .value("RML_ERROR", ReflexxesAPI::RML_ERROR)
        .value("RML_ERROR_INVALID_INPUT_VALUES", ReflexxesAPI::RML_ERROR_INVALID_INPUT_VALUES)
        .value("RML_ERROR_EXECUTION_TIME_CALCULATION", ReflexxesAPI::RML_ERROR_EXECUTION_TIME_CALCULATION)
        .value("RML_ERROR_SYNCHRONIZATION", ReflexxesAPI::RML_ERROR_SYNCHRONIZATION)
        .value("RML_ERROR_NUMBER_OF_DOFS", ReflexxesAPI::RML_ERROR_NUMBER_OF_DOFS)
        .value("RML_ERROR_NO_PHASE_SYNCHRONIZATION", ReflexxesAPI::RML_ERROR_NO_PHASE_SYNCHRONIZATION)
        .value("RML_ERROR_NULL_POINTER", ReflexxesAPI::RML_ERROR_NULL_POINTER)
        .value("RML_ERROR_EXECUTION_TIME_TOO_BIG", ReflexxesAPI::RML_ERROR_EXECUTION_TIME_TOO_BIG)
        .value("RML_ERROR_USER_TIME_OUT_OF_RANGE", ReflexxesAPI::RML_ERROR_USER_TIME_OUT_OF_RANGE)
#if defined(RML_TYPE_IV)
        .value("RML_NO_ERROR", ReflexxesAPI::RML_NO_ERROR)
        .value("RML_ERROR_POSITIONAL_LIMITS", ReflexxesAPI::RML_ERROR_POSITIONAL_LIMITS)
        .value("RML_ERROR_OVERRIDE_OUT_OF_RANGE", ReflexxesAPI::RML_ERROR_OVERRIDE_OUT_OF_RANGE)
#endif
        .export_values()
    ;

    // RMLFlags
    auto rml_flags = py::class_<RMLFlags>(m, "RMLFlags")
        .def("__eq__", &RMLFlags::operator==)
        .def("__ne__", &RMLFlags::operator!=)
        .def_readwrite("SynchronizationBehavior", &RMLFlags::SynchronizationBehavior)
        .def_readwrite("EnableTheCalculationOfTheExtremumMotionStates", &RMLFlags::EnableTheCalculationOfTheExtremumMotionStates)
#if defined(RML_TYPE_IV)
        .def_readwrite("PositionalLimitsBehavior", &RMLFlags::PositionalLimitsBehavior)
#endif
    ;
    py::enum_<RMLFlags::SyncBehaviorEnum>(rml_flags, "SyncBehaviorEnum")
        .value("PHASE_SYNCHRONIZATION_IF_POSSIBLE", RMLFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE)
        .value("ONLY_TIME_SYNCHRONIZATION", RMLFlags::ONLY_TIME_SYNCHRONIZATION)
        .value("ONLY_PHASE_SYNCHRONIZATION", RMLFlags::ONLY_PHASE_SYNCHRONIZATION)
        .value("NO_SYNCHRONIZATION", RMLFlags::NO_SYNCHRONIZATION)
        .export_values()
    ;
#if defined(RML_TYPE_IV)
    py::enum_<RMLFlags::PositionalLimitsEnum>(rml_flags, "PositionalLimitsEnum")
        .value("POSITIONAL_LIMITS_IGNORE", RMLFlags::POSITIONAL_LIMITS_IGNORE)
        .value("POSITIONAL_LIMITS_ERROR_MSG_ONLY", RMLFlags::POSITIONAL_LIMITS_ERROR_MSG_ONLY)
        .value("POSITIONAL_LIMITS_ACTIVELY_PREVENT", RMLFlags::POSITIONAL_LIMITS_ACTIVELY_PREVENT)
        .export_values()
    ;
#endif

    // RMLPositionFlags
    auto rml_position_flags = py::class_<RMLPositionFlags, RMLFlags>(m, "RMLPositionFlags")
        .def(py::init<>())
        .def("__eq__", &RMLPositionFlags::operator==)
        .def("__ne__", &RMLPositionFlags::operator!=)
        .def_readwrite("BehaviorAfterFinalStateOfMotionIsReached", &RMLPositionFlags::BehaviorAfterFinalStateOfMotionIsReached)
        .def_readwrite("KeepCurrentVelocityInCaseOfFallbackStrategy", &RMLPositionFlags::KeepCurrentVelocityInCaseOfFallbackStrategy)
    ;
    py::enum_<RMLPositionFlags::FinalMotionBehaviorEnum>(rml_position_flags, "FinalMotionBehaviorEnum")
        .value("KEEP_TARGET_VELOCITY", RMLPositionFlags::KEEP_TARGET_VELOCITY)
        .value("RECOMPUTE_TRAJECTORY", RMLPositionFlags::RECOMPUTE_TRAJECTORY)
        .export_values()
    ;
#if defined(RML_TYPE_IV)
    py::enum_<RMLPositionFlags::BehaviorIfInitialStateBreachesConstraintsEnum>(rml_position_flags, "BehaviorIfInitialStateBreachesConstraintsEnum")
        .value("GET_INTO_BOUNDARIES_FAST", RMLPositionFlags::GET_INTO_BOUNDARIES_FAST)
        .value("GET_INTO_BOUNDARIES_AT_ZERO_ACCELERATION", RMLPositionFlags::GET_INTO_BOUNDARIES_AT_ZERO_ACCELERATION)
        .export_values()
    ;
#endif

    // RMLVelocityFlags
    py::class_<RMLVelocityFlags, RMLFlags>(m, "RMLVelocityFlags")
        .def(py::init<>())
        .def("__eq__", &RMLVelocityFlags::operator==)
        .def("__ne__", &RMLVelocityFlags::operator!=)
    ;

    // RMLInputParameters
    auto rml_input_parameters = py::class_<RMLInputParameters>(m, "RMLInputParameters")
        .def_readonly("NumberOfDOFs", &RMLInputParameters::NumberOfDOFs)
        .def_readwrite("MinimumSynchronizationTime", &RMLInputParameters::MinimumSynchronizationTime)
        .def_property("SelectionVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.SelectionVector; },
            [](RMLInputParameters& self, const RMLBoolVector& v) { *self.SelectionVector = v; })
        .def_property("CurrentPositionVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.CurrentPositionVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.CurrentPositionVector = v; })
        .def_property("CurrentVelocityVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.CurrentVelocityVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.CurrentVelocityVector = v; })
        .def_property("CurrentAccelerationVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.CurrentAccelerationVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.CurrentAccelerationVector = v; })
        .def_property("MaxAccelerationVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.MaxAccelerationVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.MaxAccelerationVector = v; })
        .def_property("MaxJerkVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.MaxJerkVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.MaxJerkVector = v; })
        .def_property("TargetVelocityVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.TargetVelocityVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.TargetVelocityVector = v; })
#if defined(RML_TYPE_IV)
        .def_readwrite("OverrideValue", &RMLInputParameters::OverrideValue)
        .def_property("MaxPositionVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.MaxPositionVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.MaxPositionVector = v; })
        .def_property("MinPositionVector",
            [](const RMLInputParameters& self) -> const auto& { return *self.MinPositionVector; },
            [](RMLInputParameters& self, const RMLDoubleVector& v) { *self.MinPositionVector = v; })
#endif
    ;
#if defined(RML_TYPE_IV)
    py::enum_<RMLInputParameters::ErrorCodeForInvalidInputValues>(rml_input_parameters, "ErrorCodeForInvalidInputValues")
        .value("IP_NO_ERROR", RMLInputParameters::IP_NO_ERROR)
        .value("IP_MAX_VELOCITY", RMLInputParameters::IP_MAX_VELOCITY)
        .value("IP_MAX_ACCELERATION", RMLInputParameters::IP_MAX_ACCELERATION)
        .value("IP_MAX_JERK", RMLInputParameters::IP_MAX_JERK)
        .value("IP_TARGET_VELOCITY", RMLInputParameters::IP_TARGET_VELOCITY)
        .value("IP_ORDER_OF_MAGNITUDE", RMLInputParameters::IP_ORDER_OF_MAGNITUDE)
        .value("IP_MINIMUM_SYNC_TIME", RMLInputParameters::IP_MINIMUM_SYNC_TIME)
        .value("IP_OVERRIDE_VALUE", RMLInputParameters::IP_OVERRIDE_VALUE)
        .export_values()
    ;
#endif

    // RMLPositionInputParameters
    py::class_<RMLPositionInputParameters, RMLInputParameters>(m, "RMLPositionInputParameters")
        .def(py::init<unsigned>())
#if defined(RML_TYPE_II)
        .def("CheckForValidity", &RMLPositionInputParameters::CheckForValidity)
#elif defined(RML_TYPE_IV)
        .def("CheckForValidity", [](const RMLPositionInputParameters& self) { return self.CheckForValidity(); })
#endif
        .def_property("MaxVelocityVector",
            [](const RMLPositionInputParameters& self) -> const auto& { return *self.MaxVelocityVector; },
            [](RMLPositionInputParameters& self, const RMLDoubleVector& v) { *self.MaxVelocityVector = v; })
        .def_property("TargetPositionVector",
            [](const RMLPositionInputParameters& self) -> const auto& { return *self.TargetPositionVector; },
            [](RMLPositionInputParameters& self, const RMLDoubleVector& v) { *self.TargetPositionVector = v; })
        .def_property("AlternativeTargetVelocityVector",
            [](const RMLPositionInputParameters& self) -> const auto& { return *self.AlternativeTargetVelocityVector; },
            [](RMLPositionInputParameters& self, const RMLDoubleVector& v) { *self.AlternativeTargetVelocityVector = v; })
    ;

    // RMLVelocityInputParameters
    py::class_<RMLVelocityInputParameters, RMLInputParameters>(m, "RMLVelocityInputParameters")
        .def(py::init<unsigned>())
#if defined(RML_TYPE_II)
        .def("CheckForValidity", &RMLVelocityInputParameters::CheckForValidity)
#elif defined(RML_TYPE_IV)
        .def("CheckForValidity", [](const RMLVelocityInputParameters& self) { return self.CheckForValidity(); })
#endif
    ;

    // RMLOutputParameters
    auto rml_output_parameters = py::class_<RMLOutputParameters>(m, "RMLOutputParameters")
        .def_readonly("ANewCalculationWasPerformed", &RMLOutputParameters::ANewCalculationWasPerformed)
        .def_readonly("TrajectoryIsPhaseSynchronized", &RMLOutputParameters::TrajectoryIsPhaseSynchronized)
        .def_readonly("NumberOfDOFs", &RMLOutputParameters::NumberOfDOFs)
        .def_readonly("DOFWithTheGreatestExecutionTime", &RMLOutputParameters::DOFWithTheGreatestExecutionTime)
        .def_readonly("SynchronizationTime", &RMLOutputParameters::SynchronizationTime)
        .def_property_readonly("NewPositionVector", [](const RMLOutputParameters& self) -> const auto& { return *self.NewPositionVector; })
        .def_property_readonly("NewVelocityVector", [](const RMLOutputParameters& self) -> const auto& { return *self.NewVelocityVector; })
        .def_property_readonly("NewAccelerationVector", [](const RMLOutputParameters& self) -> const auto& { return *self.NewAccelerationVector; })
        .def_property_readonly("MinExtremaTimesVector", [](const RMLOutputParameters& self) -> const auto& { return *self.MinExtremaTimesVector; })
        .def_property_readonly("MaxExtremaTimesVector", [](const RMLOutputParameters& self) -> const auto& { return *self.MaxExtremaTimesVector; })
        .def_property_readonly("MinPosExtremaPositionVectorOnly", [](const RMLOutputParameters& self) -> const auto& { return *self.MinPosExtremaPositionVectorOnly; })
        .def_property_readonly("MaxPosExtremaPositionVectorOnly", [](const RMLOutputParameters& self) -> const auto& { return *self.MaxPosExtremaPositionVectorOnly; })
        .def_property_readonly("ExecutionTimes", [](const RMLOutputParameters& self) -> const auto& { return *self.ExecutionTimes; })
        .def_property_readonly("MinPosExtremaPositionVectorArray",
            [](const RMLOutputParameters& self) { return make_list(self.MinPosExtremaPositionVectorArray, std::next(self.MinPosExtremaPositionVectorArray, self.NumberOfDOFs)); })
        .def_property_readonly("MinPosExtremaVelocityVectorArray",
            [](const RMLOutputParameters& self) { return make_list(self.MinPosExtremaVelocityVectorArray, std::next(self.MinPosExtremaVelocityVectorArray, self.NumberOfDOFs)); })
        .def_property_readonly("MinPosExtremaAccelerationVectorArray",
            [](const RMLOutputParameters& self) { return make_list(self.MinPosExtremaAccelerationVectorArray, std::next(self.MinPosExtremaAccelerationVectorArray, self.NumberOfDOFs)); })
        .def_property_readonly("MaxPosExtremaPositionVectorArray",
            [](const RMLOutputParameters& self) { return make_list(self.MaxPosExtremaPositionVectorArray, std::next(self.MaxPosExtremaPositionVectorArray, self.NumberOfDOFs)); })
        .def_property_readonly("MaxPosExtremaVelocityVectorArray",
            [](const RMLOutputParameters& self) { return make_list(self.MaxPosExtremaVelocityVectorArray, std::next(self.MaxPosExtremaVelocityVectorArray, self.NumberOfDOFs)); })
        .def_property_readonly("MaxPosExtremaAccelerationVectorArray",
            [](const RMLOutputParameters& self) { return make_list(self.MaxPosExtremaAccelerationVectorArray, std::next(self.MaxPosExtremaAccelerationVectorArray, self.NumberOfDOFs)); })
#if defined(RML_TYPE_IV)
        .def("GetErrorString", [](const RMLOutputParameters& self) { return std::string(self.GetErrorString()); })
        .def_readonly("OverrideFilterIsActive", &RMLOutputParameters::OverrideFilterIsActive)
        .def_readonly("ResultValue", &RMLOutputParameters::ResultValue)
        .def_readonly("CurrentOverrideValue", &RMLOutputParameters::CurrentOverrideValue)
        .def_property_readonly("Polynomials", [](const RMLOutputParameters& self) -> const auto& { return *self.Polynomials; })
#endif
    ;
    py::enum_<RMLOutputParameters::ReturnValue>(rml_output_parameters, "ReturnValue")
        .value("RETURN_SUCCESS", RMLOutputParameters::RETURN_SUCCESS)
        .value("RETURN_ERROR", RMLOutputParameters::RETURN_ERROR)
        .export_values()
    ;

    // RMLPositionOutputParameters
    py::class_<RMLPositionOutputParameters, RMLOutputParameters>(m, "RMLPositionOutputParameters")
        .def(py::init<unsigned>())
#if defined(RML_TYPE_IV)
        .def_readonly("TrajectoryExceedsTargetPosition", &RMLPositionOutputParameters::TrajectoryExceedsTargetPosition)
#endif
    ;

    // RMLVelocityOutputParameters
    py::class_<RMLVelocityOutputParameters, RMLOutputParameters>(m, "RMLVelocityOutputParameters")
        .def(py::init<unsigned>())
        .def_property_readonly("PositionValuesAtTargetVelocity", [](const RMLVelocityOutputParameters& self) -> const auto& { return *self.PositionValuesAtTargetVelocity; })
    ;

#if defined(RML_TYPE_IV)
    // RMLPolynomial
    py::class_<RMLPolynomial>(m, "RMLPolynomial")
        .def(py::init<>())
        .def_property_readonly("PositionPolynomialCoefficients", // TODO should not be readonly?
            [](const RMLPolynomial& self) { return make_list(std::begin(self.PositionPolynomialCoefficients), std::end(self.PositionPolynomialCoefficients)); })
        .def_property_readonly("VelocityPolynomialCoefficients",
            [](const RMLPolynomial& self) { return make_list(std::begin(self.VelocityPolynomialCoefficients), std::end(self.VelocityPolynomialCoefficients)); })
        .def_property_readonly("AccelerationPolynomialCoefficients",
            [](const RMLPolynomial& self) { return make_list(std::begin(self.AccelerationPolynomialCoefficients), std::end(self.AccelerationPolynomialCoefficients)); })
        .def_readonly("Time_ValidUntil", &RMLPolynomial::Time_ValidUntil)
    ;

    // RMLOutputPolynomials
    py::class_<RMLOutputPolynomials>(m, "RMLOutputPolynomials")
        .def(py::init<unsigned>())
        .def_readonly("NumberOfDOFs", &RMLOutputPolynomials::NumberOfDOFs)
        .def_property_readonly("NumberOfCurrentlyValidSegments",
            [](const RMLOutputPolynomials& self) { return make_list(self.NumberOfCurrentlyValidSegments, std::next(self.NumberOfCurrentlyValidSegments, self.NumberOfDOFs)); })
        .def_property_readonly("Coefficients",
            [](const RMLOutputPolynomials& self) { return make_list(self.Coefficients, std::next(self.Coefficients, self.NumberOfDOFs)); })
    ;
#endif

    py::class_<PositionTrajectoryGenerator>(m, "PositionTrajectoryGenerator")
        .def(py::init<unsigned, double>(), "number_of_dofs"_a, "cycle_time"_a)
        .def(py::init<unsigned, double, const RMLDoubleVector&, const RMLDoubleVector&>(),
             "number_of_dofs"_a, "cycle_time"_a, "max_velocity"_a, "max_acceleration"_a)
        .def(py::init<unsigned, double, const RMLDoubleVector&, const RMLDoubleVector&, const RMLDoubleVector&>(),
             "number_of_dofs"_a, "cycle_time"_a, "max_velocity"_a, "max_acceleration"_a, "max_jerk"_a)
        .def_readonly("number_of_dofs", &PositionTrajectoryGenerator::number_of_dofs)
        .def_readonly("cycle_time", &PositionTrajectoryGenerator::cycle_time)
        .def("trajectory", py::overload_cast<const RMLDoubleVector&, double>(&PositionTrajectoryGenerator::trajectory),
             "target_position"_a, "min_sync_time"_a = 0.0)
        .def("trajectory", py::overload_cast<const RMLDoubleVector&, const RMLDoubleVector&, double>(&PositionTrajectoryGenerator::trajectory),
             "target_position"_a, "target_velocity"_a, "min_sync_time"_a = 0.0)
        .def_property("current_position",
            [](const PositionTrajectoryGenerator& self) -> const auto& { return *self.ip.CurrentPositionVector; },
            [](PositionTrajectoryGenerator& self, const RMLDoubleVector& v) { *self.ip.CurrentPositionVector = v; })
        .def_property("current_velocity",
            [](const PositionTrajectoryGenerator& self) -> const auto& { return *self.ip.CurrentVelocityVector; },
            [](PositionTrajectoryGenerator& self, const RMLDoubleVector& v) { *self.ip.CurrentVelocityVector = v; })
        .def_property("current_acceleration",
            [](const PositionTrajectoryGenerator& self) -> const auto& { return *self.ip.CurrentAccelerationVector; },
            [](PositionTrajectoryGenerator& self, const RMLDoubleVector& v) { *self.ip.CurrentAccelerationVector = v; })
    ;

    py::class_<PositionTrajectoryIterator>(m, "PositionTrajectoryIterator")
        .def("__next__", &PositionTrajectoryIterator::next)
        .def("__iter__", [](PositionTrajectoryIterator& self) -> auto& { return self; })
    ;
}