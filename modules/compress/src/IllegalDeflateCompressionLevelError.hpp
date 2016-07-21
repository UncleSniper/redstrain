#ifndef REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATECOMPRESSIONLEVEL_HPP
#define REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATECOMPRESSIONLEVEL_HPP

#include "IllegalCompressionParameterError.hpp"

namespace redengine {
namespace compress {

	class REDSTRAIN_COMPRESS_API IllegalDeflateCompressionLevelError : public IllegalCompressionParameterError {

	  private:
		const unsigned level;

	  public:
		IllegalDeflateCompressionLevelError(unsigned);
		IllegalDeflateCompressionLevelError(const IllegalDeflateCompressionLevelError&);

		inline unsigned getCompressionLevel() const {
			return level;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalDeflateCompressionLevelError)

	};

}}

#endif /* REDSTRAIN_MOD_COMPRESS_ILLEGALDEFLATECOMPRESSIONLEVEL_HPP */
