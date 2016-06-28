#ifndef REDSTRAIN_MOD_TEXT_STACKTRACEINDENTERBASE16_HPP
#define REDSTRAIN_MOD_TEXT_STACKTRACEINDENTERBASE16_HPP

#include <redstrain/error/IndentingStackTraceSink.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API StackTraceIndenterBase16 : public error::IndentingStackTraceSink::Indenter {

	  public:
		static const String16 DEFAULT_INDENT_STRING;

	  private:
		String16 marginString, indentString;
		unsigned marginWidth, indentWidth;

	  protected:
		virtual void writeString(const String16&) = 0;

	  public:
		StackTraceIndenterBase16();
		StackTraceIndenterBase16(const StackTraceIndenterBase16&);
		virtual ~StackTraceIndenterBase16();

		inline const String16& getMarginString() const {
			return marginString;
		}

		void setMarginString(const String16&);

		inline unsigned getMarginWidth() const {
			return marginWidth;
		}

		inline void setMarginWidth(unsigned marginWidth) {
			this->marginWidth = marginWidth;
		}

		unsigned getEffectiveMarginWidth() const;

		inline const String16& getIndentString() const {
			return indentString;
		}

		void setIndentString(const String16&);

		inline unsigned getIndentWidth() const {
			return indentWidth;
		}

		inline void setIndentWidth(unsigned indentWidth) {
			this->indentWidth = indentWidth;
		}

		unsigned getEffectiveIndentWidth() const;

		virtual unsigned indent(unsigned);
		virtual void skip(unsigned);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_STACKTRACEINDENTERBASE16_HPP */
