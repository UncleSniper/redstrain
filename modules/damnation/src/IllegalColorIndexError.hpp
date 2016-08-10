#ifndef REDSTRAIN_MOD_DAMNATION_ILLEGALCOLORINDEXERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_ILLEGALCOLORINDEXERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "DamnError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API IllegalColorIndexError : public error::IllegalArgumentError, public DamnError {

	  private:
		const unsigned index;

	  public:
		IllegalColorIndexError(unsigned);
		IllegalColorIndexError(const IllegalColorIndexError&);

		inline unsigned getColorIndex() const {
			return index;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalColorIndexError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_ILLEGALCOLORINDEXERROR_HPP */
