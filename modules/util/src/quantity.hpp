#ifndef REDSTRAIN_MOD_UTIL_QUANTITY_HPP
#define REDSTRAIN_MOD_UTIL_QUANTITY_HPP

#include <stdint.h>

#include "static_assert.hpp"

namespace redengine {
namespace util {

	namespace units {

		template<
			int TimeExponent,
			int LengthExponent,
			int MassExponent,
			int CurrentExponent,
			int TemperatureExponent,
			int LuminousIntensityExponent
		>
		class Dimension {

		  public:
			static const int TIME = TimeExponent;
			static const int LENGTH = LengthExponent;
			static const int MASS = MassExponent;
			static const int CURRENT = CurrentExponent;
			static const int TEMPERATURE = TemperatureExponent;
			static const int LUMINOUS_INTENSITY = LuminousIntensityExponent;

		};

		template<int Base, int Exponent>
		class IntExp {

		  public:
			static const int64_t POWER = static_cast<int64_t>(Base) * IntExp<Base, Exponent - 1>::POWER;

		};

		template<int Base>
		class IntExp<Base, 0> {

		  public:
			static const int64_t POWER = 1;

		};

		template<int Base, int Exponent>
		class IntExpAbs {

		  private:
			template<int RealExponent, bool Negative>
			struct Impl {

				static const int64_t POWER = IntExp<Base, RealExponent>::POWER;

			};

			template<int RealExponent>
			struct Impl<RealExponent, true> {

				static const int64_t POWER = IntExp<Base, -RealExponent>::POWER;

			};

		  public:
			static const int64_t POWER = Impl<Exponent, Exponent < 0>::POWER;

		};

		template<
			int64_t ScaleMantissa,
			int ScaleExponent,
			int64_t BiasMantissa,
			int BiasExponent
		>
		class Conversion {

		  public:
			static const int64_t SCALE_MANTISSA = ScaleMantissa;
			static const int SCALE_EXPONENT = ScaleExponent;
			static const int64_t BIAS_MANTISSA = BiasMantissa;
			static const int BIAS_EXPONENT = BiasExponent;

			static const bool IS_BIASED = !!BiasMantissa;

		  private:
			static const int64_t SCALE_MAGNITUDE = IntExpAbs<10, ScaleExponent>::POWER;
			static const int64_t BIAS_MAGNITUDE = IntExpAbs<10, BiasExponent>::POWER;

		  public:
			template<typename BaseT>
			static inline BaseT toUniversal(BaseT x) {
				const double scale = ScaleExponent < 0
						? static_cast<double>(ScaleMantissa) / static_cast<double>(SCALE_MAGNITUDE)
						: static_cast<double>(ScaleMantissa) * static_cast<double>(SCALE_MAGNITUDE);
				const double bias = BiasExponent < 0
						? static_cast<double>(BiasMantissa) / static_cast<double>(BIAS_MAGNITUDE)
						: static_cast<double>(BiasMantissa) * static_cast<double>(BIAS_MAGNITUDE);
				return x * scale + bias;
			}

			template<typename BaseT>
			static inline BaseT fromUniversal(BaseT x) {
				const double scale = ScaleExponent < 0
						? static_cast<double>(ScaleMantissa) / static_cast<double>(SCALE_MAGNITUDE)
						: static_cast<double>(ScaleMantissa) * static_cast<double>(SCALE_MAGNITUDE);
				const double bias = BiasExponent < 0
						? static_cast<double>(BiasMantissa) / static_cast<double>(BIAS_MAGNITUDE)
						: static_cast<double>(BiasMantissa) * static_cast<double>(BIAS_MAGNITUDE);
				return (x - bias) / scale;
			}

		};

		typedef Conversion<static_cast<int64_t>(1), 0, static_cast<int64_t>(0), 0> BaseUnit;

		template<typename AbsoluteT, typename RelativeT>
		class ComposeConversions {

		  public:
			typedef AbsoluteT AbsoluteConversion;
			typedef RelativeT RelativeConversion;

		  public:
			static const bool IS_BIASED = AbsoluteConversion::IS_BIASED || RelativeConversion::IS_BIASED;

		  public:
			template<typename BaseT>
			static inline BaseT toUniversal(BaseT x) {
				return AbsoluteT::template toUniversal(RelativeT::template toUniversal(x));
			}

			template<typename BaseT>
			static inline BaseT fromUniversal(BaseT x) {
				return RelativeT::template fromUniversal(AbsoluteT::template fromUniversal(x));
			}

		};

		template<typename DimensionT, typename ConversionT>
		class Unit {

		  public:
			typedef DimensionT Dimension;
			typedef ConversionT Conversion;

		};

		template<typename LeftUnitT, typename RightUnitT>
		class ProductUnit {

