#include <redstrain/text/Transcode.hpp>

#include "AbstractURI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	AbstractURI::AbstractURI(const string& scheme) : scheme(scheme) {}

	AbstractURI::AbstractURI(const AbstractURI& uri) : URI(uri), scheme(uri.scheme) {}

	AbstractURI::~AbstractURI() {}

	bool AbstractURI::hasScheme() const {
		return !scheme.empty();
	}

	string AbstractURI::getScheme8() const {
		return scheme;
	}

	String16 AbstractURI::getScheme16() const {
		return Transcode::utf8ToBMP(scheme);
	}

	String32 AbstractURI::getScheme32() const {
		return Transcode::utf8ToUnicode(scheme);
	}

}}
