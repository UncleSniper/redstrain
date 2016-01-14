#include <cctype>
#include <cstring>

#include "StringUtils.hpp"
#include "types.hpp"

using std::list;
using std::deque;
using std::vector;
using std::string;

namespace redengine {
namespace util {

	/**
	 * Test whether a string is a prefix of another.
	 * Return whether @c haystack starts with <tt>needle</tt>,
	 * i.e. the front @em n characters of @c haystack equate
	 * the characters of <tt>needle</tt>, where @em n is the
	 * length of <tt>needle</tt>, in characters. If @c haystack
	 * has fewer than @em n characters, the test is considered
	 * negative.
	 *
	 * @param haystack the string to be searched, i.e. the
	 * "containing" string
	 * @param needle the string to be searched for, i.e.
	 * the "contained" string
	 * @return @c true if and only of @c needle is a prefix
	 * of @c haystack
	 *
	 * @since 0.1.0
	 */
	bool StringUtils::startsWith(const string& haystack, const string& needle) {
		string::size_type nlen = needle.length();
		if(nlen > haystack.length())
			return false;
		return haystack.substr(static_cast<string::size_type>(0u), nlen) == needle;
	}

	/**
	 * Test whether a string is a suffix of another.
	 * Return whether @c haystack ends with <tt>needle</tt>,
	 * i.e. the back @em n characters of @c haystack equate
	 * the characters of <tt>needle</tt>, where @em n is the
	 * length of <tt>needle</tt>, in characters. If @c haystack
	 * has fewer than @em n characters, the test is considered
	 * negative.
	 *
	 * @param haystack the string to be searched, i.e. the
	 * "containing" string
	 * @param needle the string to be searched for, i.e.
	 * the "contained" string
	 * @return @c true if and only of @c needle is a suffix
	 * of @c haystack
	 *
	 * @since 0.1.0
	 */
	bool StringUtils::endsWith(const string& haystack, const string& needle) {
		string::size_type hlen = haystack.length(), nlen = needle.length();
		if(nlen > hlen)
			return false;
		return haystack.substr(hlen - nlen) == needle;
	}

