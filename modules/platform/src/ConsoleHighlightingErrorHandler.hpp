#ifndef REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGERRORHANDLER_HPP
#define REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGERRORHANDLER_HPP

#include <redstrain/error/HighlightingErrorHandler.hpp>

#include "Console.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConsoleHighlightingErrorHandler : public virtual error::HighlightingErrorHandler {

	  public:
		class REDSTRAIN_PLATFORM_API HighlightingConfiguration {

		  public:
			static const int FL_SET_TYPE_SECTION_FOREGROUND  = 001;
			static const int FL_SET_TYPE_SECTION_BACKGROUND  = 002;
			static const int FL_SET_TYPE_NAME_FOREGROUND     = 004;
			static const int FL_SET_TYPE_NAME_BACKGROUND     = 010;
			static const int FL_SET_ERROR_MESSAGE_FOREGROUND = 020;
			static const int FL_SET_ERROR_MESSAGE_BACKGROUND = 040;
			static const int FL_MASK                         = 077;
			static const int FL_SET_ALL_FOREGROUNDS          = 025;
			static const int FL_SET_ALL_BACKGROUNDS          = 052;
			static const int FL_DEFAULT                      = 017;

		  private:
			int flags;
			Console::Color typeSectionForeground, typeSectionBackground;
			Console::Intensity typeSectionIntensity;
			Console::Color typeNameForeground, typeNameBackground;
			Console::Intensity typeNameIntensity;
			Console::Color errorMessageForeground, errorMessageBackground;
			Console::Intensity errorMessageIntensity;

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

			inline Console::Color getTypeSectionForeground() const {
				return typeSectionForeground;
			}

			inline void setTypeSectionForeground(Console::Color color) {
				typeSectionForeground = color;
			}

			inline Console::Color getTypeSectionBackground() const {
				return typeSectionBackground;
			}

			inline void setTypeSectionBackground(Console::Color color) {
				typeSectionBackground = color;
			}

			inline Console::Intensity getTypeSectionIntensity() const {
				return typeSectionIntensity;
			}

			void setTypeSectionIntensity(Console::Intensity intensity) {
				typeSectionIntensity = intensity;
			}

			inline Console::Color getTypeNameForeground() const {
				return typeNameForeground;
			}

			inline void setTypeNameForeground(Console::Color color) {
				typeNameForeground = color;
			}

			inline Console::Color getTypeNameBackground() const {
				return typeNameBackground;
			}

			inline void setTypeNameBackground(Console::Color color) {
				typeNameBackground = color;
			}

			inline Console::Intensity getTypeNameIntensity() const {
				return typeNameIntensity;
			}

			void setTypeNameIntensity(Console::Intensity intensity) {
				typeNameIntensity = intensity;
			}

			inline Console::Color getErrorMessageForeground() const {
				return errorMessageForeground;
			}

			inline void setErrorMessageForeground(Console::Color color) {
				errorMessageForeground = color;
			}

			inline Console::Color getErrorMessageBackground() const {
				return errorMessageBackground;
			}

			inline void setErrorMessageBackground(Console::Color color) {
				errorMessageBackground = color;
			}

			inline Console::Intensity getErrorMessageIntensity() const {
				return errorMessageIntensity;
			}

			void setErrorMessageIntensity(Console::Intensity intensity) {
				errorMessageIntensity = intensity;
			}

			static const HighlightingConfiguration defaultInstance;

		};

	  private:
		Console* console;
		HighlightingConfiguration configuration;

	  protected:
		virtual void highlightErrorTypeSection();
		virtual void highlightErrorTypeName();
		virtual void highlightErrorMessage();
		virtual void endHighlighting();
		virtual void notifyConsoleChanged();

	  public:
		ConsoleHighlightingErrorHandler(Console*);
		ConsoleHighlightingErrorHandler(Console*, const HighlightingConfiguration&);
		ConsoleHighlightingErrorHandler(const ConsoleHighlightingErrorHandler&);

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

#endif /* REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGERRORHANDLER_HPP */
