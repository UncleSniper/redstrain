#ifndef REDSTRAIN_MOD_TEXT_INVALIDFORMATTINGITEMREFERENCEERROR_HPP
#define REDSTRAIN_MOD_TEXT_INVALIDFORMATTINGITEMREFERENCEERROR_HPP

#include <redstrain/util/IntegerBits.hpp>

#include "FormattingError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API InvalidFormattingItemReferenceError : public FormattingError {

	  public:
		typedef util::IntegerBits<size_t>::AsSigned Reference;

	  private:
		const Reference reference;
		const size_t offset;

	  public:
		InvalidFormattingItemReferenceError(Reference, size_t);
		InvalidFormattingItemReferenceError(const InvalidFormattingItemReferenceError&);

		inline Reference getReference() const {
			return reference;
		}

		inline size_t getOffset() const {
			return offset;
		}

		REDSTRAIN_DECLARE_ERROR(InvalidFormattingItemReferenceError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_INVALIDFORMATTINGITEMREFERENCEERROR_HPP */
