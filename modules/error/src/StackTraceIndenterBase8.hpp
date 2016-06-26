#ifndef REDSTRAIN_MOD_ERROR_STACKTRACEINDENTERBASE8_HPP
#define REDSTRAIN_MOD_ERROR_STACKTRACEINDENTERBASE8_HPP

#include "IndentingStackTraceSink.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StackTraceIndenterBase8 : public IndentingStackTraceSink::Indenter {

	  private:
		std::string marginString, indentString;
		unsigned marginWidth, indentWidth;

	  protected:
		virtual void writeString(const std::string&) = 0;

	  public:
		StackTraceIndenterBase8();
		StackTraceIndenterBase8(const StackTraceIndenterBase8&);
		virtual ~StackTraceIndenterBase8();

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
		virtual void skip(unsigned);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STACKTRACEINDENTERBASE8_HPP */
