#include <redstrain/protostr/ProtocolReader.hpp>
#include <redstrain/protostr/ProtocolWriter.hpp>

#include "Theme.hpp"
#include "../IllegalColorIndexError.hpp"

using std::string;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::protostr::ProtocolReader;
using redengine::protostr::ProtocolWriter;

namespace redengine {
namespace damnation {
namespace tk {

	Theme::Theme() {}

	Theme::Theme(const Theme& theme) : ColorSource(theme), ReferenceCounted(theme), colors(theme.colors) {}

	Theme::~Theme() {}

	void Theme::getColors(ColorIterator& begin, ColorIterator& end) const {
		begin = colors.begin();
		end = colors.end();
	}

	void Theme::putColor(const string& key, unsigned color) {
		switch(color) {
			case Color::NO_COLOR:
				colors.erase(key);
				break;
			default:
				if(color > 255u)
					throw IllegalColorIndexError(color);
			case Color::TERMINAL_DEFAULT:
				colors[key] = color;
				break;
		}
	}

	bool Theme::removeColor(const string& key) {
		Colors::iterator it(colors.find(key));
		if(it == colors.end())
			return false;
		colors.erase(it);
		return true;
	}

	unsigned Theme::getColor(const string& key) {
		ColorIterator it(colors.find(key));
		return it == colors.end() ? Color::NO_COLOR : it->second;
	}

	void Theme::save(OutputStream<char>& stream) const {
		ProtocolWriter proto(stream);
		proto.writeUInt32(static_cast<uint32_t>(colors.size()));
		ColorIterator begin(colors.begin()), end(colors.end());
		for(; begin != end; ++begin) {
			proto.writeString16(begin->first);
			proto.writeUInt16(static_cast<uint16_t>(begin->second));
		}
		proto.flush();
	}

	void Theme::load(InputStream<char>& stream) {
		ProtocolReader proto(stream);
		uint32_t count = proto.readUInt32(), u;
		for(u = static_cast<uint32_t>(0u); u < count; ++u) {
			string key;
			proto.readString16(key);
			unsigned color = static_cast<unsigned>(proto.readUInt16());
			putColor(key, color);
		}
	}

}}}
