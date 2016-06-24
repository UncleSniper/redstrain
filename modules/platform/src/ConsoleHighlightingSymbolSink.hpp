#ifndef REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGSYMBOLSINK_HPP
#define REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGSYMBOLSINK_HPP

#include <redstrain/redmond/unmangle/HighlightingSymbolSink.hpp>

#include "Console.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConsoleHighlightingSymbolSink
			: public virtual redmond::unmangle::HighlightingSymbolSink {

	  public:
		class REDSTRAIN_PLATFORM_API HighlightingConfiguration {

		  public:
			static const int FL_SET_CV_QUALIFIER_FOREGROUND                 = 0000001;
			static const int FL_SET_CV_QUALIFIER_BACKGROUND                 = 0000002;
			static const int FL_SET_KEYWORD_FOREGROUND                      = 0000004;
			static const int FL_SET_KEYWORD_BACKGROUND                      = 0000010;
			static const int FL_SET_TYPE_MODIFIER_FOREGROUND                = 0000020;
			static const int FL_SET_TYPE_MODIFIER_BACKGROUND                = 0000040;
			static const int FL_SET_SYMBOLIC_CONSTANT_FOREGROUND            = 0000100;
			static const int FL_SET_SYMBOLIC_CONSTANT_BACKGROUND            = 0000200;
			static const int FL_SET_NUMERIC_LITERAL_FOREGROUND              = 0000400;
			static const int FL_SET_NUMERIC_LITERAL_BACKGROUND              = 0001000;
			static const int FL_SET_DISCRIMINATOR_FOREGROUND                = 0002000;
			static const int FL_SET_DISCRIMINATOR_BACKGROUND                = 0004000;
			static const int FL_SET_UNDEFINED_TEMPLATE_PARAMETER_FOREGROUND = 0010000;
			static const int FL_SET_UNDEFINED_TEMPLATE_PARAMETER_BACKGROUND = 0020000;
			static const int FL_SET_BUILTIN_TYPE_FOREGROUND                 = 0040000;
			static const int FL_SET_BUILTIN_TYPE_BACKGROUND                 = 0100000;
			static const int FL_MASK                                        = 0177777;
			static const int FL_SET_ALL_FOREGROUNDS                         = 0052525;
			static const int FL_SET_ALL_BACKGROUNDS                         = 0125252;
			static const int FL_DEFAULT                                     = 0052525;

		  private:
			int flags;
			Console::Color cvForeground, cvBackground;
			Console::Intensity cvIntensity;
			Console::Color keywordForeground, keywordBackground;
			Console::Intensity keywordIntensity;
			Console::Color typeModifierForeground, typeModifierBackground;
			Console::Intensity typeModifierIntensity;
			Console::Color constantForeground, constantBackground;
			Console::Intensity constantIntensity;
			Console::Color numberForeground, numberBackground;
			Console::Intensity numberIntensity;
			Console::Color discriminatorForeground, discriminatorBackground;
			Console::Intensity discriminatorIntensity;
			Console::Color tparamForeground, tparamBackground;
			Console::Intensity tparamIntensity;
			Console::Color typeForeground, typeBackground;
			Console::Intensity typeIntensity;

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

			inline Console::Color getCVQualifierForeground() const {
				return cvForeground;
			}

			inline void setCVQualifierForeground(Console::Color color) {
				cvForeground = color;
			}

			inline Console::Color getCVQualifierBackground() const {
				return cvBackground;
			}

			inline void setCVQualifierBackground(Console::Color color) {
				cvBackground = color;
			}

			inline Console::Intensity getCVQualifierIntensity() const {
				return cvIntensity;
			}

			inline void setCVQualifierIntensity(Console::Intensity intensity) {
				cvIntensity = intensity;
			}

			inline Console::Color getKeywordForeground() const {
				return keywordForeground;
			}

			inline void setKeywordForeground(Console::Color color) {
				keywordForeground = color;
			}

			inline Console::Color getKeywordBackground() const {
				return keywordBackground;
			}

			inline void setKeywordBackground(Console::Color color) {
				keywordBackground = color;
			}

			inline Console::Intensity getKeywordIntensity() const {
				return keywordIntensity;
			}

			inline void setKeywordIntensity(Console::Intensity intensity) {
				keywordIntensity = intensity;
			}

			inline Console::Color getTypeModifierForeground() const {
				return typeModifierForeground;
			}

			inline void setTypeModifierForeground(Console::Color color) {
				typeModifierForeground = color;
			}

			inline Console::Color getTypeModifierBackground() const {
				return typeModifierBackground;
			}

			inline void setTypeModifierBackground(Console::Color color) {
				typeModifierBackground = color;
			}

			inline Console::Intensity getTypeModifierIntensity() const {
				return typeModifierIntensity;
			}

			inline void setTypeModifierIntensity(Console::Intensity intensity) {
				typeModifierIntensity = intensity;
			}

			inline Console::Color getSymbolicConstantForeground() const {
				return constantForeground;
			}

			inline void setSymbolicConstantForeground(Console::Color color) {
				constantForeground = color;
			}

			inline Console::Color getSymbolicConstantBackground() const {
				return constantBackground;
			}

			inline void setSymbolicConstantBackground(Console::Color color) {
				constantBackground = color;
			}

			inline Console::Intensity getSymbolicConstantIntensity() const {
				return constantIntensity;
			}

			inline void setSymbolicConstantIntensity(Console::Intensity intensity) {
				constantIntensity = intensity;
			}

			inline Console::Color getNumericLiteralForeground() const {
				return numberForeground;
			}

			inline void setNumericLiteralForeground(Console::Color color) {
				numberForeground = color;
			}

			inline Console::Color getNumericLiteralBackground() const {
				return numberBackground;
			}

			inline void setNumericLiteralBackground(Console::Color color) {
				numberBackground = color;
			}

			inline Console::Intensity getNumericLiteralIntensity() const {
				return numberIntensity;
			}

			inline void setNumericLiteralIntensity(Console::Intensity intensity) {
				numberIntensity = intensity;
			}

			inline Console::Color getDiscriminatorForeground() const {
				return discriminatorForeground;
			}

			inline void setDiscriminatorForeground(Console::Color color) {
				discriminatorForeground = color;
			}

			inline Console::Color getDiscriminatorBackground() const {
				return discriminatorBackground;
			}

			inline void setDiscriminatorBackground(Console::Color color) {
				discriminatorBackground = color;
			}

			inline Console::Intensity getDiscriminatorIntensity() const {
				return discriminatorIntensity;
			}

			inline void setDiscriminatorIntensity(Console::Intensity intensity) {
				discriminatorIntensity = intensity;
			}

			inline Console::Color getUndefinedTemplateParameterForeground() const {
				return tparamForeground;
			}

			inline void setUndefinedTemplateParameterForeground(Console::Color color) {
				tparamForeground = color;
			}

			inline Console::Color getUndefinedTemplateParameterBackground() const {
				return tparamBackground;
			}

			inline void setUndefinedTemplateParameterBackground(Console::Color color) {
				tparamBackground = color;
			}

			inline Console::Intensity getUndefinedTemplateParameterIntensity() const {
				return tparamIntensity;
			}

			inline void setUndefinedTemplateParameterIntensity(Console::Intensity intensity) {
				tparamIntensity = intensity;
			}

			inline Console::Color getBuiltinTypeForeground() const {
				return typeForeground;
			}

			inline void setBuiltinTypeForeground(Console::Color color) {
				typeForeground = color;
			}

			inline Console::Color getBuiltinTypeBackground() const {
				return typeBackground;
			}

			inline void setBuiltinTypeBackground(Console::Color color) {
				typeBackground = color;
			}

			inline Console::Intensity getBuiltinTypeIntensity() const {
				return typeIntensity;
			}

			inline void setBuiltinTypeIntensity(Console::Intensity intensity) {
				typeIntensity = intensity;
			}

			static const HighlightingConfiguration defaultInstance;

		};

	  private:
		Console* console;
		HighlightingConfiguration configuration;

	  protected:
		virtual void highlightCVQualifier();
		virtual void highlightKeyword();
		virtual void highlightTypeModifier();
		virtual void highlightSymbolicConstant();
		virtual void highlightNumericLiteral();
		virtual void highlightDiscriminator();
		virtual void highlightUndefinedTemplateParameter();
		virtual void highlightBuiltinType();
		virtual void endHighlighting();
		virtual void notifyConsoleChanged();

	  public:
		ConsoleHighlightingSymbolSink(Console*);
		ConsoleHighlightingSymbolSink(Console*, const HighlightingConfiguration&);
		ConsoleHighlightingSymbolSink(const ConsoleHighlightingSymbolSink&);

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

#endif /* REDSTRAIN_MOD_PLATFORM_CONSOLEHIGHLIGHTINGSYMBOLSINK_HPP */
