#ifndef REDSTRAIN_MOD_LOCALE_DUPLICATEMESSAGEKEYERROR_HPP
#define REDSTRAIN_MOD_LOCALE_DUPLICATEMESSAGEKEYERROR_HPP

#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API DuplicateMessageKeyError : public LocalizationError {

	  private:
		const std::string key, file;
		const unsigned line;

	  public:
		DuplicateMessageKeyError(const std::string&, const std::string&, unsigned);
		DuplicateMessageKeyError(const DuplicateMessageKeyError&);
		virtual ~DuplicateMessageKeyError();

		inline const std::string& getDuplicateKey() const {
			return key;
		}

		inline const std::string& getFile() const {
			return file;
		}

		inline unsigned getLine() const {
			return line;
		}

		REDSTRAIN_DECLARE_ERROR(DuplicateMessageKeyError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_DUPLICATEMESSAGEKEYERROR_HPP */
