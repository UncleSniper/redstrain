#ifndef REDSTRAIN_MOD_DAMNATION_TK_LINEBREAKER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LINEBREAKER_HPP

#include <redstrain/util/Appender.hpp>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/util/types.hpp>
#include <redstrain/text/types.hpp>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API LineBreaker : public util::ReferenceCounted {

	  public:
		LineBreaker();
		LineBreaker(const LineBreaker&);

		virtual void breakIntoLines(const text::String16&, util::MemorySize, util::Appender<text::String16>&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LINEBREAKER_HPP */
