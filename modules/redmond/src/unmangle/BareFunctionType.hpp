#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_BAREFUNCTIONTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_BAREFUNCTIONTYPE_HPP

#include <list>

#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API BareFunctionType {

	  private:
		typedef std::list<Type*> Types;

	  public:
		typedef Types::const_iterator TypeIterator;

	  private:
		Types types;

	  public:
		BareFunctionType();
		BareFunctionType(const BareFunctionType&);
		~BareFunctionType();

		unsigned getTypeCount() const;
		void getTypes(TypeIterator&, TypeIterator&) const;
		Type* getFirstType() const;
		void getRestTypes(TypeIterator&, TypeIterator&) const;
		void addType(Type&);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_BAREFUNCTIONTYPE_HPP */
