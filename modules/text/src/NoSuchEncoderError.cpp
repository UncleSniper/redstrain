#include "NoSuchEncoderError.hpp"

using std::string;

namespace redengine {
namespace text {

	NoSuchEncoderError::NoSuchEncoderError(const string& codec) : NoSuchCodecError(codec) {}

	NoSuchEncoderError::NoSuchEncoderError(const NoSuchEncoderError& error) : NoSuchCodecError(error) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchEncoderError) {
		out << "No such text encoder: " << codec;
	}

}}
