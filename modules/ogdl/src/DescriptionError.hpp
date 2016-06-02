#ifndef REDSTRAIN_MOD_OGDL_DESCRIPTIONERROR_HPP
#define REDSTRAIN_MOD_OGDL_DESCRIPTIONERROR_HPP

#include <redstrain/text/types.hpp>

#include "OGDLError.hpp"

namespace redengine {
namespace ogdl {

	class REDSTRAIN_OGDL_API DescriptionError : public OGDLError {

	  private:
		const text::String16 file;
		const unsigned line, column;

	  public:
		DescriptionError(const text::String16&, unsigned, unsigned);
		DescriptionError(const DescriptionError&);
		virtual ~DescriptionError();

		inline const text::String16& getFile() const {
			return file;
		}

		inline unsigned getLine() const {
			return line;
		}

		inline unsigned getColumn() const {
			return column;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(DescriptionError)

	};

}}

#endif /* REDSTRAIN_MOD_OGDL_DESCRIPTIONERROR_HPP */
