#ifndef REDSTRAIN_MOD_DAMNATION_TK_INSETS_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_INSETS_HPP

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Insets {

	  public:
		unsigned top, left, right, bottom;

	  public:
		Insets(unsigned, unsigned, unsigned, unsigned);
		Insets(const Insets&);

		Insets& operator=(const Insets&);
		bool operator==(const Insets&) const;
		bool operator!=(const Insets&) const;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_INSETS_HPP */
