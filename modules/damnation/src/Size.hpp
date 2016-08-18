#ifndef REDSTRAIN_MOD_DAMNATION_SIZE_HPP
#define REDSTRAIN_MOD_DAMNATION_SIZE_HPP

#include "api.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API Size {

	  public:
		unsigned width, height;

	  public:
		Size(unsigned, unsigned);
		Size(const Size&);

		Size& operator=(const Size&);
		bool operator==(const Size&) const;
		bool operator!=(const Size&) const;

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_SIZE_HPP */
