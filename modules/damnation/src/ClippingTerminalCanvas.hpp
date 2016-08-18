#ifndef REDSTRAIN_MOD_DAMNATION_CLIPPINGTERMINALCANVAS_HPP
#define REDSTRAIN_MOD_DAMNATION_CLIPPINGTERMINALCANVAS_HPP

#include <deque>

#include "Size.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"
#include "TerminalCanvas.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API ClippingTerminalCanvas : public TerminalCanvas {

	  private:
		typedef std::deque<Rectangle> ClipStack;
		typedef std::deque<Point> OriginStack;

	  private:
		TerminalCanvas& terminal;
		Size currentSize;
		bool sizeChangePending;
		ClipStack clipStack;
		OriginStack originStack;
		int cursorRow, cursorColumn;

	  private:
		void updateSize();

	  public:
		ClippingTerminalCanvas(TerminalCanvas&);
		ClippingTerminalCanvas(const ClippingTerminalCanvas&);
		virtual ~ClippingTerminalCanvas();

		void pushClip(const Rectangle&);
		void popClip();
		void clearClip();
		Rectangle getCurrentClipMask();

		void pushOrigin(const Point&);
		void popOrigin();
		void clearOrigin();
		const Point& getCurrentOrigin() const;

		virtual bool hasSizeChanged();
		virtual Size getSize();
		virtual unsigned getCursorRow();
		virtual unsigned getCursorColumn();
		virtual void getCursorPosition(unsigned&, unsigned&);

		virtual unsigned getTabSpacing();
		virtual unsigned getTabOffset();
		virtual void carriageReturn();
		virtual void newLine();
		virtual void clearScreen(bool);
		virtual void moveTo(unsigned, unsigned);
		virtual void rowTo(unsigned);
		virtual void columnTo(unsigned);
		virtual void moveBy(int, int);
		virtual void rowBy(int);
		virtual void columnBy(int);
		virtual void cursorDown();
		virtual void cursorDownBy(unsigned);
		virtual void cursorUp();
		virtual void cursorUpBy(unsigned);
		virtual void cursorLeft();
		virtual void cursorLeftBy(unsigned);
		virtual void cursorRight();
		virtual void cursorRightBy(unsigned);

		virtual unsigned setForegroundColor(unsigned);
		virtual unsigned setBackgroundColor(unsigned);
		virtual int setTextAttributes(int);
		virtual int addTextAttributes(int);
		virtual int removeTextAttributes(int);
		virtual void resetTextAttributes();

		virtual KeySym read(bool);
		virtual void prepareForWrite();
		virtual void writeTab();
		virtual void writeNonControl(char);
		virtual void writeNonControl(const char*, util::MemorySize);
		virtual void writeNonControl(text::Char16);
		virtual void writeNonControl(const text::Char16*, util::MemorySize);
		virtual void write(BoxSymbol);
		virtual void flush();
		virtual void closeCanvas();

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_CLIPPINGTERMINALCANVAS_HPP */
