#ifndef REDSTRAIN_MOD_BUILD_LINKER_HPP
#define REDSTRAIN_MOD_BUILD_LINKER_HPP

#include "Linkage.hpp"
#include "Architecture.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Linker {

	  private:
		Architecture architecture;

	  protected:
		inline void setTargetArchitecture(Architecture architecture) {
			this->architecture = architecture;
		}

	  public:
		Linker(Architecture = REDSTRAIN_BUILD_DEFAULT_ARCH);
		Linker(const Linker&);
		virtual ~Linker();

		inline Architecture getTargetArchitecture() {
			return architecture;
		}

		virtual Linkage* newLinkage(const std::string&, Linkage::LinkMode) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKER_HPP */
