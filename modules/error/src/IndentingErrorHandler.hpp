#ifndef REDSTRAIN_MOD_ERROR_INDENTINGERRORHANDLER_HPP
#define REDSTRAIN_MOD_ERROR_INDENTINGERRORHANDLER_HPP

#include "ErrorHandlerTextBase.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API IndentingErrorHandler : public virtual ErrorHandlerTextBase {

	  public:
		class REDSTRAIN_ERROR_API Indenter {

		  public:
			Indenter();
			Indenter(const Indenter&);
			virtual ~Indenter();

			virtual void indent() = 0;
			virtual void endLine() = 0;

		};

	  private:
		Indenter& indenter;

	  protected:
		virtual void prepareLine();
		virtual void endLine();

	  public:
		IndentingErrorHandler(Indenter&);
		IndentingErrorHandler(const IndentingErrorHandler&);

		inline Indenter& getIndenter() {
			return indenter;
		}

		inline const Indenter& getIndenter() const {
			return indenter;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INDENTINGERRORHANDLER_HPP */
