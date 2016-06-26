#ifndef REDSTRAIN_MOD_ERROR_STDOSTREAMSTACKTRACEINDENTER_HPP
#define REDSTRAIN_MOD_ERROR_STDOSTREAMSTACKTRACEINDENTER_HPP

#include <iostream>

#include "StackTraceIndenterBase8.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StdOStreamStackTraceIndenter : public StackTraceIndenterBase8 {

	  private:
		std::ostream& stream;

	  protected:
		virtual void writeString(const std::string&);

	  public:
		StdOStreamStackTraceIndenter(std::ostream&);
		StdOStreamStackTraceIndenter(const StdOStreamStackTraceIndenter&);

		inline std::ostream& getStream() {
			return stream;
		}

		inline const std::ostream& getStream() const {
			return stream;
		}

		virtual void endLine();

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STDOSTREAMSTACKTRACEINDENTER_HPP */
