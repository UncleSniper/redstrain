#ifndef REDSTRAIN_MOD_TEXT_STRINGUTILS_HPP
#define REDSTRAIN_MOD_TEXT_STRINGUTILS_HPP

#include <list>
#include <deque>
#include <string>
#include <vector>
#include <redstrain/util/Appender.hpp>
#include <redstrain/util/types.hpp>

namespace redengine {
namespace text {

	template<typename CharT>
	class StringUtils {

	  public:
		typedef std::basic_string<CharT> String;

		enum Flags {
			TRIM_FRONT      = 001,
			TRIM_BACK       = 002,
			TRIM_CENTER     = 004,
			KEEP_SEPARATORS = 010
		};

		class DefaultWhitespaceTrimPredicate {

		  public:
			bool operator()(CharT c) const {
				switch(c) {
					case static_cast<CharT>(' '):
					case static_cast<CharT>('\t'):
					case static_cast<CharT>('\r'):
					case static_cast<CharT>('\n'):
					case static_cast<CharT>('\f'):
						return true;
					default:
						return false;
				}
			}

		};

		class StringElementsTrimPredicate {

		  private:
			const String& trimChars;

		  public:
			StringElementsTrimPredicate(const String& trimChars) : trimChars(trimChars) {}
			StringElementsTrimPredicate(const StringElementsTrimPredicate& predicate)
					: trimChars(predicate.trimChars) {}

			bool operator()(CharT c) const {
				return trimChars.find(c) != String::npos;
			}

		};

	  public:
		static bool startsWith(const String& haystack, const String& needle) {
			typename String::size_type nlen = needle.length();
			if(nlen > haystack.length())
				return false;
			return haystack.substr(static_cast<typename String::size_type>(0u), nlen) == needle;
		}

		static bool endsWith(const String& haystack, const String& needle) {
			typename String::size_type hlen = haystack.length(), nlen = needle.length();
			if(nlen > hlen)
				return false;
			return haystack.substr(hlen - nlen) == needle;
		}

		static void split(const String& haystack, const String& needle, util::Appender<String>& sink,
				int flags = 0) {
			if(needle.empty()) {
				sink.append(haystack);
				sink.doneAppending();
				return;
			}
			bool atFront = true;
			util::MemorySize skipped = static_cast<util::MemorySize>(0u);
			typename String::size_type pos, old = static_cast<typename String::size_type>(0u);
			while((pos = haystack.find(needle, old)) != String::npos) {
				if(pos > old) {
					if(flags & TRIM_CENTER) {
						if(flags & KEEP_SEPARATORS)
							for(; skipped; --skipped)
								sink.append(needle);
						else
							skipped = static_cast<util::MemorySize>(0u);
					}
					else {
						for(; skipped; --skipped) {
							sink.append(String());
							if(flags & KEEP_SEPARATORS)
								sink.append(needle);
						}
					}
					sink.append(haystack.substr(old, pos - old));
					if(flags & KEEP_SEPARATORS)
						sink.append(needle);
					atFront = false;
				}
				else if(atFront) {
					if(!(flags & TRIM_FRONT))
						sink.append(String());
					if(flags & KEEP_SEPARATORS)
						sink.append(needle);
				}
				else if(flags & (TRIM_CENTER | TRIM_BACK))
					++skipped;
				else {
					sink.append(String());
					if(flags & KEEP_SEPARATORS)
						sink.append(needle);
				}
				old = pos + needle.length();
			}
			if(old < haystack.length()) {
				if(!(flags & (atFront ? TRIM_FRONT : TRIM_CENTER))) {
					for(; skipped; --skipped) {
						sink.append(String());
						if(flags & KEEP_SEPARATORS)
							sink.append(needle);
					}
				}
				else if(flags & KEEP_SEPARATORS) {
					for(; skipped; --skipped)
						sink.append(needle);
				}
				sink.append(haystack.substr(old));
			}
			else if(!(flags & (atFront ? TRIM_FRONT : TRIM_BACK))) {
				for(; skipped; --skipped) {
					sink.append(String());
					if(flags & KEEP_SEPARATORS)
						sink.append(needle);
				}
				sink.append(String());
			}
			else if(flags & KEEP_SEPARATORS) {
				for(; skipped; --skipped)
					sink.append(needle);
			}
			sink.doneAppending();
		}

