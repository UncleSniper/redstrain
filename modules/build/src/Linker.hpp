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
		redmond::OperatingSystem targetOS;

	  protected:
		inline void setTargetArchitecture(redmond::Architecture architecture) {
			this->architecture = architecture;
		}

	  public:
		Linker(redmond::Architecture = REDSTRAIN_BUILD_DEFAULT_ARCH,
				redmond::OperatingSystem = REDSTRAIN_BUILD_DEFAULT_OS);
		Linker(const Linker&);
		virtual ~Linker();

		inline redmond::Architecture getTargetArchitecture() {
			return architecture;
		}

		inline redmond::OperatingSystem getTargetOperatingSystem() {
			return targetOS;
		}

		virtual Linkage* newLinkage(const std::string&, Linkage::LinkMode) = 0;
		virtual bool isObjectFile(const std::string&) = 0;
		virtual std::string decorateBinaryFileName(const std::string&, Linkage::LinkMode) = 0;
		virtual std::string getObjectFileFormatName() = 0;

		static redmond::OperatingSystem parseOperatingSystem(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKER_HPP */
