#include "OptionParser.hpp"
#include "AmbiguousOptionError.hpp"
#include "UnrecognizedOptionError.hpp"
#include "UnexpectedBarewordError.hpp"
#include "ExcessOptionArgumentError.hpp"
#include "MissingOptionArgumentError.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	OptionParser::OptionParser(const OptionLogic& logic, WordAction* nonOptionAction)
			: logic(logic), nonOptionAction(nonOptionAction), pendingOption(NULL), terminated(false) {}

	OptionParser::OptionParser(const OptionParser& parser)
			: WordAction(parser), logic(parser.logic), nonOptionAction(parser.nonOptionAction),
			nonOptionWords(parser.nonOptionWords), pendingName(parser.pendingName),
			pendingOption(parser.pendingOption), pendingType(parser.pendingType), terminated(parser.terminated) {}

	unsigned OptionParser::getNonOptionWordCount() const {
		return static_cast<unsigned>(nonOptionWords.size());
	}

	void OptionParser::getNonOptionWords(WordIterator& begin, WordIterator& end) const {
		begin = nonOptionWords.begin();
		end = nonOptionWords.end();
	}

	void OptionParser::parseWord(const string& word) {
		if(pendingOption) {
			WordAction* action = pendingOption->getAction();
			if(pendingOption->getArity() == OptionLogic::OPTIONAL_ARGUMENT && !word.empty()
					&& word[0] == logic.getInitiatorSymbol()) {
				pendingOption = NULL;
				action->wordEncountered(pendingName, "");
				parseInitialWord(word);
			}
			else {
				pendingOption = NULL;
				action->wordEncountered(pendingName, word);
			}
		}
		else
			parseInitialWord(word);
	}

	void OptionParser::parseInitialWord(const string& word) {
		// is it '--'?
		if(logic.hasFlags(OptionLogic::OPTION_TERMINATOR) && word == logic.getOptionTerminator()) {
			terminated = true;
			return;
		}
		// is it a bareword?
		string::size_type length = word.length();
		if(terminated || length < static_cast<string::size_type>(2u)
				|| word[0] != logic.getInitiatorSymbol()) {
			if(logic.hasFlags(OptionLogic::BAREWORD_TERMINATES))
				terminated = true;
			if(nonOptionAction)
				nonOptionAction->wordEncountered("", word);
			else
				throw UnexpectedBarewordError(word);
			return;
		}
		// so it's either '--foo'...
		if(logic.hasFlags(OptionLogic::DOUBLE_LONG_OPTIONS) && length > static_cast<string::size_type>(2u)
				&& word[1] == logic.getInitiatorSymbol())
			parseDoubleOption(word);
		// ...or '-foo'
		else
			parseSingleOption(word);
	}

	void OptionParser::parseSingleOption(const string& word) {
		const OptionLogic::Option *shortOption, *longOption;
		string name;
		// is it '-o'?
		if(word.length() == static_cast<string::size_type>(2u)) {
			name = word.substr(static_cast<string::size_type>(1u));
			shortOption = logic.getShortOption(word[1]);
			if(!shortOption)
				shortOption = logic.getLongOption(name);
			if(!shortOption)
				throw UnrecognizedOptionError(name, OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
			if(shortOption->getArity() == OptionLogic::NO_ARGUMENT)
				shortOption->getAction()->wordEncountered(name, "");
			else if(logic.hasFlags(OptionLogic::WORD_SEPARATORS)) {
				pendingName = name;
				pendingOption = shortOption;
				pendingType = OptionError::SINGLE_OPTION;
			}
			else if(shortOption->getArity() == OptionLogic::OPTIONAL_ARGUMENT)
				shortOption->getAction()->wordEncountered(name, "");
			else
				throw MissingOptionArgumentError(name, OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
			return;
		}
		// so it's '-owhatever'
		// could it be long?
		string arg;
		string::size_type sep;
		if(!logic.hasFlags(OptionLogic::SINGLE_LONG_OPTIONS))
			longOption = NULL;
		else if(logic.hasFlags(OptionLogic::SYMBOL_SEPARATORS)) {
			// is it '-foo=bar'?
			sep = word.find(logic.getSeparatorSymbol(), static_cast<string::size_type>(2u));
			if(sep == string::npos)
				name = word.substr(static_cast<string::size_type>(1u));
			else {
				name = word.substr(static_cast<string::size_type>(1u),
						sep - static_cast<string::size_type>(1u));
				arg = word.substr(sep + static_cast<string::size_type>(1u));
			}
			longOption = logic.getLongOption(name);
		}
		else {
			sep = string::npos;
			name = word.substr(static_cast<string::size_type>(1u));
			longOption = logic.getLongOption(name);
		}
		// could it be short?
		bool restIsArg, restIsOptions;
		if(!logic.hasFlags(OptionLogic::SHORT_OPTIONS))
			shortOption = NULL;
		else {
			shortOption = logic.getShortOption(word[1]);
			if(shortOption) {
				switch(shortOption->getArity()) {
					case OptionLogic::OPTIONAL_ARGUMENT:
						restIsArg = true;
						restIsOptions = areRemainingShortOptionsValid(word);
						break;
					case OptionLogic::REQUIRED_ARGUMENT:
						restIsArg = true;
						restIsOptions = false;
						break;
					default:
						restIsArg = false;
						restIsOptions = areRemainingShortOptionsValid(word);
						break;
				}
			}
		}
		// which is it, long or short?
		if(shortOption) {
			if(!longOption) {
				flushShortOption(word, shortOption, restIsArg, restIsOptions);
				return;
			}
			switch(logic.getInlineLongConflictPolicy()) {
				case OptionLogic::LONG_OVER_INLINE:
					break;
				case OptionLogic::INLINE_OVER_LONG:
					flushShortOption(word, shortOption, restIsArg, restIsOptions);
					return;
				default:
					throw AmbiguousOptionError(name, logic.getInitiatorSymbol());
			}
			// long chosen
		}
		else {
			if(!longOption)
				throw UnrecognizedOptionError(word.substr(static_cast<string::size_type>(1u)),
						OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
			// definitely long
		}
		// so it's long
		if(longOption->getArity() != OptionLogic::NO_ARGUMENT) {
			if(sep != string::npos)
				longOption->getAction()->wordEncountered(name, arg);
			else if(logic.hasFlags(OptionLogic::WORD_SEPARATORS)) {
				pendingName = name;
				pendingOption = longOption;
				pendingType = OptionError::SINGLE_OPTION;
			}
			else
				throw MissingOptionArgumentError(name, OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
		}
		else if(sep != string::npos)
			throw ExcessOptionArgumentError(name, OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
		else
			longOption->getAction()->wordEncountered(name, "");
	}

	void OptionParser::flushShortOption(const string& word, const OptionLogic::Option* option,
			bool restIsArg, bool) {
		if(!logic.hasFlags(OptionLogic::INLINE_ARGUMENTS))
			restIsArg = false;
		// perhaps some day we'll allow setting precedence between
		// treating the rest as an argument or further options,
		// but not right now
		if(restIsArg) {
			option->getAction()->wordEncountered(
				word.substr(static_cast<string::size_type>(1u), static_cast<string::size_type>(1u)),
				word.substr(static_cast<string::size_type>(2u))
			);
		}
		else {
			option->getAction()->wordEncountered(word.substr(static_cast<string::size_type>(1u),
					static_cast<string::size_type>(1u)), "");
			parseRemainingShortOptions(word);
		}
	}

	bool OptionParser::areRemainingShortOptionsValid(const string& word) const {
		string::size_type pos = static_cast<string::size_type>(0u), length = word.length();
		for(; pos < length; ++pos) {
			const OptionLogic::Option* option = logic.getShortOption(word[pos]);
			if(!option)
				return false;
			bool hasMore = pos < length - static_cast<string::size_type>(1u);
			switch(option->getArity()) {
				case OptionLogic::OPTIONAL_ARGUMENT:
				case OptionLogic::REQUIRED_ARGUMENT:
					if(!hasMore) {
						if(!logic.hasFlags(OptionLogic::WORD_SEPARATORS) &&
								option->getArity() == OptionLogic::REQUIRED_ARGUMENT)
							return false;
						break;
					}
					if(logic.hasFlags(OptionLogic::INLINE_ARGUMENTS))
						return true;
					if(option->getArity() == OptionLogic::REQUIRED_ARGUMENT)
						return false;
					break;
				default:
					break;
			}
		}
		return true;
	}

	void OptionParser::parseRemainingShortOptions(const string& word) {
		string::size_type pos = static_cast<string::size_type>(0u), length = word.length();
		for(; pos < length; ++pos) {
			string name(word.substr(pos, static_cast<string::size_type>(1u)));
			const OptionLogic::Option* option = logic.getShortOption(word[pos]);
			if(!option)
				throw UnrecognizedOptionError(name, OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
			bool hasMore = pos < length - static_cast<string::size_type>(1u);
			switch(option->getArity()) {
				case OptionLogic::OPTIONAL_ARGUMENT:
				case OptionLogic::REQUIRED_ARGUMENT:
					if(!hasMore) {
						if(logic.hasFlags(OptionLogic::WORD_SEPARATORS)) {
							pendingName = name;
							pendingOption = option;
							pendingType = OptionError::SINGLE_OPTION;
						}
						else if(option->getArity() == OptionLogic::OPTIONAL_ARGUMENT)
							option->getAction()->wordEncountered(name, "");
						else
							throw MissingOptionArgumentError(name, OptionError::SINGLE_OPTION,
									logic.getInitiatorSymbol());
						break;
					}
					if(logic.hasFlags(OptionLogic::INLINE_ARGUMENTS)) {
						option->getAction()->wordEncountered(name,
								word.substr(pos + static_cast<string::size_type>(1u)));
						return;
					}
					if(option->getArity() == OptionLogic::OPTIONAL_ARGUMENT)
						option->getAction()->wordEncountered(name, "");
					else
						throw MissingOptionArgumentError(word.substr(pos, static_cast<string::size_type>(1u)),
								OptionError::SINGLE_OPTION, logic.getInitiatorSymbol());
					break;
				default:
					option->getAction()->wordEncountered(name, "");
					break;
			}
		}
	}

	void OptionParser::parseDoubleOption(const string& word) {
		string name, arg;
		string::size_type sep;
		const OptionLogic::Option* option;
		if(logic.hasFlags(OptionLogic::SYMBOL_SEPARATORS)) {
			// is it '--foo=bar'?
			sep = word.find(logic.getSeparatorSymbol(), static_cast<string::size_type>(3u));
			if(sep == string::npos)
				name = word.substr(static_cast<string::size_type>(2u));
			else {
				name = word.substr(static_cast<string::size_type>(2u),
						sep - static_cast<string::size_type>(2u));
				arg = word.substr(sep + static_cast<string::size_type>(1u));
			}
			option = logic.getLongOption(name);
		}
		else {
			sep = string::npos;
			name = word.substr(static_cast<string::size_type>(2u));
			option = logic.getLongOption(name);
		}
		if(!option)
			throw UnrecognizedOptionError(name, OptionError::DOUBLE_OPTION, logic.getInitiatorSymbol());
		if(option->getArity() != OptionLogic::NO_ARGUMENT) {
			if(sep != string::npos)
				option->getAction()->wordEncountered(name, arg);
			else if(logic.hasFlags(OptionLogic::WORD_SEPARATORS)) {
				pendingName = name;
				pendingOption = option;
				pendingType = OptionError::DOUBLE_OPTION;
			}
			else
				throw MissingOptionArgumentError(name, OptionError::DOUBLE_OPTION, logic.getInitiatorSymbol());
		}
		else if(sep != string::npos)
			throw ExcessOptionArgumentError(name, OptionError::DOUBLE_OPTION, logic.getInitiatorSymbol());
		else
			option->getAction()->wordEncountered(name, "");
	}

	void OptionParser::endCommandLine() {
		if(!pendingOption)
			return;
		if(pendingOption->getArity() == OptionLogic::REQUIRED_ARGUMENT)
			throw MissingOptionArgumentError(pendingName, pendingType, logic.getInitiatorSymbol());
		WordAction* action = pendingOption->getAction();
		pendingOption = NULL;
		action->wordEncountered(pendingName, "");
	}

	void OptionParser::reset() {
		pendingOption = NULL;
		terminated = false;
	}

	void OptionParser::wordEncountered(const string&, const string& value) {
		nonOptionWords.push_back(value);
	}

	void OptionParser::parseWords(char** argv, int argc) {
		if(argc > 1)
			parseAllWords<char**>(argv + 1, argv + argc);
	}

	void OptionParser::parseWordsProper(const char** argv, unsigned argc) {
		parseAllWords<const char**>(argv, argv + argc);
	}

	void OptionParser::parseWords(const std::list<std::string>& words) {
		parseAllWords<std::list<std::string>::const_iterator>(words.begin(), words.end());
	}

	void OptionParser::parseWords(const std::vector<std::string>& words) {
		parseAllWords<std::vector<std::string>::const_iterator>(words.begin(), words.end());
	}

}}
