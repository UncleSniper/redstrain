#ifndef REDSTRAIN_HOME_CLOUD_REDSTRAIN_MODULES_ERROR_STDOSTREAMERRORINDENTER_HPP
#define REDSTRAIN_HOME_CLOUD_REDSTRAIN_MODULES_ERROR_STDOSTREAMERRORINDENTER_HPP

#include <iostream>

#include "ErrorIndenterBase8.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StdOStreamErrorIndenter : public ErrorIndenterBase8 {

	  private:
		std::ostream& stream;

	  protected:
		virtual void writeString(const std::string&);

	  public:
		StdOStreamErrorIndenter(std::ostream&);
		StdOStreamErrorIndenter(const StdOStreamErrorIndenter&);

		inline std::ostream& getStream() {
			return stream;
		}

		inline const std::ostream& getStream() const {
			return stream;
		}

		virtual void endLine();

	};

}}

#endif /* REDSTRAIN_HOME_CLOUD_REDSTRAIN_MODULES_ERROR_STDOSTREAMERRORINDENTER_HPP */
