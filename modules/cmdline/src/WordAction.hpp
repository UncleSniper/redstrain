#ifndef REDSTRAIN_MOD_CMDLINE_WORDACTION_HPP
#define REDSTRAIN_MOD_CMDLINE_WORDACTION_HPP

#include <string>

namespace redengine {
namespace cmdline {

	class WordAction {

	  public:
		WordAction();
		WordAction(const WordAction&);
		virtual ~WordAction();

		virtual void wordEncountered(const std::string&, const std::string&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_WORDACTION_HPP */
