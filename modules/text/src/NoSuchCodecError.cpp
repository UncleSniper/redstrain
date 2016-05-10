#include "NoSuchCodecError.hpp"

using std::string;

namespace redengine {
namespace text {

	NoSuchCodecError::NoSuchCodecError(const string& codec) : codec(codec) {}

	NoSuchCodecError::NoSuchCodecError(const NoSuchCodecError& error) : TextError(error), codec(error.codec) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchCodecError) {
		out << "No such text codec: " << codec;
	}

}}
