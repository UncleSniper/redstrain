#ifndef REDSTRAIN_MOD_TEXT_CODETABLE_HPP
#define REDSTRAIN_MOD_TEXT_CODETABLE_HPP

namespace redengine {
namespace text {

	template<typename CharT>
	class CodeTable {

	  public:
		typedef CharT Char;

	  public:
		CodeTable() {}
		CodeTable(const CodeTable&) {}
		virtual ~CodeTable() {}

		virtual void close() {}
		virtual char encodeCharacter(CharT) = 0;
		virtual CharT decodeCharacter(char) = 0;
		virtual bool supportsConcurrentLookup() const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODETABLE_HPP */
