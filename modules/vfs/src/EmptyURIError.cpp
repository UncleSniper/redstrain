#include "EmptyURIError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	EmptyURIError::EmptyURIError() : InvalidURIError(string()) {}

	EmptyURIError::EmptyURIError(const EmptyURIError& error) : Error(error), InvalidURIError(error) {}

	REDSTRAIN_DEFINE_ERROR(EmptyURIError) {
		out << "Empty URI is invalid";
	}

}}
