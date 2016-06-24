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
			const HighlightingConfiguration& configuration) : console(console), configuration(configuration) {}

	ConsoleHighlightingSymbolSink::ConsoleHighlightingSymbolSink(const ConsoleHighlightingSymbolSink& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), console(sink.console),
			configuration(sink.configuration) {}

	/*TODO
	void ConsoleHighlightingSymbolSink::highlightCVQualifier();
	void ConsoleHighlightingSymbolSink::highlightKeyword();
	void ConsoleHighlightingSymbolSink::highlightTypeModifier();
	void ConsoleHighlightingSymbolSink::highlightSymbolicConstant();
	void ConsoleHighlightingSymbolSink::highlightNumericLiteral();
	void ConsoleHighlightingSymbolSink::highlightDiscriminator();
	void ConsoleHighlightingSymbolSink::highlightUndefinedTemplateParameter();
	void ConsoleHighlightingSymbolSink::highlightBuiltinType();
	void ConsoleHighlightingSymbolSink::endHighlighting();
	*/

}}
