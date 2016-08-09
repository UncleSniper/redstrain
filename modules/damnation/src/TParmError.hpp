#ifndef REDSTRAIN_MOD_DAMNATION_TPARMERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TPARMERROR_HPP

#include "DamnError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API TParmError : public DamnError {

	  public:
		TParmError();
		TParmError(const TParmError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(TParmError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_TPARMERROR_HPP */
