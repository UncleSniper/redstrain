#ifndef REDSTRAIN_MOD_DAMNATION_TK_LABEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LABEL_HPP

#include <list>
#include <redstrain/util/types.hpp>
#include <redstrain/text/types.hpp>

#include "LeafWidget.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class LineBreaker;

	class REDSTRAIN_DAMNATION_API Label : public LeafWidget {

	  private:
		typedef std::list<text::String16> Lines;

	  private:
		text::String16 caption;
		LineBreaker* lineBreaker;
		Lines brokenLines;
		bool breakingCached;
		util::MemorySize brokenWidth;
		Color foreground;
		bool setCursorPosition;

	  private:
		void rebreak(util::MemorySize);

	  protected:
		virtual Size getMinimumSizeWithinBorder();
		virtual Size getMaximumSizeWithinBorder();
		virtual Size getPreferredSizeWithinBorder();
		virtual unsigned getHeightForWidthWithinBorder(unsigned);
		virtual unsigned getWidthForHeightWithinBorder(unsigned);
		virtual void drawIntoBorder(const Rectangle&, DrawContext&);

	  public:
		Label(const std::string&, const char* = NULL);
		Label(const text::String16&, const char* = NULL);
		Label(const Label&);
		virtual ~Label();

		inline const text::String16& getCaption() const {
			return caption;
		}

		void setCaption(const text::String16&);
		std::string getCaptionUTF8() const;
		void setCaption(const std::string&);

		inline LineBreaker* getLineBreaker() const {
			return lineBreaker;
		}

		void setLineBreaker(LineBreaker*);

		inline Color& getForeground() {
			return foreground;
		}

		inline const Color& getForeground() const {
			return foreground;
		}

		void setForeground(const Color&);
		unsigned getForegroundColor() const;
		void setForegroundColor(unsigned);

		inline bool isSetCursorPosition() const {
			return setCursorPosition;
		}

		inline void setSetCursorPosition(bool setCursorPosition) {
			this->setCursorPosition = setCursorPosition;
		}

		virtual bool takesFocus();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LABEL_HPP */
