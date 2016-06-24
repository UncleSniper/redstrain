#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINKINDENTERBASE8_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINKINDENTERBASE8_HPP

#include "IndentingSymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API SymbolSinkIndenterBase8 : public IndentingSymbolSink::Indenter {

	  private:
		std::string marginString, indentString;
		unsigned marginWidth, indentWidth;

	  protected:
		virtual void writeString(const std::string&) = 0;

	  public:
		SymbolSinkIndenterBase8();
		SymbolSinkIndenterBase8(const SymbolSinkIndenterBase8&);
		virtual ~SymbolSinkIndenterBase8();

		inline const std::string& getMarginString() const {
			return marginString;
		}

		void setMarginString(const std::string&);

		inline unsigned getMarginWidth() const {
			return marginWidth;
		}

		inline void setMarginWidth(unsigned marginWidth) {
			this->marginWidth = marginWidth;
		}

		unsigned getEffectiveMarginWidth() const;

		inline const std::string& getIndentString() const {
			return indentString;
		}

		void setIndentString(const std::string&);

		inline unsigned getIndentWidth() const {
			return indentWidth;
		}

		inline void setIndentWidth(unsigned indentWidth) {
			this->indentWidth = indentWidth;
		}

		unsigned getEffectiveIndentWidth() const;

		virtual unsigned indent(unsigned);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SYMBOLSINKINDENTERBASE8_HPP */
