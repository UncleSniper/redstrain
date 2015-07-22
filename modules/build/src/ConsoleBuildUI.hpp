#ifndef REDSTRAIN_MOD_BUILD_CONSOLEBUILDUI_HPP
#define REDSTRAIN_MOD_BUILD_CONSOLEBUILDUI_HPP

#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>

#include "AbstractBuildUI.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ConsoleBuildUI : public AbstractBuildUI {

	  private:
		io::FileOutputStream output;
		io::FormattedOutputStream<char> formatted;
		unsigned componentTypeWidth, componentNameWidth;

	  private:
		void indent(unsigned);
		void printAction(unsigned, unsigned, const ActionDescriptor&);

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

		virtual void willPerformAction(const ActionDescriptor&);
		virtual void wouldPerformAction(const ActionDescriptor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CONSOLEBUILDUI_HPP */
