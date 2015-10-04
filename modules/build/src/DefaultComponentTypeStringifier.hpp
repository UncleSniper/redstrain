#ifndef REDSTRAIN_MOD_BUILD_DEFAULTCOMPONENTTYPESTRINGIFIER_HPP
#define REDSTRAIN_MOD_BUILD_DEFAULTCOMPONENTTYPESTRINGIFIER_HPP

#include "ComponentTypeStringifier.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DefaultComponentTypeStringifier : public ComponentTypeStringifier {

	  public:
		static const char *const DEFAULT_LIBRARY_TAG;
		static const char *const DEFAULT_EXECUTABLE_TAG;
		static const char *const DEFAULT_DATA_TAG;
		static const char *const DEFAULT_BLOB_TAG;

	  private:
		std::string libraryTag, executableTag, dataTag, blobTag;

	  public:
		DefaultComponentTypeStringifier();
		DefaultComponentTypeStringifier(const DefaultComponentTypeStringifier&);
		virtual ~DefaultComponentTypeStringifier();

		inline const std::string& getLibraryTag() const {
			return libraryTag;
		}

		void setLibraryTag(const std::string&);

		inline const std::string& getExecutableTag() const {
			return executableTag;
		}

		void setExecutableTag(const std::string&);

		inline const std::string& getDataTag() const {
			return dataTag;
		}

		void setDataTag(const std::string&);

		inline const std::string& getBlobTag() const {
			return blobTag;
		}

		void setBlobTag(const std::string&);

		virtual std::string stringifyComponentType(Component::Type);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEFAULTCOMPONENTTYPESTRINGIFIER_HPP */
