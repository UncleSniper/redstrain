#ifndef REDSTRAIN_MOD_DAMNATION_TK_SCREEN_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_SCREEN_HPP

#include <vector>
#include <stdint.h>
#include <redstrain/util/ListenerSet.hpp>

#include "../TerminalCanvas.hpp"
#include "../tweaks.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Task;
	class Layer;
	class Theme;
	class DrawContext;

	class REDSTRAIN_DAMNATION_API Screen {

	  private:
		typedef std::vector<Layer*> Layers;

	  public:
		static const uint64_t DEFAULT_MAX_INPUT_DELAY
				= static_cast<uint64_t>(REDSTRAIN_DAMNATION_SCREEN_DEFAULT_MAX_INPUT_DELAY);

	  private:
		TerminalCanvas& terminal;
		Size termSize;
		DrawContext *const drawContext;
		Layer* activeLayer;
		util::ListenerSet<Task> tasks;
		uint64_t maxInputDelay;
		bool shouldFinish;

	  private:
		void pushInput(const KeySym&);
		uint64_t advanceTasks();

	  protected:
		Screen(const Screen&);

	  public:
		Screen(TerminalCanvas&, Theme&);
		~Screen();

		inline TerminalCanvas& getTerminal() {
			return terminal;
		}

		inline const TerminalCanvas& getTerminal() const {
			return terminal;
		}

		inline Layer* getActiveLayer() const {
			return activeLayer;
		}

		inline uint64_t getMaxInputDelay() const {
			return maxInputDelay;
		}

		void setMaxInputDelay(uint64_t);

		inline void finish() {
			shouldFinish = true;
		}

		Layer* addLayer();
		Layer& getLayer(unsigned);
		void swapLayers(Layer&, Layer&);
		void removeLayer(Layer&);
		void notifyTerminalSizeChanged();
		Theme& getTheme() const;
		void setTheme(Theme&);
		void draw();
		void pollInput();
		bool tryInput();
		void getTasks(util::Appender<Task*>&) const;
		bool addTask(Task&);
		bool removeTask(Task&);
		void clearTasks();
		void cycle();
		void loop();
		void leaveRendition();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_SCREEN_HPP */
