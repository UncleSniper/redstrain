#include "ConsoleHighlightingErrorHandler.hpp"

namespace redengine {
namespace platform {

	// ======== HighlightingConfiguration ========

	const ConsoleHighlightingErrorHandler::HighlightingConfiguration
			ConsoleHighlightingErrorHandler::HighlightingConfiguration::defaultInstance;

	ConsoleHighlightingErrorHandler::HighlightingConfiguration::HighlightingConfiguration()
			: flags(HighlightingConfiguration::FL_DEFAULT),
			typeSectionForeground(Console::YELLOW), typeSectionBackground(Console::BLACK),
			typeSectionIntensity(Console::BRIGHT),
			typeNameForeground(Console::RED), typeNameBackground(Console::BLACK), typeNameIntensity(Console::BRIGHT),
			errorMessageForeground(Console::WHITE), errorMessageBackground(Console::BLACK),
			errorMessageIntensity(Console::BRIGHT) {}

	ConsoleHighlightingErrorHandler::HighlightingConfiguration::HighlightingConfiguration(const
			HighlightingConfiguration& configuration)
			: flags(configuration.flags),
			typeSectionForeground(configuration.typeSectionForeground),
			typeSectionBackground(configuration.typeSectionBackground),
			typeSectionIntensity(configuration.typeSectionIntensity),
			typeNameForeground(configuration.typeNameForeground),
			typeNameBackground(configuration.typeNameBackground),
			typeNameIntensity(configuration.typeNameIntensity),
			errorMessageForeground(configuration.errorMessageForeground),
			errorMessageBackground(configuration.errorMessageBackground),
			errorMessageIntensity(configuration.errorMessageIntensity) {}

	// ======== ConsoleHighlightingErrorHandler ========

	ConsoleHighlightingErrorHandler::ConsoleHighlightingErrorHandler(Console* console) : console(console) {}

	ConsoleHighlightingErrorHandler::ConsoleHighlightingErrorHandler(Console* console,
			const HighlightingConfiguration& configuration) : console(console), configuration(configuration) {}

	ConsoleHighlightingErrorHandler::ConsoleHighlightingErrorHandler(const ConsoleHighlightingErrorHandler& handler)
			: ErrorHandler(handler), HighlightingErrorHandler(handler),
			console(handler.console), configuration(handler.configuration) {}

	void ConsoleHighlightingErrorHandler::setHighlightingConsole(Console* console) {
		this->console = console;
		notifyConsoleChanged();
	}

	void ConsoleHighlightingErrorHandler::highlightErrorTypeSection() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_TYPE_SECTION_FOREGROUND)) {
			console->setForeground(configuration.getTypeSectionForeground());
			console->setIntensity(configuration.getTypeSectionIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_TYPE_SECTION_BACKGROUND))
			console->setBackground(configuration.getTypeSectionBackground());
	}

	void ConsoleHighlightingErrorHandler::highlightErrorTypeName() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_TYPE_NAME_FOREGROUND)) {
			console->setForeground(configuration.getTypeNameForeground());
			console->setIntensity(configuration.getTypeNameIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_TYPE_NAME_BACKGROUND))
			console->setBackground(configuration.getTypeNameBackground());
	}

	void ConsoleHighlightingErrorHandler::highlightErrorMessage() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_ERROR_MESSAGE_FOREGROUND)) {
			console->setForeground(configuration.getErrorMessageForeground());
			console->setIntensity(configuration.getErrorMessageIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_ERROR_MESSAGE_BACKGROUND))
			console->setBackground(configuration.getErrorMessageBackground());
	}

	void ConsoleHighlightingErrorHandler::endHighlighting() {
		if(console)
			console->resetAttributes();
	}

	void ConsoleHighlightingErrorHandler::notifyConsoleChanged() {}

}}
