#include <set>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Thread.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/platform/utils.hpp>

#include "Task.hpp"
#include "Layer.hpp"
#include "Screen.hpp"
#include "StagePanel.hpp"
#include "DrawContext.hpp"
#include "LayerDepthOutOfBoundsError.hpp"
#include "CannotModifyForeignLayerError.hpp"
#include "CannotModifyRemovedLayerError.hpp"

using std::set;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::util::MemorySize;
using redengine::platform::Thread;
using redengine::util::ListenerSet;
using redengine::util::IntegerBounds;
using redengine::platform::currentTimeMillis;

namespace redengine {
namespace damnation {
namespace tk {

	Screen::Screen(TerminalCanvas& terminal, Theme& rootTheme) : terminal(terminal), termSize(terminal.getSize()),
			drawContext(new DrawContext(terminal, rootTheme)), activeLayer(NULL),
			maxInputDelay(Screen::DEFAULT_MAX_INPUT_DELAY), shouldFinish(false) {}

	Screen::Screen(const Screen& screen) : terminal(screen.terminal), termSize(screen.termSize),
			drawContext(new DrawContext(screen.drawContext->getCanvas().getTerminal(),
			screen.drawContext->getTheme())), activeLayer(NULL), maxInputDelay(screen.maxInputDelay),
			shouldFinish(false) {}

	Screen::~Screen() {
		Layers::const_iterator lbegin(layers.begin()), lend(layers.end());
		for(; lbegin != lend; ++lbegin)
			delete *lbegin;
		delete drawContext;
	}

	void Screen::setMaxInputDelay(uint64_t delay) {
		maxInputDelay = delay ? delay : Screen::DEFAULT_MAX_INPUT_DELAY;
	}

	Layer& Screen::addLayer() {
		Delete<Layer> layer(new Layer(*this, static_cast<unsigned>(layers.size())));
		Layer* oldActive = NULL;
		bool setActive = false;
		if(!activeLayer || (!layers.empty() && activeLayer == layers.back())) {
			oldActive = activeLayer;
			setActive = true;
		}
		layer->getStage().setSize(termSize);
		layers.push_back(*layer);
		Layer* l = layer.set();
		if(setActive)
			activeLayer = l;
		if(oldActive)
			oldActive->notifyActiveChanged(false);
		if(activeLayer == l)
			l->notifyActiveChanged(true);
		return *l;
	}

	Layer& Screen::getLayer(unsigned depth) {
		if(depth >= static_cast<unsigned>(layers.size()))
			throw LayerDepthOutOfBoundsError(static_cast<MemorySize>(depth));
		return *layers[static_cast<Layers::size_type>(depth)];
	}

	unsigned Screen::getTrueDepthOfLayer(Layer& layer) const {
		Layers::const_iterator lbegin(layers.begin()), lend(layers.end());
		unsigned depth = 0u;
		for(; lbegin != lend; ++lbegin, ++depth) {
			if(*lbegin == &layer)
				return depth;
		}
		return Layer::INVALID_LAYER_DEPTH;
	}

	void Screen::swapLayers(Layer& a, Layer& b) {
		if(&a.getScreen() != this || &b.getScreen() != this)
			throw CannotModifyForeignLayerError();
		if(a.getDepth() == Layer::INVALID_LAYER_DEPTH || a.isCurrentlyRemoving())
			throw CannotModifyRemovedLayerError();
		if(b.getDepth() == Layer::INVALID_LAYER_DEPTH || b.isCurrentlyRemoving())
			throw CannotModifyRemovedLayerError();
		if(&a == &b)
			return;
		unsigned da = a.getDepth(), db = b.getDepth();
		layers[static_cast<Layers::size_type>(da)] = &b;
		layers[static_cast<Layers::size_type>(db)] = &a;
		a.notifyDepthChanged();
		b.notifyDepthChanged();
		a.getStage().setOpaque(!db);
		b.getStage().setOpaque(!da);
	}

	bool Screen::removeLayer(Layer& layer) {
		if(&layer.getScreen() != this)
			throw CannotModifyForeignLayerError();
		unsigned depth = layer.getDepth();
		if(depth == Layer::INVALID_LAYER_DEPTH)
			return false;
		if(!layer.isCurrentlyRemoving())
			return layer.remove();
		layers.erase(layers.begin() + static_cast<Layers::size_type>(depth));
		unsigned u, size = static_cast<unsigned>(layers.size());
		for(u = depth; u < size; ++u)
			layers[static_cast<Layers::size_type>(u)]->notifyDepthChanged();
		bool activeChanged = false;
		if(activeLayer == &layer) {
			if(size > depth)
				activeLayer = layers[static_cast<Layers::size_type>(depth)];
			else if(depth && size)
				activeLayer = layers.back();
			else
				activeLayer = NULL;
			activeChanged = true;
		}
		if(activeChanged) {
			layer.notifyActiveChanged(false);
			if(activeLayer)
				activeLayer->notifyActiveChanged(true);
		}
		return true;
	}

