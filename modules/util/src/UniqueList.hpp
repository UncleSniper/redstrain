#ifndef REDSTRAIN_MOD_UTIL_UNIQUELIST_H
#define REDSTRAIN_MOD_UTIL_UNIQUELIST_H

#include <map>
#include <list>
#include <cstddef>

namespace redengine {
namespace util {

	template<typename ElementT>
	class UniqueList {

	  private:
		typedef std::list<ElementT> List;
		typedef std::map<ElementT, typename List::iterator> Map;

	  public:
		typedef typename List::const_iterator Iterator;

	  private:
		struct ListRemover {

			List* list;
			bool back;

			ListRemover(List* list, bool back) : list(list), back(back) {}

			~ListRemover() {
				if(!list)
					return;
				if(back)
					list->pop_back();
				else
					list->pop_front();
			}

		};

	  private:
		List list;
		Map map;

	  public:
		UniqueList() {}
		UniqueList(const UniqueList& list) : list(list.list), map(list.map) {}

		bool append(const ElementT& element) {
			if(map.find(element) != map.end())
				return false;
			list.push_back(element);
			ListRemover remover(&list, true);
			typename List::iterator it = list.end();
			map[element] = --it;
			remover.list = NULL;
			return true;
		}

		bool prepend(const ElementT& element) {
			if(map.find(element) != map.end())
				return false;
			list.push_front(element);
			ListRemover remover(&list, false);
			map[element] = list.begin();
			remover.list = NULL;
			return true;
		}

		bool erase(const ElementT& element) {
			typename Map::iterator it = map.find(element);
			if(it == map.end())
				return false;
			list.erase(it->second);
			map.erase(it);
			return true;
		}

		void clear() {
			list.clear();
			map.clear();
		}

		Iterator begin() const {
			return list.begin();
		}

		Iterator end() const {
			return list.end();
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_UNIQUELIST_H */
