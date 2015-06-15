#ifndef REDSTRAIN_MOD_BUILD_GENERATION_HPP
#define REDSTRAIN_MOD_BUILD_GENERATION_HPP

#include <list>
#include <redstrain/util/ReferenceCounted.hpp>

namespace redengine {
namespace build {

	class BuildContext;

	template<typename ArtifactT>
	class Generation : public util::ReferenceCounted {

	  public:
		typedef ArtifactT Artifact;

	  public:
		Generation() {}
		Generation(const Generation&) {}
		virtual ~Generation() {}

		virtual void generate(const std::list<ArtifactT*>&, ArtifactT*, BuildContext&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GENERATION_HPP */
