#ifndef REDSTRAIN_MOD_TEXT_INVALIDFORMATTINGITEMREFERENCEERROR_HPP
#define REDSTRAIN_MOD_TEXT_INVALIDFORMATTINGITEMREFERENCEERROR_HPP

#include <redstrain/util/IntegerBits.hpp>
#include <redstrain/util/types.hpp>

#include "FormattingError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API InvalidFormattingItemReferenceError : public FormattingError {

	  public:
		typedef util::IntegerBits<util::MemorySize>::AsSigned Reference;

	  private:
		const Reference reference;
		const util::MemorySize offset;

	  public:
		InvalidFormattingItemReferenceError(Reference, util::MemorySize);
		InvalidFormattingItemReferenceError(const InvalidFormattingItemReferenceError&);

		inline Reference getReference() const {
			return reference;
		}

		inline util::MemorySize getOffset() const {
			return offset;
		}

		REDSTRAIN_DECLARE_ERROR(InvalidFormattingItemReferenceError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_INVALIDFORMATTINGITEMREFERENCEERROR_HPP */
