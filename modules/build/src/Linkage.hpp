#ifndef REDSTRAIN_MOD_BUILD_LINKAGE_HPP
#define REDSTRAIN_MOD_BUILD_LINKAGE_HPP

#include <list>
#include <string>

#include "Invocation.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Linkage : public virtual Invocation {

	  public:
		enum REDSTRAIN_BUILD_API LinkMode {
			STATIC_EXECUTABLE,
			DYNAMIC_EXECUTABLE,
			STATIC_LIBRARY,
			DYNAMIC_LIBRARY
		};

	  private:
		typedef std::list<std::string> Paths;

	  public:
		typedef Paths::const_iterator PathIterator;

	  private:
		Paths sources;
		const std::string target;
		const LinkMode mode;

	  public:
		Linkage(const std::string&, LinkMode);
		Linkage(const Linkage&);

		inline const std::string& getTarget() const {
			return target;
		}

		inline LinkMode getLinkMode() const {
			return mode;
		}

		void addSource(const std::string&);
		void clearSources();
		void getSources(PathIterator&, PathIterator&) const;

		virtual void addLibraryDirectory(const std::string&) = 0;
		virtual void addLibrary(const std::string&) = 0;

		static const char* getLinkModeName(LinkMode);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKAGE_HPP */
