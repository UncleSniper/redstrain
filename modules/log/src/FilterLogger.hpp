#ifndef REDSTRAIN_MOD_LOG_FILTERLOGGER_HPP
#define REDSTRAIN_MOD_LOG_FILTERLOGGER_HPP

#include "Logger.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT>
	class FilterLogger : public Logger<SeverityT, ComponentT, UnitT, ConcernT> {

	  public:
		typedef Logger<SeverityT, ComponentT, UnitT, ConcernT> Child;

	  private:
		Child& child;
		bool manageChild;

	  protected:
		virtual bool shouldPropagateLogMessage(const SeverityT&, const ComponentT&, const UnitT&,
				const ConcernT&, const text::String16&) = 0;

	  public:
		FilterLogger(Child& child, bool manageChild) : child(child), manageChild(manageChild) {}

		FilterLogger(const FilterLogger& logger) : Logger<SeverityT, ComponentT, UnitT, ConcernT>(logger),
				child(logger.child), manageChild(false) {}

		virtual ~FilterLogger() {
			if(manageChild)
				delete &child;
		}

		inline Child& getChild() {
			return child;
		}

		inline const Child& getChild() const {
			return child;
		}

		inline bool isManageChild() const {
			return manageChild;
		}

		inline void setManageChild(bool manageChild) {
			this->manageChild = manageChild;
		}

		virtual void log(const SeverityT& severity, const ComponentT& component, const UnitT& unit,
				const ConcernT& concern, const text::String16& message) {
			if(shouldPropagateLogMessage(severity, component, unit, concern, message))
				child.log(severity, component, unit, concern, message);
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_FILTERLOGGER_HPP */
