#ifndef REDSTRAIN_MOD_BUILD_COMPILATION_HPP
#define REDSTRAIN_MOD_BUILD_COMPILATION_HPP

#include <string>

#include "Invocation.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Compilation : public Invocation {

	  public:
		enum REDSTRAIN_BUILD_API CompileMode {
			FOR_STATIC_EXECUTABLE,
			FOR_DYNAMIC_EXECUTABLE,
			FOR_STATIC_LIBRARY,
			FOR_DYNAMIC_LIBRARY
		};

	  private:
		const std::string source;
		const CompileMode mode;
		std::string target;

	  public:
		Compilation(const std::string&, CompileMode);
		Compilation(const Compilation&);

		inline const std::string& getSource() const {
			return source;
		}

		inline CompileMode getCompileMode() const {
			return mode;
		}

		inline const std::string& getTarget() const {
			return target;
		}

		void setTarget(const std::string&);
		void clearTarget();

		virtual void defineMacro(const std::string&) = 0;
		virtual void defineMacro(const std::string&, const std::string&) = 0;
		virtual void addIncludeDirectory(const std::string&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILATION_HPP */
