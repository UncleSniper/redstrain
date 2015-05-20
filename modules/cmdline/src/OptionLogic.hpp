#ifndef REDSTRAIN_MOD_CMDLINE_OPTIONLOGIC_HPP
#define REDSTRAIN_MOD_CMDLINE_OPTIONLOGIC_HPP

#include <map>
#include <string>

#include "api.hpp"

namespace redengine {
namespace cmdline {

	class WordAction;

	class REDSTRAIN_CMDLINE_API OptionLogic {

	  public:
		enum Flags {
			SHORT_OPTIONS       = 0001,
			DOUBLE_LONG_OPTIONS = 0002,
			SINGLE_LONG_OPTIONS = 0004,
			WORD_SEPARATORS     = 0010,
			SYMBOL_SEPARATORS   = 0020,
			INLINE_ARGUMENTS    = 0040,
			OPTION_TERMINATOR   = 0100,
			BAREWORD_TERMINATES = 0200
		};

		enum Arity {
			NO_ARGUMENT,
			REQUIRED_ARGUMENT,
			OPTIONAL_ARGUMENT
		};

		enum InlineLongConflictPolicy {
			LONG_OVER_INLINE,
			INLINE_OVER_LONG,
			INLINE_LONG_CONFLICT_IS_ERROR
		};

		class REDSTRAIN_CMDLINE_API Option {

		  private:
			Arity arity;
			WordAction* action;
			bool managed;

		  public:
			Option();
			Option(Arity, WordAction*, bool);
			Option(const Option&);
			~Option();

			inline Arity getArity() const {
				return arity;
			}

			inline WordAction* getAction() const {
				return action;
			}

			inline bool isManaged() const {
				return managed;
			}

			Option& operator=(const Option&);
			void assign(Arity, WordAction*, bool);

		};

	  public:
		static const int DEFAULT_FLAGS
				= SHORT_OPTIONS | DOUBLE_LONG_OPTIONS
				| WORD_SEPARATORS | SYMBOL_SEPARATORS
				| INLINE_ARGUMENTS | OPTION_TERMINATOR;

	  private:
		typedef std::map<char, Option> ShortOptions;
		typedef ShortOptions::const_iterator ShortOptionIterator;
		typedef std::map<std::string, Option> LongOptions;
		typedef LongOptions::const_iterator LongOptionIterator;

	  private:
		int flags;
		char initiatorSymbol, separatorSymbol;
		std::string optionTerminator;
		InlineLongConflictPolicy inlineLongConflict;
		ShortOptions shortOptions;
		LongOptions longOptions;

	  public:
		OptionLogic(int = DEFAULT_FLAGS);
		OptionLogic(const OptionLogic&);
		virtual ~OptionLogic() {}

		inline int getFlags() const {
			return flags;
		}

		inline void setFlags(int newFlags) {
			flags = newFlags;
		}

		inline void setFlags(Flags newFlags) {
			flags = newFlags;
		}

		inline void enableFlags(int mask) {
			flags |= mask;
		}

		void enableFlags(Flags);

		inline void disableFlags(int mask) {
			flags &= ~mask;
		}

		void disableFlags(Flags);

		inline bool hasFlags(int mask) const {
			return (flags & mask) == mask;
		}

		bool hasFlags(Flags) const;

		inline char getInitiatorSymbol() const {
			return initiatorSymbol;
		}

		inline void setInitiatorSymbol(char symbol) {
			initiatorSymbol = symbol;
		}

		inline char getSeparatorSymbol() const {
			return separatorSymbol;
		}

		inline void setSeparatorSymbol(char symbol) {
			separatorSymbol = symbol;
		}

		inline const std::string& getOptionTerminator() const {
			return optionTerminator;
		}

		inline void setOptionTerminator(const std::string& terminator) {
			optionTerminator = terminator;
		}

		inline InlineLongConflictPolicy getInlineLongConflictPolicy() const {
			return inlineLongConflict;
		}

		inline void setInlineLongConflictPolicy(InlineLongConflictPolicy policy) {
			inlineLongConflict = policy;
		}

		void addShortOption(char, WordAction*, Arity = NO_ARGUMENT, bool = true);
		void addLongOption(const std::string&, WordAction*, Arity = NO_ARGUMENT, bool = true);
		const Option* getShortOption(char) const;
		const Option* getLongOption(const std::string&) const;

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_OPTIONLOGIC_HPP */
