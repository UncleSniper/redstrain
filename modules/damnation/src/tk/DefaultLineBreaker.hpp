#ifndef REDSTRAIN_MOD_DAMNATION_TK_DEFAULTLINEBREAKER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DEFAULTLINEBREAKER_HPP

#include "LineBreaker.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API DefaultLineBreaker : public LineBreaker {

	  public:
		DefaultLineBreaker();
		DefaultLineBreaker(const DefaultLineBreaker&);

		virtual void breakIntoLines(const text::String16&, util::MemorySize, util::Appender<text::String16>&);

	  public:
		static DefaultLineBreaker instance;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DEFAULTLINEBREAKER_HPP */
