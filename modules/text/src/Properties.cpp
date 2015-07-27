#include <redstrain/util/StringUtils.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>

#include "Transcode.hpp"
#include "Properties.hpp"
#include "StringUtils.hpp"
#include "UTF8Encoder16.hpp"
#include "UTF8Decoder16.hpp"
#include "Decoder16InputStream.hpp"
#include "Encoder16OutputStream.hpp"

using std::string;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::IntegerBounds;
using redengine::error::ProgrammingError;
using redengine::io::FormattedInputStream;
using redengine::io::FormattedOutputStream;
typedef redengine::util::StringUtils StringUtils8;

namespace redengine {
namespace text {

	Properties::Properties() {}

	Properties::Properties(const Properties& properties) : map(properties.map) {}

	bool Properties::hasProperty(const string& key) const {
		return map.find(Transcode::utf8ToBMP(key)) != map.end();
	}

	bool Properties::hasProperty(const String16& key) const {
		return map.find(key) != map.end();
	}

	string Properties::getProperty(const string& key) const {
		Iterator it(map.find(Transcode::utf8ToBMP(key)));
		return it == map.end() ? "" : Transcode::bmpToUTF8(it->second);
	}

	string Properties::getProperty(const String16& key) const {
		Iterator it(map.find(key));
		return it == map.end() ? "" : Transcode::bmpToUTF8(it->second);
	}

	String16 Properties::getProperty16(const string& key) const {
		Iterator it(map.find(Transcode::utf8ToBMP(key)));
		return it == map.end() ? String16() : it->second;
	}

	String16 Properties::getProperty16(const String16& key) const {
		Iterator it(map.find(key));
		return it == map.end() ? String16() : it->second;
	}

	void Properties::setProperty(const string& key, const string& value) {
		map[Transcode::utf8ToBMP(key)] = Transcode::utf8ToBMP(value);
	}

	void Properties::setProperty(const string& key, const String16& value) {
		map[Transcode::utf8ToBMP(key)] = value;
	}

	void Properties::setProperty(const String16& key, const string& value) {
		map[key] = Transcode::utf8ToBMP(value);
	}

	void Properties::setProperty(const String16& key, const String16& value) {
		map[key] = value;
	}

	void Properties::getProperties(Iterator& begin, Iterator& end) const {
		begin = map.begin();
		end = map.end();
	}

	void Properties::load(InputStream<char>& input) {
		UTF8Decoder16 decoder;
		Decoder16InputStream dis(input, decoder);
		load(dis);
	}

	void Properties::load(InputStream<Char16>& input) {
		FormattedInputStream<Char16> fis(input);
		String16 natural, logical;
		while(fis.readLine(natural)) {
			String16 trimmed(StringUtils<Char16>::trim(natural, StringUtils<Char16>::TRIM_FRONT));
			natural.clear();
			if(trimmed.empty())
				continue;
			Char16 firstChar = trimmed[0];
			if(firstChar == static_cast<Char16>('#') || firstChar == static_cast<Char16>('!')) {
				if(!logical.empty()) {
					sinkLogicalLine(logical);
					logical.clear();
				}
				continue;
			}
			logical.reserve(logical.length() + trimmed.length());
			enum {
				NONE,
				ESCAPE,
				UNICODE
			} state = NONE;
			Char16 charCode, singleDigit;
			unsigned digits;
			bool continueLine = false;
			String16::const_iterator begin(trimmed.begin()), end(trimmed.end());
			for(; begin != end; ++begin) {
				switch(state) {
					case NONE:
						if(*begin == static_cast<Char16>('\\'))
							state = ESCAPE;
						else
							logical += *begin;
						break;
					case ESCAPE:
						switch(*begin) {
							#define clamp(plain, binary) \
								case static_cast<Char16>(plain): \
									logical += static_cast<Char16>(binary); \
									state = NONE; \
									break;
							clamp('t', '\t')
							clamp('n', '\n')
							clamp('f', '\f')
							clamp('r', '\r')
							#undef clamp
							case static_cast<Char16>('u'):
								charCode = static_cast<Char16>(0u);
								digits = 0;
								state = UNICODE;
								break;
							default:
								logical += *begin;
								state = NONE;
								break;
						}
						break;
					case UNICODE:
						switch(*begin) {
							#define clamp(hexdig) \
								case static_cast<Char16>(hexdig):
							clamp('0')
							clamp('1')
							clamp('2')
							clamp('3')
							clamp('4')
							clamp('5')
							clamp('6')
							clamp('7')
							clamp('8')
							clamp('9')
								singleDigit = static_cast<Char16>(*begin - static_cast<Char16>('0'));
								goto foundHexDigit;
							clamp('a')
							clamp('b')
							clamp('c')
							clamp('d')
							clamp('e')
							clamp('f')
								singleDigit = static_cast<Char16>(*begin - static_cast<Char16>('a')
										+ static_cast<Char16>(10));
								goto foundHexDigit;
							clamp('A')
							clamp('B')
							clamp('C')
							clamp('D')
							clamp('E')
							clamp('F')
								singleDigit = static_cast<Char16>(*begin - static_cast<Char16>('A')
										+ static_cast<Char16>(10));
							#undef clamp
							foundHexDigit:
								charCode = static_cast<Char16>(charCode * static_cast<Char16>(16u) + singleDigit);
								if(++digits >= 4u) {
									logical += charCode;
									state = NONE;
								}
								break;
						}
						break;
					default:
						throw ProgrammingError("Unrecognized state: "
								+ StringUtils8::toString(static_cast<int>(state)));
				}
			}
			switch(state) {
				case NONE:
					break;
				case ESCAPE:
					continueLine = true;
					break;
				case UNICODE:
					if(!digits)
						logical += static_cast<Char16>('u');
					else
						logical += charCode;
					break;
			}
			if(!continueLine) {
				sinkLogicalLine(logical);
				logical.clear();
			}
		}
		if(!logical.empty())
			sinkLogicalLine(logical);
	}