		static String join(const std::list<String>& parts, const String& glue = String(),
				const String& emptyReplacement = String()) {
			return joinAll(parts.begin(), parts.end(), glue, emptyReplacement);
		}

		static String join(const std::deque<String>& parts, const String& glue = String(),
				const String& emptyReplacement = String()) {
			return joinAll(parts.begin(), parts.end(), glue, emptyReplacement);
		}

		static String join(const std::vector<String>& parts, const String& glue = String(),
				const String& emptyReplacement = String()) {
			return joinAll(parts.begin(), parts.end(), glue, emptyReplacement);
		}

		static String trim(const String& subject, int flags = TRIM_FRONT | TRIM_BACK) {
			DefaultWhitespaceTrimPredicate predicate;
			return trimBy(subject, predicate, flags);
		}

		static String trim(const String& subject, const String& trimChars, int flags = TRIM_FRONT | TRIM_BACK) {
			StringElementsTrimPredicate predicate(trimChars);
			return trimBy(subject, predicate, flags);
		}

		template<typename PredicateT>
		static String trimBy(const String& subject, PredicateT trimCharPredicate,
				int flags = TRIM_FRONT | TRIM_BACK) {
			String result;
			result.reserve(subject.length());
			bool atFront = true;
			const CharT* data = subject.data();
			const CharT *end = data + subject.length(), *skipped = NULL;
			for(; data != end; ++data) {
				if(trimCharPredicate(*data)) {
					if(atFront) {
						if(!(flags & TRIM_FRONT))
							result += *data;
					}
					else if(flags & (TRIM_CENTER | TRIM_BACK)) {
						if(!skipped)
							skipped = data;
					}
					else
						result += *data;
				}
				else if(atFront) {
					result += *data;
					atFront = false;
				}
				else {
					if(skipped && !(flags & TRIM_CENTER))
						result.append(skipped, static_cast<typename String::size_type>(data + 1 - skipped));
					else
						result += *data;
					skipped = NULL;
				}
			}
			if(skipped && !(flags & TRIM_BACK))
				result.append(skipped, static_cast<typename String::size_type>(end - skipped));
			return result;
		}

		static String replaceFirst(const String& subject, CharT find, CharT replaceWith) {
			String result(subject);
			typename String::size_type pos = result.find(find);
			if(pos != String::npos)
				result[pos] = replaceWith;
			return result;
		}

		static String replaceFirst(const String& subject, const String& find, const String& replaceWith) {
			typename String::size_type pos = subject.find(find);
			if(pos == String::npos)
				return subject;
			String result;
			result.reserve(subject.length() + replaceWith.length() - find.length());
			result.append(subject.substr(static_cast<typename String::size_type>(0u), pos));
			result.append(replaceWith);
			result.append(subject.substr(pos + find.length()));
			return result;
		}

		static String replaceAll(const String& subject, CharT find, CharT replaceWith) {
			String result(subject);
			typename String::size_type pos, old = static_cast<typename String::size_type>(0u);
			while((pos = subject.find(find, old)) != String::npos) {
				result[pos] = replaceWith;
				pos = old + static_cast<typename String::size_type>(1u);
			}
			return result;
		}

		static String replaceAll(const String& subject, const String& find, const String& replaceWith) {
			if(find.empty())
				return subject;
			String result;
			typename String::size_type pos, old = static_cast<typename String::size_type>(0u);
			while((pos = subject.find(find, old)) != String::npos) {
				result.append(subject.substr(old, pos - old));
				result.append(replaceWith);
				old = pos + find.length();
			}
			result.append(subject.substr(old));
			return result;
		}

		template<typename IteratorT>
		static String joinAll(const IteratorT& first, const IteratorT& last, const String& glue = String(),
				const String& emptyReplacement = String()) {
			String result;
			IteratorT it(first);
			for(; it != last; ++it) {
				if(it != first)
					result.append(glue);
				result.append(it->empty() ? emptyReplacement : *it);
			}
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_STRINGUTILS_HPP */
