#ifndef REDSTRAIN_MOD_BUILD_CPPLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_CPPLANGUAGE_HPP

#include "CompiledLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CPPLanguage : public CompiledLanguage {

	  public:
		CPPLanguage(Compiler&);
		CPPLanguage(const CPPLanguage&);

		virtual ArtifactType classifyFile(const std::string&);
		virtual void getReferencedHeaders(const std::string&, util::Appender<ReferencedHeader>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CPPLANGUAGE_HPP */
