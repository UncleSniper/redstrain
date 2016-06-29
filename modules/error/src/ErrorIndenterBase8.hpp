#ifndef REDSTRAIN_MOD_ERROR_ERRORINDENTERBASE8_HPP
#define REDSTRAIN_MOD_ERROR_ERRORINDENTERBASE8_HPP

#include <string>

#include "IndentingErrorHandler.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API ErrorIndenterBase8 : public IndentingErrorHandler::Indenter {

	  private:
		std::string indentString;
		unsigned indentWidth;

	  protected:
		virtual void writeString(const std::string&) = 0;

	  public:
		ErrorIndenterBase8();
		ErrorIndenterBase8(const ErrorIndenterBase8&);
		virtual ~ErrorIndenterBase8();

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

		virtual unsigned indent();

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ERRORINDENTERBASE8_HPP */
