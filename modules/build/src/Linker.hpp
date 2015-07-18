#ifndef REDSTRAIN_MOD_BUILD_LINKER_HPP
#define REDSTRAIN_MOD_BUILD_LINKER_HPP

#include <redstrain/redmond/constants.hpp>

#include "Linkage.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Linker {

	  private:
		redmond::Architecture architecture;

	  protected:
		inline void setTargetArchitecture(redmond::Architecture architecture) {
			this->architecture = architecture;
		}

	  public:
		Linker(redmond::Architecture = REDSTRAIN_BUILD_DEFAULT_ARCH);
		Linker(const Linker&);
		virtual ~Linker();

		inline redmond::Architecture getTargetArchitecture() {
			return architecture;
		}

		virtual Linkage* newLinkage(const std::string&, Linkage::LinkMode) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKER_HPP */
