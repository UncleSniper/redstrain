#ifndef REDSTRAIN_MOD_IO_NUMBERFORMATERROR_HPP
#define REDSTRAIN_MOD_IO_NUMBERFORMATERROR_HPP

#include <redstrain/util/ClassifyType.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API NumberFormatError : public error::IllegalArgumentError {

	  public:
		enum REDSTRAIN_IO_API NumberType {
			SIGNED_INTEGER,
			UNSIGNED_INTEGER
		};

		template<typename NumberT>
		class NumberTypeByPrimitiveType {

		  private:
			template<int Dummy, int TypeClass>
			struct NumberTypeImpl;

			template<int Dummy>
			struct NumberTypeImpl<Dummy, util::TCLS_INTEGER> {

				static const NumberType NUMBER_TYPE
						= util::IntegerBounds<NumberT>::SIGNED ? SIGNED_INTEGER : UNSIGNED_INTEGER;

			};

		  public:
			static const NumberType NUMBER_TYPE = NumberTypeImpl<
				0,
				util::ClassifyType<NumberT>::CLASS
			>::NUMBER_TYPE;

		};

	  private:
		const NumberType numberType;
		const std::string rendition;

	  public:
		NumberFormatError(NumberType, const std::string&);
		NumberFormatError(const NumberFormatError&);

		inline NumberType getRequiredNumberType() const {
			return numberType;
		}

		inline const std::string& getOffendingRendition() const {
			return rendition;
		}

		REDSTRAIN_DECLARE_ERROR(NumberFormatError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_NUMBERFORMATERROR_HPP */
