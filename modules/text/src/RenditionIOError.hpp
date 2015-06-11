#ifndef REDSTRAIN_MOD_TEXT_RENDITIONIOERROR_HPP
#define REDSTRAIN_MOD_TEXT_RENDITIONIOERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "RenditionError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API RenditionIOError : public error::IOError {

	  private:
		RenditionError* cause;

	  public:
		RenditionIOError(RenditionError*);
		RenditionIOError(const RenditionIOError&);
		virtual ~RenditionIOError();

		inline RenditionError* getCause() const {
			return cause;
		}

		REDSTRAIN_DECLARE_ERROR(RenditionIOError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_RENDITIONIOERROR_HPP */
