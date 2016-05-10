#ifndef REDSTRAIN_MOD_TEXT_NOSUCHCODECERROR_HPP
#define REDSTRAIN_MOD_TEXT_NOSUCHCODECERROR_HPP

#include "TextError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API NoSuchCodecError : public TextError {

	  protected:
		const std::string codec;

	  public:
		NoSuchCodecError(const std::string&);
		NoSuchCodecError(const NoSuchCodecError&);

		inline const std::string& getCodecName() const {
			return codec;
		}

		REDSTRAIN_DECLARE_ERROR(NoSuchCodecError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_NOSUCHCODECERROR_HPP */
