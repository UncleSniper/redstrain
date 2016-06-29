#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_BOUNDEDSYMBOLSINK_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_BOUNDEDSYMBOLSINK_HPP

#include "SymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API BoundedSymbolSink : public virtual SymbolSink {

	  private:
		unsigned columnCount, currentColumn;

	  public:
		BoundedSymbolSink(unsigned);
		BoundedSymbolSink(const BoundedSymbolSink&);

		inline unsigned getColumnCount() const {
			return columnCount;
		}

		inline void setColumnCount(unsigned columnCount) {
			this->columnCount = columnCount;
		}

		inline unsigned getCurrentColumn() const {
			return currentColumn;
		}

		void advanceCurrentColumn(unsigned);
		void resetCurrentColumn();

		virtual unsigned getRemainingColumnCount();

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_BOUNDEDSYMBOLSINK_HPP */