		  public:
			typedef LeftUnitT LeftUnit;
			typedef RightUnitT RightUnit;

		  private:
			typedef typename LeftUnitT::Dimension LeftDimension;
			typedef typename LeftUnitT::Conversion LeftConversion;
			typedef typename RightUnitT::Dimension RightDimension;
			typedef typename RightUnitT::Conversion RightConversion;

		  public:
			class ProductConversion {

			  private:
				REDSTRAIN_STATIC_ASSERT(RequireUnbiased,
						!LeftConversion::IS_BIASED && !RightConversion::IS_BIASED);

			  public:
				static const bool IS_BIASED = false;

			  public:
				template<typename BaseT>
				static inline BaseT toUniversal(BaseT x) {
					return LeftConversion::template toUniversal(RightConversion::template toUniversal(x));
				}

				template<typename BaseT>
				static inline BaseT fromUniversal(BaseT x) {
					return LeftConversion::template fromUniversal(RightConversion::template fromUniversal(x));
				}

			};

		  public:
			typedef Unit<Dimension<
				LeftDimension::TIME + RightDimension::TIME,
				LeftDimension::LENGTH + RightDimension::LENGTH,
				LeftDimension::MASS + RightDimension::MASS,
				LeftDimension::CURRENT + RightDimension::CURRENT,
				LeftDimension::TEMPERATURE + RightDimension::TEMPERATURE,
				LeftDimension::LUMINOUS_INTENSITY + RightDimension::LUMINOUS_INTENSITY
			>, ProductConversion> Product;

		};

		template<typename NumeratorUnitT, typename DenominatorUnitT>
		class RatioUnit {

		  public:
			typedef NumeratorUnitT NumeratorUnit;
			typedef DenominatorUnitT DenominatorUnit;

		  private:
			typedef typename NumeratorUnitT::Dimension NumeratorDimension;
			typedef typename NumeratorUnitT::Conversion NumeratorConversion;
			typedef typename DenominatorUnitT::Dimension DenominatorDimension;
			typedef typename DenominatorUnitT::Conversion DenominatorConversion;

		  public:
			class RatioConversion {

			  private:
				REDSTRAIN_STATIC_ASSERT(RequireUnbiased,
						!NumeratorConversion::IS_BIASED && !DenominatorConversion::IS_BIASED);

			  public:
				static const bool IS_BIASED = false;

				// RatioUnit<km, h>
				// => NumeratorConversion::toUniversal(x) = x * 1000
				//    NumeratorConversion::fromUniversal(x) = x / 1000
				//    DenominatorConversion::toUniversal(x) = x * 3600
				//    DenominatorConversion::fromUniversal(x) = x / 3600
				// => RatioConversion::toUniversal(x) = x / 3.6 = x * 1000 / 3600
				//    RatioConversion::fromUniversal(x) = x * 3.6 = x / 1000 * 3600

			  public:
				template<typename BaseT>
				static inline BaseT toUniversal(BaseT x) {
					return NumeratorConversion::template toUniversal(
							DenominatorConversion::template fromUniversal(x));
				}

				template<typename BaseT>
				static inline BaseT fromUniversal(BaseT x) {
					return NumeratorConversion::template fromUniversal(
							DenominatorConversion::template toUniversal(x));
				}

			};

		  public:
			typedef Unit<Dimension<
				NumeratorDimension::TIME - DenominatorDimension::TIME,
				NumeratorDimension::LENGTH - DenominatorDimension::LENGTH,
				NumeratorDimension::MASS - DenominatorDimension::MASS,
				NumeratorDimension::CURRENT - DenominatorDimension::CURRENT,
				NumeratorDimension::TEMPERATURE - DenominatorDimension::TEMPERATURE,
				NumeratorDimension::LUMINOUS_INTENSITY - DenominatorDimension::LUMINOUS_INTENSITY
			>, RatioConversion> Ratio;

		};

	}

	template<typename UnitT, typename BaseT = double>
	class quantity {

	  public:
		typedef UnitT Unit;
		typedef BaseT Base;
		typedef typename UnitT::Dimension Dimension;
		typedef typename UnitT::Conversion Conversion;

	  public:
		BaseT magnitude;

	  public:
		quantity(BaseT magnitude) : magnitude(magnitude) {}
		quantity(const quantity& q) : magnitude(q.magnitude) {}

		template<typename SourceConversionT>
		quantity(const quantity<units::Unit<Dimension, SourceConversionT>, BaseT>& q)
				: magnitude(Conversion::template fromUniversal<BaseT>(
				SourceConversionT::template toUniversal<BaseT>(q.magnitude))) {}

		inline operator BaseT() const {
			return magnitude;
		}

		quantity<units::Unit<Dimension, units::BaseUnit>, BaseT> baseQuantity() const {
			return Conversion::template toUniversal<BaseT>(magnitude);
		}

