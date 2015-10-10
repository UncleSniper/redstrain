#ifndef REDSTRAIN_TOOLS_LNBLOB_MISSINGSEPARATORERROR_HPP
#define REDSTRAIN_TOOLS_LNBLOB_MISSINGSEPARATORERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MissingInputSeparatorError : public redengine::error::Error {

	  private:
		const std::string separator, file;
		unsigned lineNumber;

	  public:
		MissingInputSeparatorError(const std::string&, const std::string&, unsigned);
		MissingInputSeparatorError(const MissingInputSeparatorError&);

		inline const std::string& getSeparator() const {
			return separator;
		}

		inline const std::string& getFile() const {
			return file;
		}

		inline unsigned getLineNumber() const {
			return lineNumber;
		}

		REDSTRAIN_DECLARE_ERROR(MissingInputSeparatorError)

	};

}}

#endif /* REDSTRAIN_TOOLS_LNBLOB_MISSINGSEPARATORERROR_HPP */
