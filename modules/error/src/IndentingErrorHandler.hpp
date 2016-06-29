#ifndef REDSTRAIN_MOD_ERROR_INDENTINGERRORHANDLER_HPP
#define REDSTRAIN_MOD_ERROR_INDENTINGERRORHANDLER_HPP

#include <redstrain/redmond/IndentationChain.hpp>

#include "ErrorHandlerTextBase.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IndentingErrorHandler
			: public virtual ErrorHandlerTextBase, protected redmond::IndentationChain {

	  public:
		class REDSTRAIN_ERROR_API Indenter {

		  public:
			Indenter();
			Indenter(const Indenter&);
			virtual ~Indenter();

			virtual unsigned indent() = 0;
			virtual void endLine() = 0;

		};

	  private:
		Indenter& indenter;
		unsigned currentColumn;

	  protected:
		void advanceCurrentColumn(unsigned);
		void resetCurrentColumn();

		virtual void prepareLine();
		virtual void endLine();
		virtual unsigned indentInherited();

	  public:
		IndentingErrorHandler(Indenter&);
		IndentingErrorHandler(const IndentingErrorHandler&);

		inline Indenter& getIndenter() {
			return indenter;
		}

		inline const Indenter& getIndenter() const {
			return indenter;
		}

		inline unsigned getCurrentColumn() const {
			return currentColumn;
		}

		inline void setCurrentColumn(unsigned column) {
			currentColumn = column;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INDENTINGERRORHANDLER_HPP */
