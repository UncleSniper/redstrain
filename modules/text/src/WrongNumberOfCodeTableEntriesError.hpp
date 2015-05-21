#ifndef REDSTRAIN_MOD_TEXT_WRONGNUMBEROFCODETABLENTRIESERROR_HPP
#define REDSTRAIN_MOD_TEXT_WRONGNUMBEROFCODETABLENTRIESERROR_HPP

#include "CodeTableDefinitionError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API WrongNumberOfCodeTableEntriesError : public CodeTableDefinitionError {

	  public:
		WrongNumberOfCodeTableEntriesError();
		WrongNumberOfCodeTableEntriesError(const WrongNumberOfCodeTableEntriesError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(WrongNumberOfCodeTableEntriesError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_WRONGNUMBEROFCODETABLENTRIESERROR_HPP */
