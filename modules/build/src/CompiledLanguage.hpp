#ifndef REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class Compiler;
	class CompilerConfiguration;

	class REDSTRAIN_BUILD_API CompiledLanguage : public Language {

	  private:
		Compiler& compiler;

	  protected:
		virtual CompilerConfiguration& getCompilerConfiguration(const Flavor&, const Component&);

	  public:
		CompiledLanguage(const std::string&, Compiler&);
		CompiledLanguage(const CompiledLanguage&);

		inline Compiler& getCompiler() {
			return compiler;
		}

		inline const Compiler& getCompiler() const {
			return compiler;
		}

		virtual bool acceptsSource(const Flavor&, const Flavor&);
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);
		virtual bool isOneToOne(const Flavor&);
		virtual FileArtifact* getSourceTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, const Flavor&, Component&, BuildArtifactMapper&,
				ManyToOneTransform<FileArtifact>*&, Flavor&, bool&);
		virtual FileArtifact* getHeaderExposeTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, Component&, BuildArtifactMapper&, Flavor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP */
