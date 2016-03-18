#ifndef REDSTRAIN_MOD_UTIL_PREDEFUNITS_HPP
#define REDSTRAIN_MOD_UTIL_PREDEFUNITS_HPP

#include "quantity.hpp"

namespace redengine {
namespace util {
namespace units {

	// base dimensions

	typedef Dimension<0, 0, 0, 0, 0, 0> Dimensionless;
	typedef Dimension<1, 0, 0, 0, 0, 0> Time;
	typedef Dimension<0, 1, 0, 0, 0, 0> Length;
	typedef Dimension<0, 0, 1, 0, 0, 0> Mass;
	typedef Dimension<0, 0, 0, 1, 0, 0> ElectricCurrent;
	typedef Dimension<0, 0, 0, 0, 1, 0> Temperature;
	typedef Dimension<0, 0, 0, 0, 0, 1> LuminousIntensity;

	// derived dimensions

	typedef Dimension<-1,  1,  0,  0,  0,  0> Speed;

	// dimensionless units

	typedef Unit<Dimensionless, BaseUnit> piece;

	// time units

	REDSTRAIN_UTIL_DEFINE_UNIT(Time, picosecond, 1, -12, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, nanosecond, 1, -9, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, microsecond, 1, -6, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, millisecond, 1, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(Time, second)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, minute, 6, 1, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, hour, 36, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, day, 864, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, week, 6048, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, month, 2629746, 0, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Time, year, 31556952, 0, 0, 0)

	REDSTRAIN_UTIL_DEFINE_UNIT(Time, fortnight, 12096, 2, 0, 0)

	// length units

	REDSTRAIN_UTIL_DEFINE_UNIT(Length, picometer, 1, -12, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, nanometer, 1, -9, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, micrometer, 1, -6, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, millimeter, 1, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, centimeter, 1, -2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(Length, meter)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, kilometer, 1, 3, 0, 0)

	REDSTRAIN_UTIL_DEFINE_UNIT(Length, angstrom, 1, -10, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, astronomicalUnit, 1495978707, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, bohr, 52917721, -18, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, fathom, 18288, -4, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, fermi, 1, -15, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, metricFoot, 31622776601683794, -17, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, longMetricFoot, 333333333333333333, -18, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, shortMetricFoot, 3, -1, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, intlFoot, 3048, -4, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, usFoot, 3048006096012192, -16, 0, 0)
	typedef intlFoot foot;
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, inch, 254, -4, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, lightday, 259020683712, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, lighthour, 10792528488, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, lightminute, 1798754748, 1, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, lightsecond, 299792458, 0, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, lightyear, 94607304725808, 2, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, intlMile, 1609344, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, usMile, 1609347219, -6, 0, 0)
	typedef intlMile mile;
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, nauticalLeague, 5556, 0, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, nauticalMile, 1852, 0, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, pace, 762, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, parsec, 3085677581, 7, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Length, yard, 9144, -4, 0, 0)

	// mass units

	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, picogram, 1, -15, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, nanogram, 1, -12, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, microgram, 1, -9, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, milligram, 1, -6, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, gram, 1, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(Mass, kilogram)

	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, carat, 205196548333, -15, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, metricCarat, 2, -4, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, ounce, 28, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, poundMass, 45359237, -8, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, metricPound, 5, -1, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Mass, tonne, 1, 3, 0, 0)

	// electric current units

	REDSTRAIN_UTIL_DEFINE_UNIT(ElectricCurrent, picoampere, 1, -12, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(ElectricCurrent, nanoampere, 1, -9, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(ElectricCurrent, microampere, 1, -6, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(ElectricCurrent, milliampere, 1, -3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(ElectricCurrent, ampere)
	REDSTRAIN_UTIL_DEFINE_UNIT(ElectricCurrent, kiloampere, 1, 3, 0, 0)

	// temerature units

	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(Temperature, kelvin)
	REDSTRAIN_UTIL_DEFINE_UNIT(Temperature, celsius, 1, 0, 27315, -2)
	REDSTRAIN_UTIL_DEFINE_UNIT(Temperature, fahrenheit, 555555555555555555, -18, 255372222222222222, -15)

	// luminous intensity units

	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(LuminousIntensity, candela)

	// speed units

	REDSTRAIN_UTIL_DEFINE_BASE_UNIT(Speed, meterPerSecond)
	REDSTRAIN_UTIL_DEFINE_UNIT(Speed, kilometerPerSecond, 1, 3, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Speed, meterPerHour, 277777777777777, -18, 0, 0)
	REDSTRAIN_UTIL_DEFINE_UNIT(Speed, kilometerPerHour, 277777777777777777, -18, 0, 0)

}}}

#endif /* REDSTRAIN_MOD_UTIL_PREDEFUNITS_HPP */
