#ifndef REDSTRAIN_MOD_TEXT_MAPPINGLOOKUPIOERROR_HPP
#define REDSTRAIN_MOD_TEXT_MAPPINGLOOKUPIOERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "MappingLookupError.hpp"

namespace redengine {
namespace text {

	class MappingLookupIOError : public MappingLookupError {

	  private:
		error::IOError* cause;

	  public:
		MappingLookupIOError(error::IOError*);
		MappingLookupIOError(const MappingLookupIOError&);
		virtual ~MappingLookupIOError();

		inline error::IOError* getCause() const {
			return cause;
		}

		REDSTRAIN_DECLARE_ERROR(MappingLookupIOError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_MAPPINGLOOKUPIOERROR_HPP */
