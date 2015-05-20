#ifndef REDSTRAIN_MOD_CMDLINE_OPTIONERRORHANDLER_HPP
#define REDSTRAIN_MOD_CMDLINE_OPTIONERRORHANDLER_HPP

namespace redengine {
namespace cmdline {

	class AmbiguousOptionError;
	class UnrecognizedOptionError;
	class UnexpectedBarewordError;
	class ExcessOptionArgumentError;
	class MissingOptionArgumentError;
	class MissingBarewordError;
	class ConflictingOptionsError;

	class OptionErrorHandler {

	  public:
		OptionErrorHandler();
		OptionErrorHandler(const OptionErrorHandler&);
		virtual ~OptionErrorHandler();

		virtual void handleOptionError(const AmbiguousOptionError&) = 0;
		virtual void handleOptionError(const UnrecognizedOptionError&) = 0;
		virtual void handleOptionError(const ExcessOptionArgumentError&) = 0;
		virtual void handleOptionError(const MissingOptionArgumentError&) = 0;
		virtual void handleOptionError(const UnexpectedBarewordError&) = 0;
		virtual void handleOptionError(const MissingBarewordError&) = 0;
		virtual void handleOptionError(const ConflictingOptionsError&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_OPTIONERRORHANDLER_HPP */
