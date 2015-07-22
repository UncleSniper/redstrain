#ifndef REDSTRAIN_MOD_BUILD_STATICVALVE_HPP
#define REDSTRAIN_MOD_BUILD_STATICVALVE_HPP

#include "Valve.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API StaticValve : public Valve {

	  private:
		bool open;

	  public:
		StaticValve();
		StaticValve(const StaticValve&);

		inline void setOpen(bool open) {
			this->open = open;
		}

		virtual bool isOpen(BuildContext&);

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpValve(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_STATICVALVE_HPP */
