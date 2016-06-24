#include "ConsoleHighlightingSymbolSink.hpp"

namespace redengine {
namespace platform {

	// ======== HighlightingConfiguration ========

	const ConsoleHighlightingSymbolSink::HighlightingConfiguration
			ConsoleHighlightingSymbolSink::HighlightingConfiguration::HighlightingConfiguration::defaultInstance;

	ConsoleHighlightingSymbolSink::HighlightingConfiguration::HighlightingConfiguration()
			: flags(HighlightingConfiguration::FL_DEFAULT),
			cvForeground(Console::GREEN), cvBackground(Console::BLACK), cvIntensity(Console::NORMAL),
			keywordForeground(Console::YELLOW), keywordBackground(Console::BLACK), keywordIntensity(Console::BRIGHT),
			typeModifierForeground(Console::Console::GREEN), typeModifierBackground(Console::BLACK),
			typeModifierIntensity(Console::NORMAL),
			constantForeground(Console::CYAN), constantBackground(Console::BLACK), constantIntensity(Console::NORMAL),
			numberForeground(Console::CYAN), numberBackground(Console::BLACK), numberIntensity(Console::NORMAL),
			discriminatorForeground(Console::BLUE), discriminatorBackground(Console::BLUE),
			discriminatorIntensity(Console::BRIGHT),
			tparamForeground(Console::CYAN), tparamBackground(Console::BLACK), tparamIntensity(Console::BRIGHT),
			typeForeground(Console::GREEN), typeBackground(Console::BLACK), typeIntensity(Console::NORMAL) {}

	ConsoleHighlightingSymbolSink::HighlightingConfiguration::HighlightingConfiguration(const
			HighlightingConfiguration& configuration)
			: flags(configuration.flags),
			cvForeground(configuration.cvForeground), cvBackground(configuration.cvBackground),
			cvIntensity(configuration.cvIntensity),
			keywordForeground(configuration.keywordForeground), keywordBackground(configuration.keywordBackground),
			keywordIntensity(configuration.keywordIntensity),
			typeModifierForeground(configuration.typeModifierForeground),
			typeModifierBackground(configuration.typeModifierBackground),
			typeModifierIntensity(configuration.typeModifierIntensity),
			constantForeground(configuration.constantForeground),
			constantBackground(configuration.constantBackground),
			constantIntensity(configuration.constantIntensity),
			numberForeground(configuration.numberForeground), numberBackground(configuration.numberBackground),
			numberIntensity(configuration.numberIntensity),
			discriminatorForeground(configuration.discriminatorForeground),
			discriminatorBackground(configuration.discriminatorBackground),
			discriminatorIntensity(configuration.discriminatorIntensity),
			tparamForeground(configuration.tparamForeground), tparamBackground(configuration.tparamBackground),
			tparamIntensity(configuration.tparamIntensity),
			typeForeground(configuration.typeForeground), typeBackground(configuration.typeBackground),
			typeIntensity(configuration.typeIntensity) {}

	// ======== ConsoleHighlightingSymbolSink ========

	ConsoleHighlightingSymbolSink::ConsoleHighlightingSymbolSink(Console* console) : console(console) {}

	ConsoleHighlightingSymbolSink::ConsoleHighlightingSymbolSink(Console* console,
			const HighlightingConfiguration& configuration) : console(console), configuration(configuration) {
		notifyConsoleChanged();
	}

	ConsoleHighlightingSymbolSink::ConsoleHighlightingSymbolSink(const ConsoleHighlightingSymbolSink& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), console(sink.console),
			configuration(sink.configuration) {
		notifyConsoleChanged();
	}

	void ConsoleHighlightingSymbolSink::setConsole(Console* console) {
		this->console = console;
		notifyConsoleChanged();
	}

	void ConsoleHighlightingSymbolSink::highlightCVQualifier() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_CV_QUALIFIER_FOREGROUND)) {
			console->setForeground(configuration.getCVQualifierForeground());
			console->setIntensity(configuration.getCVQualifierIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_CV_QUALIFIER_BACKGROUND))
			console->setBackground(configuration.getCVQualifierBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightKeyword() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_KEYWORD_FOREGROUND)) {
			console->setForeground(configuration.getKeywordForeground());
			console->setIntensity(configuration.getKeywordIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_KEYWORD_BACKGROUND))
			console->setBackground(configuration.getKeywordBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightTypeModifier() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_TYPE_MODIFIER_FOREGROUND)) {
			console->setForeground(configuration.getTypeModifierForeground());
			console->setIntensity(configuration.getTypeModifierIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_TYPE_MODIFIER_BACKGROUND))
			console->setBackground(configuration.getTypeModifierBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightSymbolicConstant() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_SYMBOLIC_CONSTANT_FOREGROUND)) {
			console->setForeground(configuration.getSymbolicConstantForeground());
			console->setIntensity(configuration.getSymbolicConstantIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_SYMBOLIC_CONSTANT_BACKGROUND))
			console->setBackground(configuration.getSymbolicConstantBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightNumericLiteral() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_NUMERIC_LITERAL_FOREGROUND)) {
			console->setForeground(configuration.getNumericLiteralForeground());
			console->setIntensity(configuration.getNumericLiteralIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_NUMERIC_LITERAL_BACKGROUND))
			console->setBackground(configuration.getNumericLiteralBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightDiscriminator() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_DISCRIMINATOR_FOREGROUND)) {
			console->setForeground(configuration.getDiscriminatorBackground());
			console->setIntensity(configuration.getDiscriminatorIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_DISCRIMINATOR_BACKGROUND))
			console->setBackground(configuration.getDiscriminatorBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightUndefinedTemplateParameter() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_UNDEFINED_TEMPLATE_PARAMETER_FOREGROUND)) {
			console->setForeground(configuration.getUndefinedTemplateParameterForeground());
			console->setIntensity(configuration.getUndefinedTemplateParameterIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_UNDEFINED_TEMPLATE_PARAMETER_BACKGROUND))
			console->setBackground(configuration.getUndefinedTemplateParameterBackground());
	}

	void ConsoleHighlightingSymbolSink::highlightBuiltinType() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_BUILTIN_TYPE_FOREGROUND)) {
			console->setForeground(configuration.getBuiltinTypeForeground());
			console->setIntensity(configuration.getBuiltinTypeIntensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_BUILTIN_TYPE_BACKGROUND))
			console->setBackground(configuration.getBuiltinTypeBackground());
	}

	void ConsoleHighlightingSymbolSink::endHighlighting() {
		if(console)
			console->resetAttributes();
	}

	void ConsoleHighlightingSymbolSink::notifyConsoleChanged() {}

}}
