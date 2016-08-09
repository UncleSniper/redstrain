#ifndef REDSTRAIN_MOD_DAMNATION_UNDEFINEDSTRINGPARAMETERREFERENCEDERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_UNDEFINEDSTRINGPARAMETERREFERENCEDERROR_HPP

#include "TParmError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API UndefinedStringParameterReferencedError : public TParmError {

	  private:
		const unsigned index;

	  public:
		UndefinedStringParameterReferencedError(unsigned);
		UndefinedStringParameterReferencedError(const UndefinedStringParameterReferencedError&);

		REDSTRAIN_DECLARE_ERROR(UndefinedStringParameterReferencedError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_UNDEFINEDSTRINGPARAMETERREFERENCEDERROR_HPP */
