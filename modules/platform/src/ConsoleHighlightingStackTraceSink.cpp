#include "ConsoleHighlightingStackTraceSink.hpp"

namespace redengine {
namespace platform {

	// ======== HighlightingConfiguration ========

	const ConsoleHighlightingStackTraceSink::HighlightingConfiguration
			ConsoleHighlightingStackTraceSink::HighlightingConfiguration::defaultInstance;

	ConsoleHighlightingStackTraceSink::HighlightingConfiguration::HighlightingConfiguration()
			: flags(HighlightingConfiguration::FL_DEFAULT),
			noTraceForeground(Console::YELLOW), noTraceBackground(Console::BLACK), noTraceIntensity(Console::BRIGHT),
			noFramesForeground(Console::YELLOW), noFramesBackground(Console::BLACK),
			noFramesIntensity(Console::BRIGHT),
			bottomForeground(Console::WHITE), bottomBackground(Console::BLACK), bottomIntensity(Console::NORMAL),
			unreconstructibleForeground(Console::YELLOW), unreconstructibleBackground(Console::BLACK),
			unreconstructibleIntensity(Console::BRIGHT),
			addressForeground(Console::MAGENTA), addressBackground(Console::BLACK), addressIntensity(Console::NORMAL),
			unresolvedForeground(Console::WHITE), unresolvedBackground(Console::BLACK),
			unresolvedIntensity(Console::NORMAL),
			moduleForeground(Console::WHITE), moduleBackground(Console::BLACK), moduleIntensity(Console::BRIGHT) {}

	ConsoleHighlightingStackTraceSink::HighlightingConfiguration::HighlightingConfiguration(const
			HighlightingConfiguration& configuration)
			: flags(configuration.flags),
			noTraceForeground(configuration.noTraceForeground), noTraceBackground(configuration.noTraceBackground),
			noTraceIntensity(configuration.noTraceIntensity),
			noFramesForeground(configuration.noFramesForeground),
			noFramesBackground(configuration.noFramesBackground),
			noFramesIntensity(configuration.noFramesIntensity),
			bottomForeground(configuration.bottomForeground), bottomBackground(configuration.bottomBackground),
			bottomIntensity(configuration.bottomIntensity),
			unreconstructibleForeground(configuration.unreconstructibleForeground),
			unreconstructibleBackground(configuration.unreconstructibleBackground),
			unreconstructibleIntensity(configuration.unreconstructibleIntensity),
			addressForeground(configuration.addressForeground), addressBackground(configuration.addressBackground),
			addressIntensity(configuration.addressIntensity),
			unresolvedForeground(configuration.unresolvedForeground),
			unresolvedBackground(configuration.unresolvedBackground),
			unresolvedIntensity(configuration.unresolvedIntensity),
			moduleForeground(configuration.moduleForeground), moduleBackground(configuration.moduleBackground),
			moduleIntensity(configuration.moduleIntensity) {}

	// ======== ConsoleHighlightingStackTraceSink ========

	ConsoleHighlightingStackTraceSink::ConsoleHighlightingStackTraceSink(Console* console) : console(console) {
		notifyConsoleChanged();
	}

	ConsoleHighlightingStackTraceSink::ConsoleHighlightingStackTraceSink(Console* console,
			const HighlightingConfiguration& configuration) : console(console), configuration(configuration) {
		notifyConsoleChanged();
	}

	ConsoleHighlightingStackTraceSink::ConsoleHighlightingStackTraceSink(const
			ConsoleHighlightingStackTraceSink& sink)
			: StackTraceSink(sink), HighlightingStackTraceSink(sink),
			console(sink.console), configuration(sink.configuration) {
		notifyConsoleChanged();
	}

	void ConsoleHighlightingStackTraceSink::setConsole(Console* console) {
		this->console = console;
		notifyConsoleChanged();
	}

	void ConsoleHighlightingStackTraceSink::highlightNoTraceAvailable() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_NO_TRACE_AVAILABLE_FOREGROUND)) {
			console->setForeground(configuration.getNoTraceAvailableForeground());
			console->setIntensity(configuration.getNoTraceAvailableItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_NO_TRACE_AVAILABLE_BACKGROUND))
			console->setBackground(configuration.getNoTraceAvailableBackground());
	}

	void ConsoleHighlightingStackTraceSink::highlightNoFramesOnStack() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_NO_FRAMES_ON_STACK_FOREGROUND)) {
			console->setForeground(configuration.getNoFramesOnStackForeground());
			console->setIntensity(configuration.getNoFramesOnStackItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_NO_FRAMES_ON_STACK_BACKGROUND))
			console->setBackground(configuration.getNoFramesOnStackBackground());
	}

	void ConsoleHighlightingStackTraceSink::highlightBottomOfUnwinding() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_BOTTOM_OF_UNWINDING_FOREGROUND)) {
			console->setForeground(configuration.getBottomOfUnwindingForeground());
			console->setIntensity(configuration.getBottomOfUnwindingItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_BOTTOM_OF_UNWINDING_BACKGROUND))
			console->setBackground(configuration.getBottomOfUnwindingBackground());
	}

	void ConsoleHighlightingStackTraceSink::highlightFrameNotReconstructible() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_FRAME_NOT_RECONSTRUCTIBLE_FOREGROUND)) {
			console->setForeground(configuration.getFrameNotReconstructibleForeground());
			console->setIntensity(configuration.getFrameNotReconstructibleItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_FRAME_NOT_RECONSTRUCTIBLE_BACKGROUND))
			console->setBackground(configuration.getFrameNotReconstructibleBackground());
	}

	void ConsoleHighlightingStackTraceSink::highlightMemoryAddress() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_MEMORY_ADDRESS_FOREGROUND)) {
			console->setForeground(configuration.getMemoryAddressForeground());
			console->setIntensity(configuration.getMemoryAddressItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_MEMORY_ADDRESS_BACKGROUND))
			console->setBackground(configuration.getMemoryAddressBackground());
	}

	void ConsoleHighlightingStackTraceSink::highlightUnresolvedSymbol() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_UNRESOLVED_SYMBOL_FOREGROUND)) {
			console->setForeground(configuration.getUnresolvedSymbolForeground());
			console->setIntensity(configuration.getUnresolvedSymbolItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_UNRESOLVED_SYMBOL_BACKGROUND))
			console->setBackground(configuration.getUnresolvedSymbolBackground());
	}

	void ConsoleHighlightingStackTraceSink::highlightModuleName() {
		if(!console)
			return;
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_MODULE_NAME_FOREGROUND)) {
			console->setForeground(configuration.getModuleNameForeground());
			console->setIntensity(configuration.getModuleNameItensity());
		}
		if(configuration.hasFlags(HighlightingConfiguration::FL_SET_MODULE_NAME_BACKGROUND))
			console->setBackground(configuration.getModuleNameBackground());
	}

	void ConsoleHighlightingStackTraceSink::endHighlighting() {
		if(console)
			console->resetAttributes();
	}

	void ConsoleHighlightingStackTraceSink::notifyConsoleChanged() {}

}}
