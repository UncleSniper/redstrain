#include "NoSuchDecoderError.hpp"

using std::string;

namespace redengine {
namespace text {

	NoSuchDecoderError::NoSuchDecoderError(const string& codec) : NoSuchCodecError(codec) {}

	NoSuchDecoderError::NoSuchDecoderError(const NoSuchDecoderError& error) : NoSuchCodecError(error) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchDecoderError) {
		out << "No such text decoder: " << codec;
	}

}}
