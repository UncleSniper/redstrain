#ifndef REDSTRAIN_MOD_LOG_CONSTANTLOGFILTER_HPP
#define REDSTRAIN_MOD_LOG_CONSTANTLOGFILTER_HPP

#include "LogFilter.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class ConstantLogFilter : public LogFilter<SeverityT, ComponentT, UnitT, ConcernT> {

	  private:
		typedef LogFilter<SeverityT, ComponentT, UnitT, ConcernT> Base;
		typedef typename Base::Message Message;

	  private:
		bool constantResult;

	  public:
		ConstantLogFilter(bool constantResult) : constantResult(constantResult) {}

		ConstantLogFilter(const ConstantLogFilter& filter)
				: LogFilter<SeverityT, ComponentT, UnitT, ConcernT>(filter),
				constantResult(filter.constantResult) {}

		inline bool getConstantResult() const {
			return constantResult;
		}

		inline void setConstantResult(bool constantResult) {
			this->constantResult = constantResult;
		}

		virtual bool shouldLogMessage(const Message&) {
			return constantResult;
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_CONSTANTLOGFILTER_HPP */
