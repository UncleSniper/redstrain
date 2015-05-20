#include <sstream>

#include "Error.hpp"

using std::string;
using std::stringstream;

namespace redengine {
namespace error {

	Error::Error() : refCount(1u) {}

	Error::Error(const Error&) : refCount(1u) {}

	Error::~Error() {}

	void Error::ref() {
		++refCount;
	}

	void Error::unref() {
		if(!--refCount)
			delete this;
	}

	string Error::getMessage() const {
		stringstream ss;
		printMessage(ss);
		return ss.str();
	}

	const char* Error::getTypename() {
		return "Error";
	}

}}