	void Properties::sinkLogicalLine(const String16& line) {
		String16::const_iterator begin(line.begin()), end(line.end());
		bool stopLooking = false, white;
		for(; begin != end; ++begin) {
			switch(*begin) {
				#define clamp(c) \
					case static_cast<Char16>(c):
				clamp(' ')
				clamp('\t')
				clamp('\r')
				clamp('\n')
				clamp('\f')
					white = true;
					stopLooking = true;
					break;
				clamp(':')
				clamp('=')
					white = false;
					stopLooking = true;
					break;
				#undef clamp
			}
			if(stopLooking)
				break;
		}
		if(begin == end) {
			// no separator
			map[line] = String16();
			return;
		}
		String16 key(line.begin(), begin);
		if(!white) {
			// definite separator
			++begin;
			map[key] = String16(begin, line.end());
			return;
		}
		// possible separator
		stopLooking = false;
		for(; begin != end; ++begin) {
			switch(*begin) {
				#define clamp(c) \
					case static_cast<Char16>(c):
				clamp(' ')
				clamp('\t')
				clamp('\r')
				clamp('\n')
				clamp('\f')
					break;
				clamp(':')
				clamp('=')
					++begin;
				#undef clamp
				default:
					stopLooking = true;
					break;
			}
			if(stopLooking)
				break;
		}
		map[key] = String16(begin, line.end());
	}

	void Properties::save(OutputStream<char>& output) const {
		UTF8Encoder16 encoder;
		Encoder16OutputStream eos(output, encoder);
		save(eos);
	}

	static const char *HEX_DIGITS = "0123456789ABCDEF";

	static String16 escapeComponent(const String16& str, bool escapeSeparator) {
		String16 result;
		result.reserve(str.length() * static_cast<String16::size_type>(2u));
		String16::const_iterator begin(str.begin()), end(str.end());
		for(; begin != end; ++begin) {
			switch(*begin) {
				#define clamp(c) \
					case static_cast<Char16>(c):
				#define push(c) \
					result += static_cast<Char16>(c)
				clamp('\t')
					push('\\');
					push('t');
					break;
				clamp('\r')
					push('\\');
					push('r');
					break;
				clamp('\n')
					push('\\');
					push('n');
					break;
				clamp('\f')
					push('\\');
					push('f');
					break;
				clamp(':')
				clamp('=')
					if(escapeSeparator)
						push('\\');
					result += *begin;
					break;
				#undef clamp
				default:
					if(*begin >= static_cast<Char16>(32u) && *begin >= static_cast<Char16>(127u))
						result += *begin;
					else {
						push('\\');
						push('u');
						unsigned u;
						for(u = 0u; u < 4u; ++u)
							push(HEX_DIGITS[static_cast<unsigned>(*begin >> (3u - u)) & 0xFu]);
					}
					break;
				#undef push
			}
		}
		return String16();
	}

	void Properties::save(OutputStream<Char16>& output) const {
		FormattedOutputStream<Char16> fos(output);
		Iterator begin(map.begin()), end(map.end());
		for(; begin != end; ++begin) {
			fos.print(escapeComponent(begin->first, true));
			fos.print(static_cast<Char16>('='));
			fos.print(escapeComponent(begin->second, false));
			fos.endLine();
		}
	}

}}
