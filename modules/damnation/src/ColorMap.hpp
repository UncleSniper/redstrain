#ifndef REDSTRAIN_MOD_DAMNATION_COLORMAP_HPP
#define REDSTRAIN_MOD_DAMNATION_COLORMAP_HPP

#include "api.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API ColorMap {

	  private:
		unsigned char map256[256], contrast256[256];

	  public:
		ColorMap();
		ColorMap(const ColorMap&);

		unsigned highToLowColor(unsigned) const;
		unsigned contrastTo(unsigned) const;

		void putHighToLow(unsigned, unsigned);
		void putContrast(unsigned, unsigned);
		void resetColors();
		void resetContrast();
		void fixKnownIssues();

		static const ColorMap& getDefaultColorMap();

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_COLORMAP_HPP */
