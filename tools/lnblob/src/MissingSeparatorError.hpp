#ifndef REDSTRAIN_TOOLS_LNBLOB_MISSINGSEPARATORERROR_HPP
#define REDSTRAIN_TOOLS_LNBLOB_MISSINGSEPARATORERROR_HPP

#include <redstrain/error/Error.hpp>

class MissingSeparatorError : public redengine::error::Error {

  private:
	unsigned lineNumber;

  public:
	MissingSeparatorError(unsigned);
	MissingSeparatorError(const MissingSeparatorError&);

	inline unsigned getLineNumber() const {
		return lineNumber;
	}

	REDSTRAIN_DECLARE_ERROR(MissingSeparatorError)

};

#endif /* REDSTRAIN_TOOLS_LNBLOB_MISSINGSEPARATORERROR_HPP */
