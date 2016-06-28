#ifndef REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGSTACKTRACESINK_HPP

#include <redstrain/error/HighlightingStackTraceSink.hpp>

#include "Console.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConsoleHighlightingStackTraceSink
			: public virtual error::HighlightingStackTraceSink {

	  public:
		class REDSTRAIN_PLATFORM_API HighlightingConfiguration {

		  public:
			static const int FL_SET_NO_TRACE_AVAILABLE_FOREGROUND        = 000001;
			static const int FL_SET_NO_TRACE_AVAILABLE_BACKGROUND        = 000002;
			static const int FL_SET_NO_FRAMES_ON_STACK_FOREGROUND        = 000004;
			static const int FL_SET_NO_FRAMES_ON_STACK_BACKGROUND        = 000010;
			static const int FL_SET_BOTTOM_OF_UNWINDING_FOREGROUND       = 000020;
			static const int FL_SET_BOTTOM_OF_UNWINDING_BACKGROUND       = 000040;
			static const int FL_SET_FRAME_NOT_RECONSTRUCTIBLE_FOREGROUND = 000100;
			static const int FL_SET_FRAME_NOT_RECONSTRUCTIBLE_BACKGROUND = 000200;
			static const int FL_SET_MEMORY_ADDRESS_FOREGROUND            = 000400;
			static const int FL_SET_MEMORY_ADDRESS_BACKGROUND            = 001000;
			static const int FL_SET_UNRESOLVED_SYMBOL_FOREGROUND         = 002000;
			static const int FL_SET_UNRESOLVED_SYMBOL_BACKGROUND         = 004000;
			static const int FL_SET_MODULE_NAME_FOREGROUND               = 010000;
			static const int FL_SET_MODULE_NAME_BACKGROUND               = 020000;
			static const int FL_MASK                                     = 037777;
			static const int FL_SET_ALL_FOREGROUNDS                      = 012525;
			static const int FL_SET_ALL_BACKGROUNDS                      = 025252;
			static const int FL_DEFAULT                                  = 031717;

		  private:
			int flags;
			Console::Color noTraceForeground, noTraceBackground;
			Console::Intensity noTraceIntensity;
			Console::Color noFramesForeground, noFramesBackground;
			Console::Intensity noFramesIntensity;
			Console::Color bottomForeground, bottomBackground;
			Console::Intensity bottomIntensity;
			Console::Color unreconstructibleForeground, unreconstructibleBackground;
			Console::Intensity unreconstructibleIntensity;
			Console::Color addressForeground, addressBackground;
			Console::Intensity addressIntensity;
			Console::Color unresolvedForeground, unresolvedBackground;
			Console::Intensity unresolvedIntensity;
			Console::Color moduleForeground, moduleBackground;
			Console::Intensity moduleIntensity;

		  public:
			HighlightingConfiguration();
			HighlightingConfiguration(const HighlightingConfiguration&);

			inline int getFlags() const {
				return flags;
			}

			inline bool hasFlags(int mask) const {
				return (flags & mask) == mask;
			}

			inline void setFlags(int flags) {
				this->flags = flags & HighlightingConfiguration::FL_MASK;
			}

			inline Console::Color getNoTraceAvailableForeground() const {
				return noTraceForeground;
			}

			inline void setNoTraceAvailableForeground(Console::Color color) {
				noTraceForeground = color;
			}

			inline Console::Color getNoTraceAvailableBackground() const {
				return noTraceBackground;
			}

			inline void setNoTraceAvailableBackground(Console::Color color) {
				noTraceBackground = color;
			}

			inline Console::Intensity getNoTraceAvailableItensity() const {
				return noTraceIntensity;
			}

			inline void setNoTraceAvailableItensity(Console::Intensity intensity) {
				noTraceIntensity = intensity;
			}

			inline Console::Color getNoFramesOnStackForeground() const {
				return noFramesForeground;
			}

			inline void setNoFramesOnStackForeground(Console::Color color) {
				noFramesForeground = color;
			}

			inline Console::Color getNoFramesOnStackBackground() const {
				return noFramesBackground;
			}

			inline void setNoFramesOnStackBackground(Console::Color color) {
				noFramesBackground = color;
			}

			inline Console::Intensity getNoFramesOnStackItensity() const {
				return noFramesIntensity;
			}

			inline void setNoFramesOnStackItensity(Console::Intensity intensity) {
				noFramesIntensity = intensity;
			}

			inline Console::Color getBottomOfUnwindingForeground() const {
				return bottomForeground;
			}

			inline void setBottomOfUnwindingForeground(Console::Color color) {
				bottomForeground = color;
			}

			inline Console::Color getBottomOfUnwindingBackground() const {
				return bottomBackground;
			}

			inline void setBottomOfUnwindingBackground(Console::Color color) {
				bottomBackground = color;
			}

			inline Console::Intensity getBottomOfUnwindingItensity() const {
				return bottomIntensity;
			}

			inline void setBottomOfUnwindingItensity(Console::Intensity intensity) {
				bottomIntensity = intensity;
			}

			inline Console::Color getFrameNotReconstructibleForeground() const {
				return unreconstructibleForeground;
			}

			inline void setFrameNotReconstructibleForeground(Console::Color color) {
				unreconstructibleForeground = color;
			}

			inline Console::Color getFrameNotReconstructibleBackground() const {
				return unreconstructibleBackground;
			}

			inline void setFrameNotReconstructibleBackground(Console::Color color) {
				unreconstructibleBackground = color;
			}

			inline Console::Intensity getFrameNotReconstructibleItensity() const {
				return unreconstructibleIntensity;
			}

			inline void setFrameNotReconstructibleItensity(Console::Intensity intensity) {
				unreconstructibleIntensity = intensity;
			}

			inline Console::Color getMemoryAddressForeground() const {
				return addressForeground;
			}

			inline void setMemoryAddressForeground(Console::Color color) {
				addressForeground = color;
			}

			inline Console::Color getMemoryAddressBackground() const {
				return addressBackground;
			}

			inline void setMemoryAddressBackground(Console::Color color) {
				addressBackground = color;
			}

			inline Console::Intensity getMemoryAddressItensity() const {
				return addressIntensity;
			}

			inline void setMemoryAddressItensity(Console::Intensity intensity) {
				addressIntensity = intensity;
			}

			inline Console::Color getUnresolvedSymbolForeground() const {
				return unresolvedForeground;
			}

			inline void setUnresolvedSymbolForeground(Console::Color color) {
				unresolvedForeground = color;
			}

			inline Console::Color getUnresolvedSymbolBackground() const {
				return unresolvedBackground;
			}

			inline void setUnresolvedSymbolBackground(Console::Color color) {
				unresolvedBackground = color;
			}

			inline Console::Intensity getUnresolvedSymbolItensity() const {
				return unresolvedIntensity;
			}

			inline void setUnresolvedSymbolItensity(Console::Intensity intensity) {
				unresolvedIntensity = intensity;
			}

			inline Console::Color getModuleNameForeground() const {
				return moduleForeground;
			}

			inline void setModuleNameForeground(Console::Color color) {
				moduleForeground = color;
			}

			inline Console::Color getModuleNameBackground() const {
				return moduleBackground;
			}

			inline void setModuleNameBackground(Console::Color color) {
				moduleBackground = color;
			}

			inline Console::Intensity getModuleNameItensity() const {
				return moduleIntensity;
			}

			inline void setModuleNameItensity(Console::Intensity intensity) {
				moduleIntensity = intensity;
			}

			static const HighlightingConfiguration defaultInstance;

		};

	  private:
		Console* console;
		HighlightingConfiguration configuration;

	  protected:
		virtual void highlightNoTraceAvailable();
		virtual void highlightNoFramesOnStack();
		virtual void highlightBottomOfUnwinding();
		virtual void highlightFrameNotReconstructible();
		virtual void highlightMemoryAddress();
		virtual void highlightUnresolvedSymbol();
		virtual void highlightModuleName();
		virtual void endHighlighting();
		virtual void notifyConsoleChanged();

	  public:
		ConsoleHighlightingStackTraceSink(Console*);
		ConsoleHighlightingStackTraceSink(Console*, const HighlightingConfiguration&);
		ConsoleHighlightingStackTraceSink(const ConsoleHighlightingStackTraceSink&);

		inline Console* getConsole() const {
			return console;
		}

		void setConsole(Console*);

		inline HighlightingConfiguration& getHighlightingConfiguration() {
			return configuration;
		}

		inline const HighlightingConfiguration& getHighlightingConfiguration() const {
			return configuration;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGSTACKTRACESINK_HPP */
