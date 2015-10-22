#ifndef REDSTRAIN_MOD_LOCALE_MESSAGEKEYERROR_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGEKEYERROR_HPP

#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API MessageKeyError : public LocalizationError {

	  private:
		const std::string key, file;
		const unsigned line;

	  protected:
		void printMessageKeyErrorLocationAndKey(std::ostream&) const;

	  public:
		MessageKeyError(const std::string&, const std::string&, unsigned);
		MessageKeyError(const MessageKeyError&);
		virtual ~MessageKeyError();

		inline const std::string& getMessageKey() const {
			return key;
		}

		inline const std::string& getFile() const {
			return file;
		}

		inline unsigned getLine() const {
			return line;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(MessageKeyError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGEKEYERROR_HPP */
