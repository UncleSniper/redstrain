#include <redstrain/platform/Stat.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/io/FormattedInputStream.hpp>

#include "CPPLanguage.hpp"

using std::string;
using redengine::util::Appender;
using redengine::platform::Stat;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::io::FileInputStream;
using redengine::platform::Filesystem;
using redengine::error::ProgrammingError;
using redengine::io::FormattedInputStream;

namespace redengine {
namespace build {

	CPPLanguage::CPPLanguage(Compiler& compiler) : CompiledLanguage("C++", compiler) {}

	CPPLanguage::CPPLanguage(const CPPLanguage& cpp) : CompiledLanguage(cpp) {}

	static const char *const SOURCE_SUFFIXES[] = {
		".cpp",
		".cxx",
		NULL
	};

	static const char *const HEADER_SUFFIXES[] = {
		".hpp",
		".hxx",
		NULL
	};

	Language::ArtifactType CPPLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, HEADER_SUFFIXES);
	}

	enum CPPIncludeState {
		CPPIS_EMPTY,
		CPPIS_NONEMPTY,
		CPPIS_HASH,
		CPPIS_INCLUDE,
		CPPIS_DIRECTIVE,
		CPPIS_QUOTE,
		CPPIS_ANGLE,
		CPPIS_SLASH,
		CPPIS_LINE_COMMENT,
		CPPIS_BLOCK_COMMENT,
		CPPIS_BLOCK_COMMENT_STAR,
		CPPIS_STRING_LITERAL,
		CPPIS_STRING_ESCAPE,
		CPPIS_CHAR_LITERAL,
		CPPIS_CHAR_ESCAPE,
		CPPIS_STRAY_BACKSLASH
	};

	void CPPLanguage::getReferencedHeaders(const string& path, Appender<ReferencedHeader>& sink) {
		if(!Filesystem::access(path, Filesystem::FILE_EXISTS)) {
			sink.doneAppending();
			return;
		}
		Stat stat;
		Filesystem::stat(path, stat);
		if(stat.getType() != Stat::REGULAR_FILE) {
			sink.doneAppending();
			return;
		}
		FileInputStream fis(path);
		StreamCloser closer(fis);
		FormattedInputStream<char> input(fis);
		string line, header;
		CPPIncludeState state = CPPIS_EMPTY, beforeComment, beforeBackslash, failBackslash;
		unsigned directivePrefix;
		while(input.readLine(line)) {
			line += '\n';
			string::const_iterator begin(line.begin()), end(line.end());
			for(; begin != end; ++begin) {
				char c = *begin;
				#define whitespace \
					case '\0': \
					case ' ': \
					case '\t': \
					case '\f'
				#define newline \
					case '\r': \
					case '\n'
				#define or_bail \
					newline: \
						state = CPPIS_EMPTY; \
						break; \
					default: \
						state = CPPIS_NONEMPTY; \
						break;
				#define or_literal \
					case '\'': \
						state = CPPIS_CHAR_LITERAL; \
						break; \
					case '"': \
						state = CPPIS_STRING_LITERAL; \
						break;
				#define or_comment \
					case '/': \
						beforeComment = state; \
						state = CPPIS_SLASH; \
						break;
				#define or_backslash \
					case '\\': \
						beforeBackslash = state; \
						failBackslash = CPPIS_NONEMPTY; \
						state = CPPIS_STRAY_BACKSLASH; \
						break;
				#define or_defaults \
					or_literal \
					or_comment \
					or_backslash \
					or_bail
				switch(state) {
					case CPPIS_EMPTY:
						switch(c) {
							whitespace:
								break;
							case '#':
								state = CPPIS_HASH;
								break;
							or_defaults
						}
						break;
					case CPPIS_NONEMPTY:
						switch(c) {
							or_defaults
						}
						break;
					case CPPIS_HASH:
						switch(c) {
							whitespace:
								break;
							case 'i':
								state = CPPIS_INCLUDE;
								directivePrefix = 1u;
								break;
							or_defaults
						}
						break;
					case CPPIS_INCLUDE:
						switch(c) {
							or_literal
							or_comment
							or_backslash
							newline:
								state = CPPIS_EMPTY;
								break;
							default:
								if(c == "include"[directivePrefix]) {
									if(++directivePrefix >= 7u)
										state = CPPIS_DIRECTIVE;
								}
								else
									state = CPPIS_NONEMPTY;
								break;
						}
						break;
					case CPPIS_DIRECTIVE:
						switch(c) {
							whitespace:
								break;
							case '<':
								header.clear();
								state = CPPIS_ANGLE;
								break;
							case '"':
								header.clear();
								state = CPPIS_QUOTE;
								break;
							case '\'':
								state = CPPIS_CHAR_LITERAL;
								break;
							or_comment
							or_backslash
							or_bail
						}
						break;
					case CPPIS_QUOTE:
						switch(c) {
							case '"':
								sink.append(ReferencedHeader(header, true));
								state = CPPIS_NONEMPTY;
								break;
							newline:
								state = CPPIS_EMPTY;
								break;
							default:
								header += c;
								break;
						}
						break;
					case CPPIS_ANGLE:
						switch(c) {
							case '>':
								sink.append(ReferencedHeader(header, false));
								state = CPPIS_NONEMPTY;
								break;
							newline:
								state = CPPIS_EMPTY;
								break;
							default:
								header += c;
								break;
						}
						break;
					case CPPIS_SLASH:
						switch(c) {
							case '/':
								state = CPPIS_LINE_COMMENT;
								break;
							case '*':
								state = CPPIS_BLOCK_COMMENT;
								break;
							or_literal
							or_backslash
							or_bail
						}
						break;
					case CPPIS_LINE_COMMENT:
						switch(c) {
							newline:
								state = beforeComment;
								break;
							or_backslash
							default:
								break;
						}
						break;
					case CPPIS_BLOCK_COMMENT:
						switch(c) {
							case '*':
								state = CPPIS_BLOCK_COMMENT_STAR;
								break;
							default:
								break;
						}
						break;
					case CPPIS_BLOCK_COMMENT_STAR:
						switch(c) {
							case '*':
								break;
							case '/':
								state = beforeComment;
								break;
							case '\\':
								beforeBackslash = state;
								failBackslash = CPPIS_BLOCK_COMMENT;
								state = CPPIS_STRAY_BACKSLASH;
								break;
							default:
								state = CPPIS_BLOCK_COMMENT;
								break;
						}
						break;
					case CPPIS_STRING_LITERAL:
						switch(c) {
							case '"':
								state = CPPIS_NONEMPTY;
								break;
							case '\\':
								state = CPPIS_STRING_ESCAPE;
								break;
							newline:
								state = CPPIS_EMPTY;
								break;
							default:
								break;
						}
						break;
					case CPPIS_STRING_ESCAPE:
						state = CPPIS_STRING_LITERAL;
						break;
					case CPPIS_CHAR_LITERAL:
						switch(c) {
							case '\'':
								state = CPPIS_NONEMPTY;
								break;
							case '\\':
								state = CPPIS_CHAR_ESCAPE;
								break;
							newline:
								state = CPPIS_EMPTY;
								break;
							default:
								break;
						}
						break;
					case CPPIS_CHAR_ESCAPE:
						state = CPPIS_CHAR_LITERAL;
						break;
					case CPPIS_STRAY_BACKSLASH:
						switch(c) {
							newline:
								state = beforeBackslash;
								break;
							default:
								state = failBackslash;
								break;
						}
						break;
					default:
						throw ProgrammingError("Unrecognized CPPLanguage include extractor state: "
								+ StringUtils::toString(static_cast<int>(state)));
				}
				#undef whitespace
				#undef newline
				#undef or_bail
				#undef or_literal
				#undef or_comment
				#undef or_backslash
				#undef or_defaults
			}
			line.clear();
		}
		closer.close();
		sink.doneAppending();
	}

}}
