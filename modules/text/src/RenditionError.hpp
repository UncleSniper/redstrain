#ifndef REDSTRAIN_MOD_TEXT_RENDITIONERROR_HPP
#define REDSTRAIN_MOD_TEXT_RENDITIONERROR_HPP

#include "TextError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API RenditionError : public TextError {

	  public:
		RenditionError();
		RenditionError(const RenditionError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(RenditionError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_RENDITIONERROR_HPP */
