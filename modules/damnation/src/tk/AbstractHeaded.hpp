#ifndef REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTHEADED_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTHEADED_HPP

#include "Headed.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API AbstractHeaded : public Headed {

	  private:
		bool inlineHeader;
		unsigned externalCount;

	  protected:
		virtual void notifyHeaderInlinenessChanged() = 0;

	  public:
		AbstractHeaded(bool);
		AbstractHeaded(const AbstractHeaded&);

		void setInlineHeader(bool);

		virtual bool isInlineHeader();
		virtual bool isHeaderExternallyDrawn();
		virtual void setHeaderExternallyDrawn(bool);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTHEADED_HPP */
