#ifndef REDSTRAIN_MOD_TEXT_CODETABLEDEFINITIONERROR_HPP
#define REDSTRAIN_MOD_TEXT_CODETABLEDEFINITIONERROR_HPP

#include "TextError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API CodeTableDefinitionError : public TextError {

	  public:
		CodeTableDefinitionError();
		CodeTableDefinitionError(const CodeTableDefinitionError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(CodeTableDefinitionError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CODETABLEDEFINITIONERROR_HPP */
