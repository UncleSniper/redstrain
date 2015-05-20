#ifndef REDSTRAIN_MOD_CMDLINE_OPTIONPARSER_HPP
#define REDSTRAIN_MOD_CMDLINE_OPTIONPARSER_HPP

#include <list>
#include <vector>

#include "WordAction.hpp"
#include "OptionLogic.hpp"
#include "OptionError.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API OptionParser : public WordAction {

	  public:
		typedef std::vector<std::string> Words;
		typedef Words::const_iterator WordIterator;

	  private:
		const OptionLogic& logic;
		WordAction* nonOptionAction;
		Words nonOptionWords;
		std::string pendingName;
		const OptionLogic::Option* pendingOption;
		OptionError::OptionType pendingType;
		bool terminated;

	  private:
		void parseInitialWord(const std::string&);
		void parseSingleOption(const std::string&);
		void parseDoubleOption(const std::string&);
		bool areRemainingShortOptionsValid(const std::string&) const;
		void parseRemainingShortOptions(const std::string&);
		void flushShortOption(const std::string&, const OptionLogic::Option*, bool, bool);

	  public:
		OptionParser(const OptionLogic&, WordAction*);
		OptionParser(const OptionParser&);

		inline const OptionLogic& getLogic() const {
			return logic;
		}

		inline WordAction* getNonOptionAction() const {
			return nonOptionAction;
		}

		inline void setNonOptionAction(WordAction* action) {
			nonOptionAction = action;
		}

		unsigned getNonOptionWordCount() const;
		void getNonOptionWords(WordIterator&, WordIterator&) const;
		void parseWord(const std::string&);
		void parseWords(char**, int);
		void parseWordsProper(const char**, unsigned);
		void parseWords(const std::list<std::string>&);
		void parseWords(const std::vector<std::string>&);

		template<typename IteratorT>
		void parseAllWords(const IteratorT& first, const IteratorT& last) {
			IteratorT it(first);
			for(; it != last; ++it)
				parseWord(*it);
		}

		void endCommandLine();
		void reset();

		virtual void wordEncountered(const std::string&, const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_OPTIONPARSER_HPP */
