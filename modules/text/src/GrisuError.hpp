#include <redstrain/error/ProgrammingError.hpp>

#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API GrisuError : public error::ProgrammingError {

	  public:
		GrisuError(const std::string&);
		GrisuError(const GrisuError&);

		REDSTRAIN_DECLARE_ERROR(GrisuError)

	};

}}
