#ifndef REDSTRAIN_MOD_DAMNATION_TK_BUTTON_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_BUTTON_HPP

#include <redstrain/text/Transcode.hpp>
#include <redstrain/util/FireEventAppender.hpp>

#include "LeafWidget.hpp"
#include "SimpleBorder.hpp"
#include "CommandListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

#define REDSTRAIN_DAMNATION_BUTTON_ACTIVE_FOREGROUND_PARAMS \
	"org.unclesniper.damnation.abstract.control.active.fg", \
	std::string(className ? className : "redengine.damnation.tk.Button") + ".caption.active.fg", \
	"buttonForeground", \
	Color::SIMPLE_WHITE
#define REDSTRAIN_DAMNATION_BUTTON_INACTIVE_FOREGROUND_PARAMS \
	"org.unclesniper.damnation.abstract.control.inactive.fg", \
	std::string(className ? className : "redengine.damnation.tk.Button") + ".caption.inactive.fg", \
	"buttonInactiveForeground", \
	Color::SIMPLE_WHITE

	template<typename CommandT>
	class Button : public LeafWidget {

	  public:
		typedef CommandT Command;

	  private:
		text::String16 caption;
		CommandT command;
		util::ListenerSet<CommandListener<CommandT> > commandListeners;
		SimpleBorder defaultBorder;
		Color foreground, inactiveForeground;

	  private:
		void setDefaultBorderColors(const char* className) {
			const string cn(className ? className : "redengine.damnation.tk.Button");
			defaultBorder.getForeground().setUse(cn + ".defaultBorder.active.fg");
			defaultBorder.getBackground().setUse(cn + ".defaultBorder.active.bg");
			defaultBorder.getInactiveForeground().setUse(cn + ".defaultBorder.inactive.fg");
			defaultBorder.getInactiveBackground().setUse(cn + ".defaultBorder.inactive.bg");
		}

		void addDefaultKeyBindings() {
			//TODO
		}

	  protected:
		void fireCommandIssued(CommandListener<CommandT>::CommandEvent& event) {
			util::FireEventAppender<CommandListener<CommandT>, CommandListener<CommandT>::CommandEvent>
					sink(&CommandListener<CommandT>::commandIssued, event);
			commandListeners.getListeners(sink);
		}

		virtual Size getMinimumSizeWithinBorder() {
			return Size(static_cast<unsigned>(caption.length()), 1u);
		}

		virtual Size getMaximumSizeWithinBorder() {
			return Size(static_cast<unsigned>(caption.length()), 1u);
		}

		virtual Size getPreferredSizeWithinBorder() {
			return Size(static_cast<unsigned>(caption.length()), 1u);
		}

		virtual unsigned getHeightForWidthWithinBorder(unsigned) {
			return 1u;
		}

		virtual unsigned getWidthForHeightWithinBorder(unsigned height) {
			return height ? static_cast<unsigned>(caption.length()) : 0u;
		}

		virtual void drawIntoBorder(const Rectangle& rectangle, DrawContext& context) {
			if(!rectangle.width || !rectangle.height)
				return;
			bool focused = isFocused();
			unsigned clen = static_cast<unsigned>(caption.length());
			text::String16 cpt;
			if(clen > rectangle.width) {
				cpt = caption.substr(static_cast<text::String16::size_type>(1u),
						static_cast<text::String16::size_type>(rectangle.width));
				clen = rectangle.width;
			}
			else
				cpt = caption;
			ClippingTerminalCanvas& canvas = context.getCanvas();
			if(focused) {
				canvas.setForegroundColor(foreground.get(context));
				canvas.setBackgroundColor(background.get(context));
			}
			else {
				canvas.setForegroundColor(inactiveForeground.get(context));
				canvas.setBackgroundColor(inactiveBackground.get(context));
			}
			int r = rectangle.row + static_cast<int>((rectangle.height - 1u) / 2u);
			unsigned ur = r < 0 ? 0u : static_cast<unsigned>(r);
			int c = rectangle.column + static_cast<int>((rectangle.width - clen) / 2u);
			unsigned uc = c < 0 ? 0u : static_cast<unsigned>(c);
			canvas.moveTo(ur, uc);
			canvas.write(cpt);
			if(focused)
				context.setCursorPosition(ur, uc, false);
		}

	  public:
		Button(const std::string& caption, const CommandT& command, const char* className = NULL)
				: LeafWidget(className ? className : "redengine.damnation.tk.Button"),
				caption(text::Transcode::utf8ToBMP(caption)), command(command),
				foreground(REDSTRAIN_DAMNATION_BUTTON_ACTIVE_FOREGROUND_PARAMS),
				inactiveForeground(REDSTRAIN_DAMNATION_BUTTON_INACTIVE_FOREGROUND_PARAMS) {
			setDefaultBorderColors(className);
			setBorder(&defaultBorder);
			addLocalColor(foreground);
			addLocalColor(inactiveForeground);
			addDefaultKeyBindings();
		}

		Button(const text::String16& caption, const CommandT& command, const char* className = NULL)
				: LeafWidget(className ? className : "redengine.damnation.tk.Button"),
				caption(caption), command(command),
				foreground(REDSTRAIN_DAMNATION_BUTTON_ACTIVE_FOREGROUND_PARAMS),
				inactiveForeground(REDSTRAIN_DAMNATION_BUTTON_INACTIVE_FOREGROUND_PARAMS) {
			setDefaultBorderColors(className);
			setBorder(&defaultBorder);
			addLocalColor(foreground);
			addLocalColor(inactiveForeground);
			addDefaultKeyBindings();
		}

		Button(const Button& button) : Widget(button), LeafWidget(button), caption(button.caption),
				command(button.command), commandListeners(button.commandListeners),
				defaultBorder(button.defaultBorder), foreground(button.foreground),
				inactiveForeground(button.inactiveForeground) {
			if(button.getBorder() == &button.defaultBorder)
				setBorder(&defaultBorder);
			addLocalColor(foreground);
			addLocalColor(inactiveForeground);
			addDefaultKeyBindings();
		}

		virtual ~Button() {}

		inline const text::String16& getCaption() const {
			return caption;
		}

		void setCaption(const text::String16& caption) {
			this->caption = caption;
		}

		std::string getCaptionUTF8() const {
			return text::Transcode::bmpToUTF8(caption);
		}

		void setCaption(const std::string& caption) {
			this->caption = text::Transcode::utf8ToBMP(caption);
		}

		inline CommandT& getCommand() {
			return command;
		}

		inline const CommandT& getCommand() const {
			return command;
		}

		inline SimpleBorder& getDefaultBorder() {
			return defaultBorder;
		}

		inline const SimpleBorder& getDefaultBorder() const {
			return defaultBorder;
		}

		inline Color& getForeground() {
			return foreground;
		}

		inline const Color& getForeground() const {
			return foreground;
		}

		void setForeground(const Color& color) {
			foreground = color;
		}

		inline Color& getInactiveForeground() {
			return inactiveForeground;
		}

		inline const Color& getInactiveForeground() const {
			return inactiveForeground;
		}

		void setInactiveForeground(const Color& color) {
			inactiveForeground = color;
		}

		unsigned getForegroundColor() const {
			return foreground.getLocal();
		}

		void setForegroundColor(unsigned color) {
			foreground.setLocal(color);
		}

		unsigned getInactiveForegroundColor() const {
			return inactiveForeground.getLocal();
		}

		void setInactiveForegroundColor(unsigned color) {
			inactiveForeground.setLocal(color);
		}

		void getCommandListeners(util::Appender<CommandListener<CommandT>*>& sink) const {
			commandListeners.getListeners(sink);
		}

		bool addCommandListener(CommandListener<CommandT>& listener) {
			return commandListeners.addListener(listener);
		}

		bool removeCommandListener(CommandListener<CommandT>& listener) {
			return commandListeners.removeListener(listener);
		}

		void clearCommandListeners() {
			commandListeners.clearListeners();
		}

		void press() {
			CommandListener<CommandT>::CommandEvent event(*this, command);
			fireCommandIssued(event);
		}

		virtual bool takesFocus() {
			return true;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_BUTTON_HPP */
