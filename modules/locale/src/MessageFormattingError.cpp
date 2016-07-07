#include "MessageFormattingError.hpp"

using std::string;
using redengine::util::MemorySize;
using redengine::text::FormattingError;

namespace redengine {
namespace locale {

	MessageFormattingError::MessageFormattingError(FormattingError* formattingError, const Locale& locale,
			MemorySize messageID, const string& componentName) :
			formattingError(formattingError), locale(locale), messageID(messageID), componentName(componentName) {
		if(formattingError)
			formattingError->ref();
	}

	MessageFormattingError::MessageFormattingError(const MessageFormattingError& error)
			: Error(error), LocalizationError(error), formattingError(error.formattingError), locale(error.locale),
			messageID(error.messageID), componentName(error.componentName) {
		if(formattingError)
			formattingError->ref();
	}

	MessageFormattingError::~MessageFormattingError() {
		if(formattingError)
			formattingError->unref();
	}

	REDSTRAIN_DEFINE_ERROR(MessageFormattingError) {
		out << "Message " << messageID << " for locale '" << locale.getName() << '\'';
		if(!componentName.empty())
			out << " in component '" << componentName << '\'';
		out << " contains a formatting error";
		if(formattingError) {
			out << ": ";
			formattingError->printMessage(out);
		}
	}

}}
