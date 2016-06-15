#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_DEPENDENTNAMEEXPRESSION_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_DEPENDENTNAMEEXPRESSION_HPP

#include <list>

#include "Expression.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;
	class UnqualifiedName;

	class REDSTRAIN_REDMOND_API DependentNameExpression : public Expression {

	  private:
		typedef std::list<TemplateArgument*> Arguments;

	  public:
		typedef Arguments::const_iterator ArgumentIterator;

	  private:
		Type* type;
		UnqualifiedName* name;
		Arguments arguments;

	  protected:
		virtual void print(std::ostream&, int) const;

	  public:
		DependentNameExpression(Type*, UnqualifiedName*);
		DependentNameExpression(const DependentNameExpression&);
		virtual ~DependentNameExpression();

		inline Type& getType() const {
			return *type;
		}

		inline UnqualifiedName& getName() const {
			return *name;
		}

		bool hasArguments() const;
		unsigned getArgumentCount() const;
		void getArguments(ArgumentIterator&, ArgumentIterator&) const;
		void addArgument(TemplateArgument&);

		virtual ExpressionType getExpressionType() const;
		virtual Expression* cloneExpression() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_DEPENDENTNAMEEXPRESSION_HPP */
