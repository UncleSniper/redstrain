#ifndef REDSTRAIN_MOD_ERROR_INDENTINGSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_ERROR_INDENTINGSTACKTRACESINK_HPP

#include "StackTraceSinkBase.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IndentingStackTraceSink : public virtual StackTraceSinkBase {

	  public:
		class REDSTRAIN_ERROR_API Indenter {

		  public:
			Indenter();
			Indenter(const Indenter&);
			virtual ~Indenter();

			virtual unsigned indent(unsigned) = 0;
			virtual void skip(unsigned) = 0;
			virtual void endLine() = 0;

		};

	  protected:
		virtual void beginHeader();
		virtual void endHeader();
		virtual void beginFrame();
		virtual void beginFrameModule(util::MemorySize);
		virtual void endFrameModule();
		virtual void endFrame();

	  private:
		Indenter& indenter;
		unsigned indentLevel;

	  public:
		IndentingStackTraceSink(Indenter&, unsigned = 0u);
		IndentingStackTraceSink(const IndentingStackTraceSink&);

		inline Indenter& getIndenter() {
			return indenter;
		}

		inline const Indenter& getIndenter() const {
			return indenter;
		}

		inline unsigned getIndentLevel() const {
			return indentLevel;
		}

		inline void setIndentLevel(unsigned indentLevel) {
			this->indentLevel = indentLevel;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INDENTINGSTACKTRACESINK_HPP */