	/**
	 * Split a string by another. That is, @c haystack is decomposed
	 * as a sequence of <em>parts</em>, each two adjacent @em parts
	 * being seperated by an interjacent occurence of @c needle as
	 * a substring of <tt>haystack</tt>. This decomposition gives
	 * rise to a sequence of substrings, each element of which is
	 * either a @em part or an occurence of @c needle called a
	 * <em>separator</em>. This sequence is appended to the specified
	 * <tt>sink</tt>, element by element. If the KEEP_SEPARATORS bit
	 * is set in <tt>flags</tt>, all such elements are appended;
	 * otherwise, only @em part elements are appended and @em separator
	 * elements are omitted.
	 *
	 * Any empty (zero-length) @em part is assigned to be in one
	 * of three sets called <em>front</em>, @em center and
	 * <em>back</em>.
	 * - Any empty @em part not preceded by any non-empty @em parts
	 *   is assigned to the @em front set (which therefore contains
	 *   the largest prefix of empty @em parts in the sequence).
	 * - Any empty @em part not followed by any non-empty @em parts
	 *   is assigned to the @em back set, unless it was already
	 *   assigned to the @em front set by the previous rule.
	 * - Any empty @em part not assigned to either of the @em front
	 *   or @em back sets by the two previous rules is assigned to
	 *   the @em center set.
	 *
	 * Which of the empty @em parts are appended to @c sink is
	 * then governed by their membership in one of these sets
	 * as well as the value of <tt>flags</tt>:
	 * - If and only if the TRIM_FRONT bit is set in <tt>flags</tt>,
	 *   the parts in the @em front set are not appended.
	 * - If and only if the TRIM_BACK bit is set, the parts in the
	 *   @em back set are not appended.
	 * - If and only if the TRIM_CENTER bit is set, the parts in
	 *   the @em center set are not appended.
	 *
	 * If @c needle is empty, @c haystack as a whole is appended to
	 * @c sink as a single element, regardless of the value of
	 * <tt>flags</tt>, and the function returns immediately.
	 *
	 * @param haystack the string to be searched
	 * @param needle the separator string to be searched for,
	 * separating the @em parts
	 * @param sink the appender to which the resulting sequence
	 * of substrings of @c haystack is appended
	 * @param flags bitwise disjunction of any subset of the
	 * constants in Flags, governing which substrings are
	 * appended; set bits not corresponding to any of the
	 * constants mentioned above are ignored
	 *
	 * @since 0.1.0
	 */
	void StringUtils::split(const string& haystack, const string& needle, Appender<string>& sink, int flags) {
		if(needle.empty()) {
			sink.append(haystack);
			sink.doneAppending();
			return;
		}
		bool atFront = true;
		MemorySize skipped = static_cast<MemorySize>(0u);
		string::size_type pos, old = static_cast<string::size_type>(0u);
		while((pos = haystack.find(needle, old)) != string::npos) {
			if(pos > old) {
				if(flags & TRIM_CENTER) {
					if(flags & KEEP_SEPARATORS)
						for(; skipped; --skipped)
							sink.append(needle);
					else
						skipped = static_cast<MemorySize>(0u);
				}
				else {
					for(; skipped; --skipped) {
						sink.append("");
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
					sink.append("");
				if(flags & KEEP_SEPARATORS)
					sink.append(needle);
			}
			else if(flags & (TRIM_CENTER | TRIM_BACK))
				++skipped;
			else {
				sink.append("");
				if(flags & KEEP_SEPARATORS)
					sink.append(needle);
			}
			old = pos + needle.length();
		}
		if(old < haystack.length()) {
			if(!(flags & (atFront ? TRIM_FRONT : TRIM_CENTER))) {
				for(; skipped; --skipped) {
					sink.append("");
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
				sink.append("");
				if(flags & KEEP_SEPARATORS)
					sink.append(needle);
			}
			sink.append("");
		}
		else if(flags & KEEP_SEPARATORS) {
			for(; skipped; --skipped)
				sink.append(needle);
		}
		sink.doneAppending();
	}

	/**
	 * Join a sequence of strings into one string. This function
	 * is a shorthand for <tt>joinAll</tt>, in which the sequence iterators
	 * are initialized from the @c begin() and @c end() member
	 * functions of <tt>parts</tt>. Calling this function is
	 * thus equivalent to:
	 * @code
	   joinAll(parts.begin(), parts.end(), glue, emptyReplacement)
	 * @endcode
	 *
	 * @param parts sequence of strings to join
	 * @param glue string to insert between two elements of
	 * the sequence
	 * @param emptyReplacement string to insert instead of
	 * empty elements of the sequence
	 * @return a string containing the elements of the sequence
	 * interspersed with @c glue
	 *
	 * @see joinAll
	 * @since 0.1.0
	 */
	string StringUtils::join(const list<string>& parts, const string& glue, const string& emptyReplacement) {
		return joinAll(parts.begin(), parts.end(), glue, emptyReplacement);
	}

	/**
	 * Join a sequence of strings into one string. This function
	 * is a shorthand for <tt>joinAll</tt>, in which the sequence iterators
	 * are initialized from the @c begin() and @c end() member
	 * functions of <tt>parts</tt>. Calling this function is
	 * thus equivalent to:
	 * @code
	   joinAll(parts.begin(), parts.end(), glue, emptyReplacement)
	 * @endcode
	 *
	 * @param parts sequence of strings to join
	 * @param glue string to insert between two elements of
	 * the sequence
	 * @param emptyReplacement string to insert instead of
	 * empty elements of the sequence
	 * @return a string containing the elements of the sequence
	 * interspersed with @c glue
	 *
	 * @see joinAll
	 * @since 0.1.0
	 */
	string StringUtils::join(const deque<string>& parts, const string& glue, const string& emptyReplacement) {
		return joinAll(parts.begin(), parts.end(), glue, emptyReplacement);
	}

	/**
	 * Join a sequence of strings into one string. This function
	 * is a shorthand for <tt>joinAll</tt>, in which the sequence iterators
	 * are initialized from the @c begin() and @c end() member
	 * functions of <tt>parts</tt>. Calling this function is
	 * thus equivalent to:
	 * @code
	   joinAll(parts.begin(), parts.end(), glue, emptyReplacement)
	 * @endcode
	 *
	 * @param parts sequence of strings to join
	 * @param glue string to insert between two elements of
	 * the sequence
	 * @param emptyReplacement string to insert instead of
	 * empty elements of the sequence
	 * @return a string containing the elements of the sequence
	 * interspersed with @c glue
	 *
	 * @see joinAll
	 * @since 0.1.0
	 */
	string StringUtils::join(const vector<string>& parts, const string& glue, const string& emptyReplacement) {
		return joinAll(parts.begin(), parts.end(), glue, emptyReplacement);
	}

	const unsigned char StringUtils::WHITESPACE[32] = {
		'\x00', '\x36', // 0..15
		'\x00', '\x00', // 16..31
		'\x01', '\x00', // 32..47
		'\x00', '\x00', // 48..63
		'\x00', '\x00', // 64..79
		'\x00', '\x00', // 80..95
		'\x00', '\x00', // 96..111
		'\x00', '\x00', // 112..127
		'\x00', '\x00', // 128..143
		'\x00', '\x00', // 144..159
		'\x00', '\x00', // 160..175
		'\x00', '\x00', // 176..191
		'\x00', '\x00', // 192..207
		'\x00', '\x00', // 208..223
		'\x00', '\x00', // 224..239
		'\x00', '\x00'  // 240..255
	};

	/**
	 * Remove occurences of whitespace from a string.
	 * This function is shorthand for the
	 * @link trim(const std::string&, const unsigned char*, int) table version@endlink
	 * of @c trim, in which the @link WHITESPACE whitespace table@endlink
	 * is used as the <em>trim table</em>.
	 * Calling this function is thus equivalent to:
	 * @code
	   trim(subject, WHITESPACE, flags)
	 * @endcode
	 *
	 * @param subject the string to trim
	 * @param flags bitwise disjunction of any subset of
	 * the constants in Flags, governing which parts of
	 * @c subject are removed
	 * @return a copy of @c subject with whitespace removed
	 *
	 * @see trim(const std::string&, const unsigned char*, int)
	 * @since 0.1.0
	 */
	string StringUtils::trim(const string& subject, int flags) {
		return trim(subject, WHITESPACE, flags);
	}

	/**
	 * Remove occurences of specified characters from a string.
	 * This function is shorthand for the
	 * @link trim(const std::string&, const unsigned char*, int) table version@endlink
	 * of @c trim, in which the <em>trim table</em> is
	 * constructed from the characters in <tt>trimChars</tt>.
	 * That is, this function creates a temporary <em>trim table</em>,
	 * as with @link prepareTrim(const std::string&, unsigned char*) prepareTrim@endlink,
	 * and then calls @c trim with that table.
	 *
	 * @param subject the string to trim
	 * @param trimChars characters to remove from @c subject
	 * @param flags bitwise disjunction of any subset of
	 * the constants in Flags, governing which parts of
	 * @c subject are removed
	 * @return a copy of @c subject with characters present
	 * in @c trimChars removed
	 *
	 * @see trim(const std::string&, const unsigned char*, int)
	 * @since 0.1.0
	 */
	string StringUtils::trim(const string& subject, const string& trimChars, int flags) {
		unsigned char lut[32];
		prepareTrim(trimChars, lut);
		return trim(subject, lut, flags);
	}

	/**
	 * Remove occurences of specified characters from a string.
	 * The returned string equates the given <tt>subject</tt>,
	 * except that occurences of the characters denoted by the
	 * table @c lut are removed from it according to the semantics
	 * specified by <tt>flags</tt>.
	 *
	 * The @c lut argument refers to a <em>trim table</em> partitioning
	 * the set of possible characters (i.e. values of type
	 * <tt>unsigned char</tt>) into two subsets: @em Trimmable
	 * characters are to be removed from the string given,
	 * @em retained characters are to be kept. The memory region
	 * pointed to by @c lut assigns one bit to each such character
	 * value; if that bit is set (1), the character is
	 * <em>trimmable</em>; if the bit is unset (0), the character
	 * is <em>retained</em>. The table is therefore 32 bytes in
	 * size, as 2<sup><tt>sizeof(unsigned char)</tt></sup>
	 * / 8 = 32. Lower-address bytes correspond to
	 * lower character codes, as do less significant bits within
	 * those bytes; e.g. <tt>(*lut & 1u)</tt> corresponds to the
	 * character value @c '\\0' and <tt>(lut[31] & 128u)</tt>
	 * corresponds to <tt>'\\xff'</tt>. The typical way to
	 * build (i.e. set the bytes in) such a <em>trim table</em>
	 * is by calling
	 * @link prepareTrim(const std::string&, unsigned char*) prepareTrim@endlink.
	 *
	 * Any @em trimmable character (in the sense of corresponding
	 * to an index into <tt>subject</tt>, as opposed to the
	 * character value per se) is assigned to be in one of
	 * three sets called <em>front</em>, @em center and <em>back</em>.
	 * - Any @em trimmable character not preceded by any
	 *   @em retained characters is assigned to the @em front set
	 *   (which therefore contains the largest prefix of
	 *   @em trimmable characters in @c subject).
	 * - Any @em trimmable character not followed by any
	 *   @em retained characters is assigned to the @em back set,
	 *   unless it was already assigned to the @em front set
	 *   by the previous rule.
	 * - Any @em trimmable character not assigned to either of
	 *   the @em front or @em back sets by the two previous
	 *   rules is assigned to the @em center set.
	 *
	 * Which characters are removed from @c subject is then
	 * governed by their membership in one of these sets as
	 * well as the value of <tt>flags</tt>:
	 * - If and only if the TRIM_FRONT bit is set in <tt>flags</tt>,
	 *   the characters in the @em front set are removed.
	 * - If and only if the TRIM_BACK bit is set, the characters
	 *   in the @em back set are removed.
	 * - If and only if the TRIM_CENTER bit is set, the characters
	 *   in the @em center set are removed.
	 *
	 * @param subject the string to trim
	 * @param lut the <em>trim table</em> indicating which
	 * characters values are to be removed; must point to
	 * an object of size >= 32
	 * @param flags bitwise disjunction of any subset of the
	 * constants in Flags, governing which characters are
	 * removed; set bits not corresponding to any of the
	 * constants mentioned above are ignored
	 * @return a copy of @c subject with characters deemed
	 * @em trimmable by @c lut removed, if so dictated by
	 * @c flags
	 *
	 * @since 0.1.0
	 */
	string StringUtils::trim(const string& subject, const unsigned char* lut, int flags) {
		string result;
		result.reserve(subject.length());
		bool atFront = true;
		const unsigned char* data = reinterpret_cast<const unsigned char*>(subject.data());
		const unsigned char *end = data + subject.length(), *skipped = NULL;
		for(; data != end; ++data) {
			unsigned code = static_cast<unsigned>(*data);
			if(lut[code / 8u] & (1u << (code % 8u))) {
				if(atFront) {
					if(!(flags & TRIM_FRONT))
						result += *reinterpret_cast<const char*>(data);
				}
				else if(flags & (TRIM_CENTER | TRIM_BACK)) {
					if(!skipped)
						skipped = data;
				}
				else
					result += *reinterpret_cast<const char*>(data);
			}
			else if(atFront) {
				result += *reinterpret_cast<const char*>(data);
				atFront = false;
			}
			else {
				if(skipped && !(flags & TRIM_CENTER))
					result.append(reinterpret_cast<const char*>(skipped),
							static_cast<string::size_type>(data + 1 - skipped));
				else
					result += *reinterpret_cast<const char*>(data);
				skipped = NULL;
			}
		}
		if(skipped && !(flags & TRIM_BACK))
			result.append(reinterpret_cast<const char*>(skipped), static_cast<string::size_type>(end - skipped));
		return result;
	}

	/**
	 * Build a trim table. The
	 * @link trim(const std::string&, const unsigned char*, int) table@endlink
	 * pointed to by @c lut must be an object at least
	 * 32 bytes in size. Those bytes are then set in a
	 * manner such that the <em>trim table</em> will deem
	 * the character values and only the character values
	 * present in @c trimChars @em trimmable characters.
	 * Typically, a table will be built once and then
	 * @c trim will be called many times with that table.
	 *
	 * @param trimChars the characters to be removed from subject
	 * strings in subsequent calls to @c trim when using
	 * @c lut as the <em>trim table</em>
	 * @param lut the <em>trim table</em> to be initialized;
	 * must point to an object of size >= 32
	 *
	 * @see trim(const std::string&, const unsigned char*, int)
	 * @since 0.1.0
	 */
	void StringUtils::prepareTrim(const string& trimChars, unsigned char* lut) {
		memset(lut, 0, static_cast<size_t>(32u));
		const unsigned char* data = reinterpret_cast<const unsigned char*>(trimChars.data());
		const unsigned char* end = data + trimChars.length();
		for(; data != end; ++data) {
			unsigned code = static_cast<unsigned>(*data);
			lut[code / 8u] |= 1u << (code % 8u);
		}
	}

	/**
	 * Replace the first occurence of a character in
	 * a string. The frontmost occurence of @c find
	 * in @c subject is replaced with <tt>replaceWith</tt>.
	 * If @c find does not occur in <tt>subject</tt>,
	 * no replacement is performed and @c subject is
	 * returned verbatim.
	 *
	 * @param subject the string in which to replace
	 * the character
	 * @param find the character value to be replaced
	 * @param replaceWith the character value to
	 * replace the first occurence of @c find with
	 * @return a copy of @c subject with the first
	 * occurence of @c find replaced with @c replaceWith
	 *
	 * @since 0.1.0
	 */
	string StringUtils::replaceFirst(const string& subject, char find, char replaceWith) {
		string result(subject);
		string::size_type pos = result.find(find);
		if(pos != string::npos)
			result[pos] = replaceWith;
		return result;
	}

	/**
	 * Replace the first occurence of a substring in
	 * a string. The frontmost occurence of @c find
	 * in @c subject is replaced with <tt>replaceWith</tt>.
	 * If @c find does not occur in <tt>subject</tt>,
	 * no replacement is performed and @c subject is
	 * returned verbatim.
	 *
	 * @param subject the string in which to replace
	 * the substring
	 * @param find the substring to be replaced
	 * @param replaceWith the substring to
	 * replace the first occurence of @c find with
	 * @return a copy of @c subject with the first
	 * occurence of @c find replaced with @c replaceWith
	 *
	 * @since 0.1.0
	 */
	string StringUtils::replaceFirst(const string& subject, const string& find, const string& replaceWith) {
		string::size_type pos = subject.find(find);
		if(pos == string::npos)
			return subject;
		string result;
		result.reserve(subject.length() + replaceWith.length() - find.length());
		result.append(subject.substr(static_cast<string::size_type>(0u), pos));
		result.append(replaceWith);
		result.append(subject.substr(pos + find.length()));
		return result;
	}

	/**
	 * Replace all occurences of a character in
	 * a string. All occurences of @c find
	 * in @c subject are replaced with <tt>replaceWith</tt>.
	 * If @c find does not occur in <tt>subject</tt>,
	 * no replacement is performed and @c subject is
	 * returned verbatim. If <tt>find == replaceWith</tt>,
	 * @c subject is returned verbatim.
	 *
	 * @param subject the string in which to replace
	 * the character
	 * @param find the character value to be replaced
	 * @param replaceWith the character value to
	 * replace all occurences of @c find with
	 * @return a copy of @c subject with the all
	 * occurences of @c find replaced with @c replaceWith
	 *
	 * @since 0.1.0
	 */
	string StringUtils::replaceAll(const string& subject, char find, char replaceWith) {
		string result(subject);
		string::size_type pos, old = static_cast<string::size_type>(0u);
		while((pos = subject.find(find, old)) != string::npos) {
			result[pos] = replaceWith;
			pos = old + static_cast<string::size_type>(1u);
		}
		return result;
	}

	/**
	 * Replace all occurences of a substring in
	 * a string. All non-overlapping occurences of @c find
	 * in @c subject are replaced with <tt>replaceWith</tt>.
	 * If @c find does not occur in <tt>subject</tt>,
	 * no replacement is performed and @c subject is
	 * returned verbatim. If <tt>find == replaceWith</tt>,
	 * @c subject is returned verbatim. For any occurence
	 * in the above sense, the next occurence in the general
	 * sense must start after the replacement in order to
	 * be deemed an occurence in the above sense; i.e.
	 * the potential circumstance that @c replaceWith
	 * might contain <tt>find</tt>, or end in a suffix thereof,
	 * is disregarded.
	 *
	 * @param subject the string in which to replace
	 * the substring
	 * @param find the substring to be replaced
	 * @param replaceWith the substring to
	 * replace all occurences of @c find with
	 * @return a copy of @c subject with the all
	 * occurences of @c find replaced with @c replaceWith
	 *
	 * @since 0.1.0
	 */
	string StringUtils::replaceAll(const string& subject, const string& find, const string& replaceWith) {
		if(find.empty())
			return subject;
		string result;
		string::size_type pos, old = static_cast<string::size_type>(0u);
		while((pos = subject.find(find, old)) != string::npos) {
			result.append(subject.substr(old, pos - old));
			result.append(replaceWith);
			old = pos + find.length();
		}
		result.append(subject.substr(old));
		return result;
	}

	/**
	 * Convert string to upper case. All characters in @c subject
	 * are replaced with their upper case counterpart, as per
	 * <tt>toupper(3)</tt>. (Consequently, characters without such
	 * a counterpart, such as digits, are retained unchanged.)
	 *
	 * @param subject the string to convert to upper case
	 * @return a copy of @c subject in which all characters
	 * have been converted to upper case
	 *
	 * @since 0.1.0
	 */
	string StringUtils::toUpperCase(const string& subject) {
		string result(subject);
		string::iterator begin(result.begin()), end(result.end());
		for(; begin != end; ++begin)
			*begin = static_cast<char>(static_cast<unsigned char>(static_cast<unsigned>(
					toupper(static_cast<int>(static_cast<unsigned>(static_cast<unsigned char>(*begin)))))));
		return result;
	}

	/**
	 * Convert string to lower case. All characters in @c subject
	 * are replaced with their lower case counterpart, as per
	 * <tt>tolower(3)</tt>. (Consequently, characters without such
	 * a counterpart, such as digits, are retained unchanged.)
	 *
	 * @param subject the string to convert to lower case
	 * @return a copy of @c subject in which all characters
	 * have been converted to lower case
	 *
	 * @since 0.1.0
	 */
	string StringUtils::toLowerCase(const string& subject) {
		string result(subject);
		string::iterator begin(result.begin()), end(result.end());
		for(; begin != end; ++begin)
			*begin = static_cast<char>(static_cast<unsigned char>(static_cast<unsigned>(
					tolower(static_cast<int>(static_cast<unsigned>(static_cast<unsigned char>(*begin)))))));
		return result;
	}

	/**
	 * Convert first character of a string to upper case.
	 * The frontmost character in @c subject (i.e.  @c subject[0])
	 * is replaced with its upper case counterpart, as per
	 * <tt>toupper(3)</tt>. Consequently, if that character does
	 * not have such a counterpart, @c subject is returned
	 * verbatim. If @c subject is empty, it is returned
	 * verbatim. Note that all other characters are retained
	 * unchanged.
	 *
	 * @param subject the string whose first character to
	 * convert to upper case
	 * @return a copy of @c subject in which the frontmost
	 * character has been converted to upper case
	 *
	 * @since 0.1.0
	 */
	string StringUtils::capitalize(const string& subject) {
		if(subject.empty())
			return subject;
		string result(subject);
		result[0] = static_cast<char>(static_cast<unsigned char>(static_cast<unsigned>(
				toupper(static_cast<int>(static_cast<unsigned>(static_cast<unsigned char>(result[0])))))));
		return result;
	}

	/**
	 * Caplitalize words in a string. All letter characters,
	 * as per <tt>isalpha(3)</tt>, are replaced with their
	 * upper case counterparts, as per <tt>toupper(3)</tt>,
	 * if and only if they are directly preceded by either
	 * a non-letter character or no character at all (i.e.
	 * beginning-of-string).
	 *
	 * @param subject the string whose words to capitalize
	 * @return a copy of @c subject in which all
	 * letter characters starting words have been converted
	 * to upper case
	 *
	 * @since 0.1.0
	 */
	string StringUtils::capitalizeAll(const string& subject) {
		string result(subject);
		bool atStart = true;
		string::iterator begin(result.begin()), end(result.end());
		for(; begin != end; ++begin) {
			int c = static_cast<int>(static_cast<unsigned>(static_cast<unsigned char>(*begin)));
			if(isalpha(c)) {
				if(atStart) {
					*begin = static_cast<char>(static_cast<unsigned char>(static_cast<unsigned>(toupper(c))));
					atStart = false;
				}
			}
			else
				atStart = true;
		}
		return result;
	}

}}