	void Screen::notifyTerminalSizeChanged() {
		Size newSize(terminal.getSize());
		if(newSize.height == termSize.height && newSize.width == termSize.width)
			return;
		termSize = newSize;
		Layers::const_iterator lbegin(layers.begin()), lend(layers.end());
		for(; lbegin != lend; ++lbegin) {
			StagePanel& stage = (*lbegin)->getStage();
			stage.setSize(termSize);
			stage.pack();
		}
	}

	Theme& Screen::getTheme() const {
		return drawContext->getTheme();
	}

	void Screen::setTheme(Theme& theme) {
		drawContext->setTheme(theme);
		Layers::const_iterator lbegin(layers.begin()), lend(layers.end());
		for(; lbegin != lend; ++lbegin)
			(*lbegin)->getStage().clearCachedColors();
	}

	void Screen::draw() {
		//TODO: cursor position stuff
		if(terminal.hasSizeChanged())
			notifyTerminalSizeChanged();
		Layers::const_iterator lbegin(layers.begin()), lend(layers.end());
		for(; lbegin != lend; ++lbegin) {
			drawContext->reset();
			(*lbegin)->getStage().draw(Point(0, 0), *drawContext);
		}
		terminal.flush();
	}

	void Screen::pollInput() {
		KeySym key(terminal.read(true));
		if(key.getType() == KeySym::T_WINCH)
			notifyTerminalSizeChanged();
		else
			pushInput(key);
	}

	bool Screen::tryInput() {
		KeySym key(terminal.read(false));
		if(key.getType() == KeySym::T_NONE)
			return false;
		if(key.getType() == KeySym::T_WINCH)
			notifyTerminalSizeChanged();
		else
			pushInput(key);
		return true;
	}

	void Screen::pushInput(const KeySym& key) {
		if(!activeLayer)
			return;
		Widget* focus = activeLayer->getCurrentFocus();
		if(!focus)
			return;
		InputListener::InputEvent event(focus, key);
		focus->processInput(event);
	}

	void Screen::getTasks(util::Appender<Task*>& sink) const {
		tasks.getListeners(sink);
	}

	bool Screen::addTask(Task& task) {
		return tasks.addListener(task);
	}

	bool Screen::removeTask(Task& task) {
		return tasks.removeListener(task);
	}

	void Screen::clearTasks() {
		tasks.clearListeners();
	}

	static const uint64_t NO_GRACE = IntegerBounds<uint64_t>::MAX;

	class ScreenAdvanceTaskAppender : public Appender<Task*> {

	  private:
		const uint64_t now;
		uint64_t grace;
		set<Task*> toRemove;

	  public:
		ScreenAdvanceTaskAppender();
		ScreenAdvanceTaskAppender(const ScreenAdvanceTaskAppender&);

		inline uint64_t getGrace() const {
			return grace;
		}

		void removeTasks(ListenerSet<Task>&) const;

		virtual void append(Task *const&);

	};

	ScreenAdvanceTaskAppender::ScreenAdvanceTaskAppender() : now(currentTimeMillis()), grace(NO_GRACE) {}

	ScreenAdvanceTaskAppender::ScreenAdvanceTaskAppender(const ScreenAdvanceTaskAppender& appender)
			: Appender<Task*>(appender), now(appender.now), grace(appender.grace) {}

	void ScreenAdvanceTaskAppender::removeTasks(ListenerSet<Task>& tasks) const {
		set<Task*>::const_iterator begin(toRemove.begin()), end(toRemove.end());
		for(; begin != end; ++begin)
			tasks.removeListener(**begin);
	}

	void ScreenAdvanceTaskAppender::append(Task *const& task) {
		uint64_t next = task->getNextExecutionTime();
		if(next <= now) {
			if(!task->executeTask())
				toRemove.insert(task);
			else {
				next = task->getNextExecutionTime();
				uint64_t delta = now >= next ? static_cast<uint64_t>(0u) : next - now;
				if(grace == NO_GRACE || delta < grace)
					grace = delta;
			}
		}
		else {
			uint64_t delta = next - now;
			if(grace == NO_GRACE || delta < grace)
				grace = delta;
		}
	}

	uint64_t Screen::advanceTasks() {
		ScreenAdvanceTaskAppender sink;
		tasks.getListeners(sink);
		sink.removeTasks(tasks);
		return sink.getGrace();
	}

	void Screen::cycle() {
		for(;;) {
			uint64_t grace = advanceTasks();
			if(!grace)
				continue;
			draw();
			if(grace == NO_GRACE) {
				pollInput();
				return;
			}
			while(grace > maxInputDelay) {
				if(tryInput())
					return;
				uint64_t now = currentTimeMillis();
				Thread::sleep(maxInputDelay);
				uint64_t delta = currentTimeMillis() - now;
				if(delta >= grace)
					grace = static_cast<uint64_t>(0u);
				else
					grace -= delta;
			}
			if(tryInput())
				return;
			if(grace)
				Thread::sleep(grace);
		}
	}

	void Screen::loop() {
		shouldFinish = false;
		do
			cycle();
		while(!shouldFinish);
	}

	void Screen::leaveRendition() {
		Size size(terminal.getSize());
		if(size.height)
			terminal.moveTo(size.height - 1u, 0u);
		terminal.resetTextAttributes();
		terminal.newLine();
		terminal.flush();
	}

}}}