		BaseT baseMagnitude() const {
			return Conversion::template toUniversal<BaseT>(magnitude);
		}

		inline quantity operator+(quantity other) const {
			return magnitude + other.magnitude;
		}

		inline quantity operator-(quantity other) const {
			return magnitude - other.magnitude;
		}

		template<typename OtherConversionT>
		inline quantity operator+(quantity<units::Unit<Dimension, OtherConversionT>, BaseT> other) const {
			return magnitude + Conversion::template fromUniversal<BaseT>(
					OtherConversionT::template toUniversal<BaseT>(other.magnitude));
		}

		template<typename OtherConversionT>
		inline quantity operator-(quantity<units::Unit<Dimension, OtherConversionT>, BaseT> other) const {
			return magnitude - Conversion::template fromUniversal(
					OtherConversionT::template toUniversal<BaseT>(other.magnitude));
		}

		template<typename OtherDimensionT, typename OtherConversionT, typename OtherBaseT>
		inline quantity<units::Unit<units::Dimension<
			Dimension::TIME + OtherDimensionT::TIME,
			Dimension::LENGTH + OtherDimensionT::LENGTH,
			Dimension::MASS + OtherDimensionT::MASS,
			Dimension::CURRENT + OtherDimensionT::CURRENT,
			Dimension::TEMPERATURE + OtherDimensionT::TEMPERATURE,
			Dimension::LUMINOUS_INTENSITY + OtherDimensionT::LUMINOUS_INTENSITY
		>, units::BaseUnit>, OtherBaseT> operator*(quantity<units::Unit<OtherDimensionT, OtherConversionT>,
				OtherBaseT> other) const {
			return static_cast<OtherBaseT>(Conversion::template toUniversal<BaseT>(magnitude)
					* OtherConversionT::template toUniversal<OtherBaseT>(other.magnitude));
		}

		template<typename OtherDimensionT, typename OtherConversionT, typename OtherBaseT>
		inline quantity<units::Unit<units::Dimension<
			Dimension::TIME - OtherDimensionT::TIME,
			Dimension::LENGTH - OtherDimensionT::LENGTH,
			Dimension::MASS - OtherDimensionT::MASS,
			Dimension::CURRENT - OtherDimensionT::CURRENT,
			Dimension::TEMPERATURE - OtherDimensionT::TEMPERATURE,
			Dimension::LUMINOUS_INTENSITY - OtherDimensionT::LUMINOUS_INTENSITY
		>, units::BaseUnit>, OtherBaseT> operator/(quantity<units::Unit<OtherDimensionT, OtherConversionT>,
				OtherBaseT> other) const {
			return static_cast<OtherBaseT>(Conversion::template toUniversal<BaseT>(magnitude)
					/ OtherConversionT::template toUniversal<OtherBaseT>(other.magnitude));
		}

	};

}}

#define REDSTRAIN_UTIL_DEFINE_UNIT(dimension, name, scaleMantissa, scaleExponent, biasMantissa, biasExponent) \
	typedef ::redengine::util::units::Unit<dimension, ::redengine::util::units::Conversion< \
		static_cast<int64_t>(scaleMantissa ## ll), \
		scaleExponent, \
		static_cast<int64_t>(biasMantissa ## ll), \
		biasExponent \
	> > name;

#define REDSTRAIN_UTIL_DEFINE_BASE_UNIT(dimension, name) \
	typedef ::redengine::util::units::Unit<dimension, ::redengine::util::units::BaseUnit> name;

#include "predefUnits.hpp"

#define USING_REDSTRAIN_UNIT(libName, localName) typedef ::redengine::util::units::libName localName;

#define REDSTRAIN_UTIL_DERIVE_UNIT(baseUnit, scaleMantissa, scaleExponent, biasMantissa, biasExponent) \
	typedef ::redengine::util::units::Unit<baseUnit::Dimension, ::redengine::util::units::ComposeConversions< \
		baseUnit::Conversion, \
		::redengine::util::units::Conversion< \
			static_cast<int64_t>(scaleMantissa ## ll), \
			scaleExponent, \
			static_cast<int64_t>(biasMantissa ## ll), \
			biasExponent \
		> \
	> > name;

#define REDSTRAIN_UTIL_PRODUCT_UNIT(name, leftUnit, rightUnit) \
	typedef ::redengine::util::units::ProductUnit<leftUnit, rightUnit>::Product name;
#define REDSTRAIN_UTIL_RATIO_UNIT(name, numeratorUnit, denominatorUnit) \
	typedef ::redengine::util::units::RatioUnit<numeratorUnit, denominatorUnit>::Ratio name;

#endif /* REDSTRAIN_MOD_UTIL_QUANTITY_HPP */
