#ifndef REDSTRAIN_MOD_TEXT_NOSUCHDECODERERROR_HPP
#define REDSTRAIN_MOD_TEXT_NOSUCHDECODERERROR_HPP

#include "NoSuchCodecError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API NoSuchDecoderError : public NoSuchCodecError {

	  public:
		NoSuchDecoderError(const std::string&);
		NoSuchDecoderError(const NoSuchDecoderError&);

		REDSTRAIN_DECLARE_ERROR(NoSuchDecoderError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_NOSUCHDECODERERROR_HPP */
