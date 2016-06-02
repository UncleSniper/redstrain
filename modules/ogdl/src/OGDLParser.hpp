#ifndef REDSTRAIN_MOD_OGDL_OGDLPARSER_HPP
#define REDSTRAIN_MOD_OGDL_OGDLPARSER_HPP

#include <stack>
#include <redstrain/io/OutputStream.hpp>
#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace ogdl {

	class OGDLSink;

	class REDSTRAIN_OGDL_API OGDLParser : public io::OutputStream<text::Char16> {

	  private:
		enum State {
			ST_TOP,
			ST_TOP_DOLLAR,
			ST_TOP_CONSTANT,
			ST_AFTER_TOP_CONSTANT,
			ST_AFTER_CONSTANT_ASSIGN
		};

		enum Nesting {
			NEST_FOO//TODO
		};

	  private:
		OGDLSink& sink;
		text::String16 file;
		unsigned line, column;
		text::Char16 breakChar;
		State state;
		std::stack<Nesting> nesting;

	  protected:
		OGDLParser(const OGDLParser&);

		virtual void writeBlock(const text::Char16*, util::MemorySize);

	  public:
		OGDLParser(OGDLSink&, const text::String16& = text::String16());
		virtual ~OGDLParser();

		inline OGDLSink& getSink() {
			return sink;
		}

		inline const OGDLSink& getSink() const {
			return sink;
		}

		inline const text::String16& getFile() const {
			return file;
		}

		void setFile(const text::String16&);

		inline unsigned getLine() const {
			return line;
		}

		void setLine(unsigned);

		inline unsigned getColumn() const {
			return column;
		}

		void setColumn(unsigned);
		void setLocation(unsigned, unsigned);

		inline text::Char16 getLineBreakCharacter() const {
			return breakChar;
		}

		inline void setLineBreakCharacter(text::Char16 breakChar) {
			this->breakChar = breakChar;
		}

	};

}}

#endif /* REDSTRAIN_MOD_OGDL_OGDLPARSER_HPP */
