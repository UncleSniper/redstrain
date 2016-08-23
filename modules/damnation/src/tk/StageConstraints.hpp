#ifndef REDSTRAIN_MOD_DAMNATION_TK_STAGECONSTRAINTS_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_STAGECONSTRAINTS_HPP

#include "../Rectangle.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API StageConstraints {

	  public:
		enum REDSTRAIN_DAMNATION_API Direction {
			ORIGIN,
			CENTER
		};

		enum REDSTRAIN_DAMNATION_API Extent {
			SIZE,
			CORNER
		};

	  private:
		class REDSTRAIN_DAMNATION_API Edge {

		  public:
			const unsigned origin, size;

		  public:
			Edge(unsigned, unsigned);
			Edge(const Edge&);

		};

	  public:
		static const unsigned RELATIVE = ~0u;

	  private:
		static int encodeSpec(Direction, Extent, unsigned, unsigned);
		static Edge getChildEdge(unsigned, Direction, Extent, unsigned, double, unsigned, double);
		static unsigned getEnclosingEdge(unsigned, Direction, Extent, unsigned, double, unsigned, double, bool);

	  private:
		static const int OSaa =    0;
		static const int OSar =    1;
		static const int OSra =   10;
		static const int OSrr =   11;
		static const int OCaa =  100;
		static const int OCar =  101;
		static const int OCra =  110;
		static const int OCrr =  111;
		static const int CSaa = 1000;
		static const int CSar = 1001;
		static const int CSra = 1010;
		static const int CSrr = 1011;
		static const int CCaa = 1100;
		static const int CCar = 1101;
		static const int CCra = 1110;
		static const int CCrr = 1111;

	  public:
		unsigned topAbsolute;
		double topRelative;
		Direction topDirection;
		unsigned leftAbsolute;
		double leftRelative;
		Direction leftDirection;
		Extent widthExtent;
		unsigned widthAbsolute;
		double widthRelative;
		Extent heightExtent;
		unsigned heightAbsolute;
		double heightRelative;

	  public:
		StageConstraints();
		StageConstraints(const StageConstraints&);

		StageConstraints& operator=(const StageConstraints&);

		Rectangle getChildSize(const Size&) const;
		Size getEnclosingSize(const Size&, bool) const;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_STAGECONSTRAINTS_HPP */
