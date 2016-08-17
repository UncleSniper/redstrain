#include "TerminalCanvas.hpp"

namespace redengine {
namespace damnation {

	class TerminalBinding;

	class REDSTRAIN_DAMNATION_API BindingTerminalCanvas : public TerminalCanvas {

	  private:
		enum CarriageReturnStrategy {
			CRS_CARRIAGE_RETURN,
			CRS_COLUMN_TO,
			CRS_COLUMN_BY,
			CRS_MOVE_TO,
			CRS_MOVE_BY,
			CRS_CURSOR_LEFT_BY,
			CRS_CURSOR_LEFT
		};

		enum ClearScreenStrategy {
			CSS_CLEAR_SCREEN,
			CSS_ROW_TO,
			CSS_ROW_BY,
			CSS_MOVE_TO,
			CSS_MOVE_BY,
			CSS_CURSOR_DOWN_BY,
			CSS_NEW_LINE
		};

		enum HomeRowStrategy {
			HRS_ROW_TO,
			HRS_ROW_BY,
			HRS_MOVE_TO,
			HRS_MOVE_BY,
			HRS_CURSOR_UP_BY,
			HRS_CURSOR_UP,
			HRS_IMPOSSIBLE
		};

		enum MoveToStrategy {
			MTS_MOVE_TO,
			MTS_MOVE_BY,
			MTS_SPLIT
		};

		enum RowToStrategy {
			RTS_ROW_TO,
			RTS_ROW_BY,
			RTS_MOVE_TO,
			RTS_MOVE_BY,
			RTS_CURSOR_UP_BY_DOWN_BY,
			RTS_CURSOR_UP_BY_DOWN,
			RTS_CURSOR_UP_DOWN_BY,
			RTS_CURSOR_UP_BY_NEW_LINE_COLUMN_TO,
			RTS_CURSOR_UP_BY_NEW_LINE_COLUMN_BY,
			RTS_CURSOR_UP_BY_NEW_LINE_CURSOR_RIGHT_BY,
			RTS_CURSOR_UP_BY_NEW_LINE_CURSOR_RIGHT,
			RTS_CURSOR_UP_DOWN,
			RTS_CURSOR_UP_NEW_LINE_COLUMN_TO,
			RTS_CURSOR_UP_NEW_LINE_COLUMN_BY,
			RTS_CURSOR_UP_NEW_LINE_CURSOR_RIGHT_BY,
			RTS_CURSOR_UP_NEW_LINE_CURSOR_RIGHT
		};

		enum ColumnToStrategy {
			CTS_COLUMN_TO,
			CTS_COLUMN_BY,
			CTS_MOVE_TO,
			CTS_MOVE_BY,
			CTS_CURSOR_LEFT_BY_RIGHT_BY,
			CTS_CURSOR_LEFT_BY_RIGHT,
			CTS_CURSOR_LEFT_RIGHT_BY,
			CTS_CURSOR_LEFT_RIGHT
		};

		enum CursorMotionStrategy {
			CMS_CURSOR_DIRECTION,
			CMS_CURSOR_DIRECTION_BY,
			CMS_DIMENSION_TO
		};

	  private:
		TerminalBinding& binding;
		Size currentSize;
		bool sizeChangePending;
		unsigned cursorRow, cursorColumn;
		const CarriageReturnStrategy carriageReturnStrategy;
		const ClearScreenStrategy clearScreenStrategy;
		const HomeRowStrategy homeRowStrategy;
		const MoveToStrategy moveToStrategy;
		const RowToStrategy rowToStrategy;
		const ColumnToStrategy columnToStrategy;
		const CursorMotionStrategy cursorDownStrategy, cursorDownByStrategy;
		const CursorMotionStrategy cursorUpStrategy, cursorUpByStrategy;
		const CursorMotionStrategy cursorLeftStrategy, cursorLeftByStrategy;
		const CursorMotionStrategy cursorRightStrategy, cursorRightByStrategy;

	  private:
		void updateSize();
		void homeRow();
		void doTab();
		void writeUntabbed(const std::string&);
		void writeUntabbed(const text::String16&);

		static CarriageReturnStrategy determineCarriageReturnStrategy(TerminalBinding&);
		static ClearScreenStrategy determineClearScreenStrategy(TerminalBinding&);
		static HomeRowStrategy determineHomeRowStrategy(TerminalBinding&);
		static MoveToStrategy determineMoveToStrategy(TerminalBinding&);
		static RowToStrategy determineRowToStrategy(TerminalBinding&);
		static ColumnToStrategy determineColumnToStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorDownStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorDownByStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorUpStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorUpByStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorLeftStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorLeftByStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorRightStrategy(TerminalBinding&);
		static CursorMotionStrategy determineCursorRightByStrategy(TerminalBinding&);

	  protected:
		BindingTerminalCanvas(const BindingTerminalCanvas&);

	  public:
		BindingTerminalCanvas(TerminalBinding&);

		inline TerminalBinding& getBinding() {
			return binding;
		}

		inline const TerminalBinding& getBinding() const {
			return binding;
		}

		virtual bool hasSizeChanged();
		virtual Size getSize();
		virtual unsigned getCursorRow();
		virtual unsigned getCursorColumn();
		virtual void getCursorPosition(unsigned&, unsigned&);

		virtual unsigned getTabSpacing();
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
		virtual void write(char);
		virtual void write(const std::string&);
		virtual void write(text::Char16);
		virtual void write(const text::String16&);
		virtual void write(BoxSymbol);
		virtual void flush();
		virtual void closeCanvas();

	};

}}
