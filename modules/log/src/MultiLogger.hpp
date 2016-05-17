#ifndef REDSTRAIN_MOD_LOG_MULTILOGGER_HPP
#define REDSTRAIN_MOD_LOG_MULTILOGGER_HPP

#include <set>
#include <redstrain/platform/PolicyLocker.hpp>

#include "Logger.hpp"

namespace redengine {
namespace log {

	template<typename SeverityT, typename ComponentT, typename UnitT, typename ConcernT, typename LockingPolicyT>
	class MultiLogger : protected LockingPolicyT, public Logger<SeverityT, ComponentT, UnitT, ConcernT> {

	  public:
		typedef Logger<SeverityT, ComponentT, UnitT, ConcernT> Child;
		typedef std::set<Child*> Children;
		typedef typename Children::const_iterator ChildIterator;

	  protected:
		typedef platform::PolicyLocker<LockingPolicyT> LoggerLocker;

	  private:
		bool manageChildren;
		Children children;

	  public:
		MultiLogger(bool manageChildren) : manageChildren(manageChildren) {}

		MultiLogger(bool manageChildren, typename LockingPolicyT::LockingPolicyInitializer lockInitializer)
				: LockingPolicyT(lockInitializer), manageChildren(manageChildren) {}

		MultiLogger(const MultiLogger& logger) : LockingPolicyT(logger),
				Logger<SeverityT, ComponentT, UnitT, ConcernT>(logger), manageChildren(false) {
			LoggerLocker lock(logger);
			children = logger.children;
			lock.release();
		}

		virtual ~MultiLogger() {
			if(manageChildren) {
				ChildIterator begin(children.begin()), end(children.end());
				for(; begin != end; ++begin)
					delete *begin;
			}
		}

		inline bool isManageChildren() const {
			return manageChildren;
		}

		inline void setManageChildren(bool manageChildren) {
			this->manageChildren = manageChildren;
		}

		void getChildren(ChildIterator& begin, ChildIterator& end) const {
			begin = children.begin();
			end = children.end();
		}

		void getChildren(Children& children) const {
			LoggerLocker lock(this);
			children = this->children;
			lock.release();
		}

		bool addChild(Child& child) {
			LoggerLocker lock(this);
			if(children.find(&child) != children.end()) {
				lock.release();
				return false;
			}
			children.insert(&child);
			lock.release();
			return true;
		}

		bool removeChild(Child& child) {
			LoggerLocker lock(this);
			typename Children::iterator it(children.find(&child));
			if(it == children.end()) {
				lock.release();
				return false;
			}
			children.erase(it);
			lock.release();
			return true;
		}

		void clearChildren() {
			LoggerLocker lock(this);
			if(manageChildren) {
				ChildIterator begin(children.begin()), end(children.end());
				for(; begin != end; ++begin)
					delete *begin;
			}
			children.clear();
			lock.release();
		}

		virtual void log(const SeverityT& severity, const ComponentT& component, const UnitT& unit,
				const ConcernT& concern, const text::String16& message) {
			LoggerLocker lock(this);
			ChildIterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin)
				(*begin)->log(severity, component, unit, concern, message);
			lock.release();
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOG_MULTILOGGER_HPP */
