#ifndef REDSTRAIN_MOD_BUILD_CONSOLEBUILDUI_HPP
#define REDSTRAIN_MOD_BUILD_CONSOLEBUILDUI_HPP

#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "AbstractBuildUI.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ConsoleBuildUI : public AbstractBuildUI {

	  public:
		enum {
			PRINT_PREDICTIVE = 01,
			PRINT_DEFINITIVE = 02
		};

	  public:
		static const int DEFAULT_FLAGS = PRINT_PREDICTIVE | PRINT_DEFINITIVE;

	  private:
		io::FileOutputStream output;
		io::DefaultConfiguredOutputStream<char>::Stream formatted;
		unsigned componentTypeWidth, componentNameWidth;
		int flags;

	  private:
		void indent(unsigned);
		void printAction(unsigned, unsigned, const ActionDescriptor&, bool);

	  public:
		ConsoleBuildUI();
		ConsoleBuildUI(const platform::File::Handle&);
		ConsoleBuildUI(platform::Console::StandardHandle);
		ConsoleBuildUI(const ConsoleBuildUI&);

		inline io::FileOutputStream& getOutputStream() {
			return output;
		}

		inline const io::FileOutputStream& getOutputStream() const {
			return output;
		}

		inline io::DefaultConfiguredOutputStream<char>::Stream& getFormattedOutputStream() {
			return formatted;
		}

		inline const io::DefaultConfiguredOutputStream<char>::Stream& getFormattedOutputStream() const {
			return formatted;
		}

		inline unsigned getMinimalComponentTypeWidth() const {
			return componentTypeWidth;
		}

		inline void setMinimalComponentTypeWidth(unsigned width) {
			componentTypeWidth = width;
		}

		inline unsigned getMinimalComponentNameWidth() const {
			return componentNameWidth;
		}

		inline void setMinimalComponentNameWidth(unsigned width) {
			componentNameWidth = width;
		}

		inline int getFlags() const {
			return flags;
		}

		inline void setFlags(int flags) {
			this->flags = flags;
		}

		virtual void willPerformAction(const ActionDescriptor&, bool);
		virtual void wouldPerformAction(const ActionDescriptor&, bool);
		virtual void endPredictiveRun();
		virtual void endDefinitiveRun();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CONSOLEBUILDUI_HPP */
