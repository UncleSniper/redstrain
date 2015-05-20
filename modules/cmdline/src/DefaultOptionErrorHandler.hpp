#ifndef REDSTRAIN_MOD_CMDLINE_DEFAULTOPTIONERRORHANDLER_HPP
#define REDSTRAIN_MOD_CMDLINE_DEFAULTOPTIONERRORHANDLER_HPP

#include <redstrain/io/FormattedOutputStream.hpp>

#include "OptionErrorHandler.hpp"

namespace redengine {
namespace cmdline {

	class CommandLineError;

	class REDSTRAIN_CMDLINE_API DefaultOptionErrorHandler : public OptionErrorHandler {

	  private:
		std::string progname;
		io::FormattedOutputStream<char>& conerr;

	  private:
		void handleAnyOptionError(const CommandLineError&) const;

	  public:
		DefaultOptionErrorHandler(const std::string&, io::FormattedOutputStream<char>&);
		DefaultOptionErrorHandler(const DefaultOptionErrorHandler&);

		inline const std::string& getProgramName() const {
			return progname;
		}

		inline void setProgramName(const std::string& newProgname) {
			progname = newProgname;
		}

		inline io::FormattedOutputStream<char>& getOutputStream() {
			return conerr;
		}

		inline const io::FormattedOutputStream<char>& getOutputStream() const {
			return conerr;
		}

		virtual void handleOptionError(const AmbiguousOptionError&);
		virtual void handleOptionError(const UnrecognizedOptionError&);
		virtual void handleOptionError(const ExcessOptionArgumentError&);
		virtual void handleOptionError(const MissingOptionArgumentError&);
		virtual void handleOptionError(const UnexpectedBarewordError&);
		virtual void handleOptionError(const MissingBarewordError&);
		virtual void handleOptionError(const ConflictingOptionsError&);

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_DEFAULTOPTIONERRORHANDLER_HPP */
