#ifndef REDSTRAIN_MOD_TEXT_NOSUCHENCODERERROR_HPP
#define REDSTRAIN_MOD_TEXT_NOSUCHENCODERERROR_HPP

#include "NoSuchCodecError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API NoSuchEncoderError : public NoSuchCodecError {

	  public:
		NoSuchEncoderError(const std::string&);
		NoSuchEncoderError(const NoSuchEncoderError&);

		REDSTRAIN_DECLARE_ERROR(NoSuchEncoderError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_NOSUCHENCODERERROR_HPP */
