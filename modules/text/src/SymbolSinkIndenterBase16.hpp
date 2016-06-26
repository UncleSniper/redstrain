#ifndef REDSTRAIN_MOD_TEXT_SYMBOLSINKINDENTERBASE16_HPP
#define REDSTRAIN_MOD_TEXT_SYMBOLSINKINDENTERBASE16_HPP

#include <redstrain/redmond/unmangle/IndentingSymbolSink.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API SymbolSinkIndenterBase16 : public redmond::unmangle::IndentingSymbolSink::Indenter {

	  private:
		static const String16 defaultIndentString;

	  private:
		String16 marginString, indentString;
		unsigned marginWidth, indentWidth;

	  protected:
		virtual void writeString(const String16&) = 0;

	  public:
		SymbolSinkIndenterBase16();
		SymbolSinkIndenterBase16(const SymbolSinkIndenterBase16&);
		virtual ~SymbolSinkIndenterBase16();

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

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_SYMBOLSINKINDENTERBASE16_HPP */
