#ifndef REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEFUNCTIONERROR_HPP
#define REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEFUNCTIONERROR_HPP

#include "IllegalCompressionParameterError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API IllegalDeflateFunctionError : public IllegalCompressionParameterError {

	  private:
		const int function;

	  public:
		IllegalDeflateFunctionError(int);
		IllegalDeflateFunctionError(const IllegalDeflateFunctionError&);

		inline int getFunctionID() const {
			return function;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalDeflateFunctionError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATEFUNCTIONERROR_HPP */
