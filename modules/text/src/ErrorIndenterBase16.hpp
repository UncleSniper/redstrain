#ifndef REDSTRAIN_MOD_TEXT_ERRORINDENTERBASE16_HPP
#define REDSTRAIN_MOD_TEXT_ERRORINDENTERBASE16_HPP

#include <redstrain/error/IndentingErrorHandler.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ErrorIndenterBase16 : public error::IndentingErrorHandler::Indenter {

	  private:
		String16 indentString;
		unsigned indentWidth;

	  protected:
		virtual void writeString(const String16&) = 0;

	  public:
		ErrorIndenterBase16();
		ErrorIndenterBase16(const ErrorIndenterBase16&);
		virtual ~ErrorIndenterBase16();

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

		virtual unsigned indent();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ERRORINDENTERBASE16_HPP */
