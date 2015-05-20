#ifndef REDSTRAIN_MOD_CMDLINE_WORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_WORDACTION_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API WordAction {

	  public:
		WordAction();
		WordAction(const WordAction&);
		virtual ~WordAction();

		virtual void wordEncountered(const std::string&, const std::string&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_WORDACTION_HPP */
