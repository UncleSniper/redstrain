#ifndef REDSTRAIN_MOD_BUILD_LANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_LANGUAGE_HPP

#include <string>
#include <redstrain/util/Appender.hpp>

#include "Flavor.hpp"
#include "Component.hpp"
#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"
#include "ManyToOneTransform.hpp"

namespace redengine {
namespace build {

	class BuildArtifactMapper;

	class REDSTRAIN_BUILD_API Language {

	  public:
		enum REDSTRAIN_BUILD_API ArtifactType {
			AT_NONE,
			AT_SOURCE,
			AT_HEADER
		};

		class REDSTRAIN_BUILD_API ReferencedHeader {

		  private:
			const std::string path;
			bool local;

		  public:
			ReferencedHeader(const std::string&, bool);
			ReferencedHeader(const ReferencedHeader&);

			inline const std::string& getPath() const {
				return path;
			}

			inline bool isLocal() const {
				return local;
			}

		};

	  private:
		const std::string name;

	  public:
		Language(const std::string&);
		Language(const Language&);
		virtual ~Language();

		inline const std::string getName() const {
			return name;
		}

		virtual Flavor getShippedSourceFlavor();
		virtual Flavor getShippedHeaderFlavor();
		virtual ArtifactType classifyFile(const std::string&) = 0;
		virtual bool acceptsSource(const Flavor&, const Flavor&) = 0;
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&) = 0;
		virtual bool isOneToOne(const Flavor&) = 0;
		virtual FileArtifact* getSourceTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, const Flavor&, Component&, BuildArtifactMapper&,
				ManyToOneTransform<FileArtifact>*&, Flavor&, bool&) = 0;
		virtual FileArtifact* getHeaderExposeTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, Component&, BuildArtifactMapper&, Flavor&) = 0;
		virtual void getReferencedHeaders(const std::string&, util::Appender<ReferencedHeader>&);

		static ArtifactType classifyFileBySuffix(const std::string&, const char *const*, const char *const*);
		static void sinkSupportedFlavors(const Flavor *const*, util::Appender<Flavor>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LANGUAGE_HPP */
