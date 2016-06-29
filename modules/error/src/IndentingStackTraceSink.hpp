#ifndef REDSTRAIN_MOD_ERROR_INDENTINGSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_ERROR_INDENTINGSTACKTRACESINK_HPP

#include <redstrain/redmond/IndentationChain.hpp>

#include "StackTraceSinkBase.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IndentingStackTraceSink
			: public virtual StackTraceSinkBase, protected redmond::IndentationChain {

	  public:
		class REDSTRAIN_ERROR_API Indenter {

		  private:
			redmond::IndentationChain* chain;

		  protected:
			unsigned indentOwnInherited() const;

		  public:
			Indenter();
			Indenter(const Indenter&);
			virtual ~Indenter();

			inline IndentationChain* getIndentationChain() const {
				return chain;
			}

			inline void setIndentationChain(IndentationChain* chain) {
				this->chain = chain;
			}

			virtual unsigned indent(unsigned) = 0;
			virtual void skip(unsigned) = 0;
			virtual void endLine() = 0;

		};

	  private:
		Indenter& indenter;
		unsigned indentLevel;
		bool withinFrame;
		unsigned currentColumn;

	  protected:
		virtual void beginHeader();
		virtual void endHeader();
		virtual void beginFrame();
		virtual void beginFrameModule(util::MemorySize);
		virtual void endFrameModule();
		virtual void endFrame();
		virtual unsigned indentInherited();

	  public:
		IndentingStackTraceSink(Indenter&, unsigned = 0u);
		IndentingStackTraceSink(const IndentingStackTraceSink&);

		void advanceCurrentColumn(unsigned);
		void resetCurrentColumn();

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

		inline unsigned getCurrentColumn() const {
			return currentColumn;
		}

		inline void setCurrentColumn(unsigned column) {
			currentColumn = column;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INDENTINGSTACKTRACESINK_HPP */
