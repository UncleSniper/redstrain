#include <redstrain/error/ProgrammingError.hpp>

#include "../Size.hpp"
#include "StageConstraints.hpp"

using redengine::error::ProgrammingError;

namespace redengine {
namespace damnation {
namespace tk {

	// ======== Edge ========

	StageConstraints::Edge::Edge(unsigned origin, unsigned size) : origin(origin), size(size) {}

	StageConstraints::Edge::Edge(const Edge& edge) : origin(edge.origin), size(edge.size) {}

	// ======== StageConstraints ========

	StageConstraints::StageConstraints()
			: topAbsolute(StageConstraints::RELATIVE), topRelative(0.5), topDirection(CENTER),
			leftAbsolute(StageConstraints::RELATIVE), leftRelative(0.5), leftDirection(CENTER),
			widthExtent(SIZE), widthAbsolute(StageConstraints::RELATIVE), widthRelative(0.8),
			heightExtent(SIZE), heightAbsolute(StageConstraints::RELATIVE), heightRelative(0.8) {}

	StageConstraints::StageConstraints(const StageConstraints& constraints)
			: topAbsolute(constraints.topAbsolute), topRelative(constraints.topRelative),
			topDirection(constraints.topDirection),
			leftAbsolute(constraints.leftAbsolute), leftRelative(constraints.leftRelative),
			leftDirection(constraints.leftDirection),
			widthExtent(constraints.widthExtent), widthAbsolute(constraints.widthAbsolute),
			widthRelative(constraints.widthRelative),
			heightExtent(constraints.heightExtent), heightAbsolute(constraints.heightAbsolute),
			heightRelative(constraints.heightRelative) {}

	int StageConstraints::encodeSpec(Direction direction, Extent extent,
			unsigned originAbsolute, unsigned sizeAbsolute) {
		// encoding: ORIGIN/CENTER, SIZE/CORNER, originAbsolute/originRelative, sizeAbsolute/sizeRelative
		return
			(direction == ORIGIN ? 0 : 1000)
			+ (extent == CORNER ? 100 : 0)
			+ (originAbsolute == StageConstraints::RELATIVE ? 10 : 0)
			+ (sizeAbsolute == StageConstraints::RELATIVE ? 1 : 0);
	}

	StageConstraints& StageConstraints::operator=(const StageConstraints& constraints) {
		topAbsolute = constraints.topAbsolute;
		topRelative = constraints.topRelative;
		topDirection = constraints.topDirection;
		leftAbsolute = constraints.leftAbsolute;
		leftRelative = constraints.leftRelative;
		leftDirection = constraints.leftDirection;
		widthExtent = constraints.widthExtent;
		widthAbsolute = constraints.widthAbsolute;
		widthRelative = constraints.widthRelative;
		heightExtent = constraints.heightExtent;
		heightAbsolute = constraints.heightAbsolute;
		heightRelative = constraints.heightRelative;
		return *this;
	}

	Rectangle StageConstraints::getChildSize(const Size& containerSize) const {
		const Edge vertical(StageConstraints::getChildEdge(containerSize.height, topDirection, heightExtent,
				topAbsolute, topRelative, heightAbsolute, heightRelative));
		const Edge horizontal(StageConstraints::getChildEdge(containerSize.width, leftDirection, widthExtent,
				leftAbsolute, leftRelative, widthAbsolute, widthRelative));
		Rectangle rectangle(static_cast<int>(vertical.origin), static_cast<int>(horizontal.origin),
				horizontal.size, vertical.size);
		rectangle.constrainInto(containerSize);
		return rectangle;
	}

	StageConstraints::Edge StageConstraints::getChildEdge(unsigned container, Direction direction, Extent extent,
			unsigned originAbsolute, double originRelative, unsigned sizeAbsolute, double sizeRelative) {
		// container = pre + child + post
		unsigned pre, child, post, delta;
		switch(StageConstraints::encodeSpec(direction, extent, originAbsolute, sizeAbsolute)) {
			// ORIGIN, SIZE
			case StageConstraints::OSaa:
				// pre = originAbsolute
				// child = sizeAbsolute
				return Edge(originAbsolute, sizeAbsolute);
			case StageConstraints::OSar:
				// pre = originAbsolute
				// child = sizeRelative * container
				child = static_cast<unsigned>(sizeRelative * static_cast<double>(container));
				return Edge(originAbsolute, child);
			case StageConstraints::OSra:
				// pre = originRelative * (container - 1)
				// child = sizeAbsolute
				pre = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				return Edge(pre, sizeAbsolute);
			case StageConstraints::OSrr:
				// pre = originRelative * (container - 1)
				// child = sizeRelative * container
				pre = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				child = static_cast<unsigned>(sizeRelative * static_cast<double>(container));
				return Edge(pre, child);
			// ORIGIN, CORNER
			// child = container - pre - post
			case StageConstraints::OCaa:
				// pre = originAbsolute
				// post = sizeAbsolute
				delta = originAbsolute + sizeAbsolute;
				return Edge(originAbsolute, container > delta ? container - delta : 0u);
			case StageConstraints::OCar:
				// pre = originAbsolute
				// post = sizeRelative * (container - 1)
				post = static_cast<unsigned>(sizeRelative * static_cast<double>(container ? container - 1u : 0u));
				delta = originAbsolute + post;
				return Edge(originAbsolute, container > delta ? container - delta : 0u);
			case StageConstraints::OCra:
				// pre = originRelative * (container - 1)
				// post = sizeAbsolute
				pre = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				delta = pre + sizeAbsolute;
				return Edge(pre, container > delta ? container - delta : 0u);
			case StageConstraints::OCrr:
				// pre = originRelative * (container - 1)
				// post = sizeRelative * (container - 1)
				pre = static_cast<int>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				post = static_cast<int>(sizeRelative * static_cast<double>(container ? container - 1u : 0u));
				delta = pre + post;
				return Edge(pre, container > delta ? container - delta : 0u);
			// CENTER, SIZE
			case StageConstraints::CSaa:
				// child = sizeAbsolute
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				delta = sizeAbsolute / 2u;
				return Edge(originAbsolute > delta ? originAbsolute - delta : 0u, sizeAbsolute);
			case StageConstraints::CSar:
				// child = sizeRelative * container
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				child = static_cast<unsigned>(sizeRelative * static_cast<double>(container));
				delta = child / 2u;
				return Edge(originAbsolute > delta ? originAbsolute - delta : 0u, child);
			case StageConstraints::CSra:
				// child = sizeAbsolute
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				pre = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				delta = sizeAbsolute / 2u;
				return Edge(pre > delta ? pre - delta : 0u, sizeAbsolute);
			case StageConstraints::CSrr:
				// child = sizeRelative * container
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				child = static_cast<unsigned>(sizeRelative * static_cast<double>(container));
				pre = static_cast<unsigned>(originRelative
						* static_cast<double>(container ? container - 1u : container));
				delta = child / 2u;
				return Edge(pre > delta ? pre - delta : 0u, child);
			// CENTER, CORNER
			// child = container - pre - post
			case StageConstraints::CCaa:
				// post = sizeAbsolute
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				// => child = container - (originAbsolute - child / 2) - sizeAbsolute
				// => child = container - originAbsolute + child / 2 - sizeAbsolute
				// => child / 2 = container - originAbsolute - sizeAbsolute
				delta = originAbsolute + sizeAbsolute;
				child = 2u * (container > delta ? container - delta : 0u);
				delta = child / 2u;
				return Edge(originAbsolute > delta ? originAbsolute - delta : 0u, child);
			case StageConstraints::CCar:
				// post = sizeRelative * (container - 1)
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				// => child = container - (originAbsolute - child / 2) - sizeRelative * (container - 1)
				// => child = container - originAbsolute + child / 2 - sizeRelative * (container - 1)
				// => child / 2 = container - originAbsolute - sizeRelative * (container - 1)
				delta = originAbsolute + static_cast<unsigned>(sizeRelative
						* static_cast<double>(container ? container - 1u : 0u));
				child = 2u * (container > delta ? container - delta : 0u);
				delta = child / 2u;
				return Edge(originAbsolute > delta ? originAbsolute - delta : 0u, child);
			case StageConstraints::CCra:
				// post = sizeAbsolute
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				// => child = container - (originRelative * (container - 1) - child / 2) - sizeAbsolute
				// => child = container - originRelative * (container - 1) + child / 2 - sizeAbsolute
				// => child / 2 = container - originRelative * (container - 1) - sizeAbsolute
				delta = static_cast<unsigned>(originRelative
						* static_cast<double>(container ? container - 1u : 0u)) + sizeAbsolute;
				child = 2u * (container > delta ? container - delta : 0u);
				delta = child / 2u;
				pre = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				return Edge(pre > delta ? pre - delta : 0u, child);
			case StageConstraints::CCrr:
				// post = sizeRelative * (container - 1)
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				// => child = container - (originRelative * (container - 1) - child / 2)
				//            - sizeRelative * (container - 1)
				// => child = container - originRelative * (container - 1) + child / 2
				//            - sizeRelative * (container - 1)
				// => child / 2 = container - originRelative * (container - 1) - sizeRelative * (container - 1)
				delta = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u))
						+ static_cast<unsigned>(sizeRelative * static_cast<double>(container ? container - 1u : 0u));
				child = 2u * (container > delta ? container - delta : 0u);
				delta = child / 2u;
				pre = static_cast<unsigned>(originRelative * static_cast<double>(container ? container - 1u : 0u));
				return Edge(pre > delta ? pre - delta : 0u, child);
			default:
				throw ProgrammingError("Unrecognized spec encoding in StageConstraints::getChildEdge()");
		}
	}

	Size StageConstraints::getEnclosingSize(const Size& size, bool minimal) const {
		return Size(
			StageConstraints::getEnclosingEdge(size.width, leftDirection, widthExtent,
					leftAbsolute, leftRelative, widthAbsolute, widthRelative, minimal),
			StageConstraints::getEnclosingEdge(size.height, topDirection, heightExtent,
					topAbsolute, topRelative, heightAbsolute, heightRelative, minimal)
		);
	}

	unsigned StageConstraints::getEnclosingEdge(unsigned child, Direction direction, Extent extent,
			unsigned originAbsolute, double originRelative, unsigned sizeAbsolute, double sizeRelative,
			bool minimal) {
		// container = pre + child + post
		double rel;
		unsigned delta;
		switch(StageConstraints::encodeSpec(direction, extent, originAbsolute, sizeAbsolute)) {
			// ORIGIN, SIZE
			case StageConstraints::OSaa:
				// pre = originAbsolute
				// child = sizeAbsolute
				return originAbsolute + sizeAbsolute + (minimal ? 0u : originAbsolute);
			case StageConstraints::OSar:
				// pre = originAbsolute
				// child = sizeRelative * container
				return sizeRelative <= 0.0 ? originAbsolute
						: static_cast<unsigned>(static_cast<double>(child) / sizeRelative);
			case StageConstraints::OSra:
				// pre = originRelative * (container - 1)
				// child = sizeAbsolute
				// minimal:
				//   => container = originRelative * (container - 1) + sizeAbsolute
				//   => container = originRelative * container - originRelative + sizeAbsolute
				//   => (1 - originRelative) * container = -originRelative + sizeAbsolute
				//   => container = (sizeAbsolute - originRelative) / (1 - originRelative)
				// otherwise:
				//   => container = 2 * originRelative * (container - 1) + sizeAbsolute
				//   => container = 2 * originRelative * container - 2 * originRelative + sizeAbsolute
				//   => (1 - 2 * originRelative) * container = -2 * originRelative + sizeAbsolute
				//   => container = (sizeAbsolute - 2 * originRelative) / (1 - 2 * originRelative);
				rel = minimal ? originRelative : 2.0 * originRelative;
				return static_cast<unsigned>((static_cast<double>(sizeAbsolute) - rel) / (1.0 - rel));
			case StageConstraints::OSrr:
				// pre = originRelative * (container - 1)
				// child = sizeRelative * container
				return sizeRelative <= 0.0 ? 0u : static_cast<unsigned>(static_cast<double>(child) / sizeRelative);
			// ORIGIN, CORNER
			// child = container - pre - post
			case StageConstraints::OCaa:
				// pre = originAbsolute
				// post = sizeAbsolute
				return originAbsolute + child + sizeAbsolute;
			case StageConstraints::OCar:
				// pre = originAbsolute
				// post = sizeRelative * (container - 1)
				// => container = originAbsolute + child + sizeRelative * (container - 1)
				// => container = originAbsolute + child + sizeRelative * container - sizeRelative
				// => (1 - sizeRelative) * container = originAbsolute + child - sizeRelative
				// => container = (originAbsolute + child - sizeRelative) / (1 - sizeRelative)
				return static_cast<unsigned>((static_cast<double>(originAbsolute + child) - sizeRelative)
						/ (1.0 - sizeRelative));
			case StageConstraints::OCra:
				// pre = originRelative * (container - 1)
				// post = sizeAbsolute
				// => container = originRelative * (container - 1) + child + sizeAbsolute
				// => container = originRelative * container - originRelative + child + sizeAbsolute
				// => (1 - originRelative) * container = -originRelative + child + sizeAbsolute
				// => container = (child + sizeAbsolute - originRelative) / (1 - originRelative)
				return static_cast<unsigned>((static_cast<double>(child + sizeAbsolute) - originRelative)
						/ (1.0 - originRelative));
			case StageConstraints::OCrr:
				// pre = originRelative * (container - 1)
				// post = sizeRelative * (container - 1)
				// => container = originRelative * (container - 1) + child + sizeRelative * (container - 1)
				// => container = originRelative * container - originRelative + child
				//                + sizeRelative * container - sizeRelative
				// => (1 - originRelative - sizeRelative) * container = -originRelative + child - sizeRelative
				// => container = (child - sizeRelative - originRelative) / (1 - originRelative - sizeRelative)
				return static_cast<unsigned>((static_cast<double>(child) - sizeRelative - originRelative)
						/ (1.0 - originRelative - sizeRelative));
			// CENTER, SIZE
			case StageConstraints::CSaa:
				// child = sizeAbsolute
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				delta = child / 2u;
				return (minimal ? 1u : 2u) * (originAbsolute > delta ? originAbsolute - delta : 0u) + child;
			case StageConstraints::CSar:
				// child = sizeRelative * container
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				delta = child / 2u;
				return sizeRelative <= 0.0 ? (originAbsolute > delta ? originAbsolute - delta : 0u)
						: static_cast<unsigned>(static_cast<double>(child) / sizeRelative);
			case StageConstraints::CSra:
				// child = sizeAbsolute
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				// minimal:
				//   => container = originRelative * (container - 1) - sizeAbsolute / 2 + sizeAbsolute
				//   => container = originRelative * container - originRelative + sizeAbsolute / 2
				//   => (1 - originRelative) * container = sizeAbsolute / 2 - originRelative
				//   => container = (sizeAbsolute / 2 - originRelative) / (1 - originRelative)
				// otherwise:
				//   => container = 2 * originRelative * (container - 1) - sizeAbsolute / 2 + sizeAbsolute
				//   => container = 2 * originRelative * container - 2 * originRelative + sizeAbsolute / 2
				//   => (1 - 2 * originRelative) * container = -2 * originRelative + sizeAbsolute / 2
				//   => container = (sizeAbsolute / 2 - 2 * originRelative) / (1 - 2 * originRelative)
				rel = minimal ? originRelative : 2.0 * originRelative;
				return static_cast<unsigned>((static_cast<double>(sizeAbsolute) / 2.0 - rel) / (1.0 - rel));
			case StageConstraints::CSrr:
				// child = sizeRelative * container
				// pre + child / 2 = originRelative * (container - 1)
				return sizeRelative <= 0.0 ? 0u : static_cast<unsigned>(static_cast<double>(child) / sizeRelative);
			// CENTER, CORNER
			// child = container - pre - post
			case StageConstraints::CCaa:
				// post = sizeAbsolute
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				delta = child / 2u;
				return (originAbsolute > delta ? originAbsolute - delta : 0u) + child + sizeAbsolute;
			case StageConstraints::CCar:
				// post = sizeRelative * (container - 1)
				// pre + child / 2 = originAbsolute
				// => pre = originAbsolute - child / 2
				// => container = originAbsolute - child / 2 + child + sizeRelative * (container - 1)
				// => container = originAbsolute + child / 2 + sizeRelative * container - sizeRelative
				// => (1 - sizeRelative) * container = originAbsolute + child / 2 - sizeRelative
				// => container = (originAbsolute + child / 2 - sizeRelative) / (1 - sizeRelative)
				return static_cast<unsigned>((static_cast<double>(originAbsolute + child / 2u) - sizeRelative)
						/ (1.0 - sizeRelative));
			case StageConstraints::CCra:
				// post = sizeAbsolute
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				// => container = originRelative * (container - 1) - child / 2 + child + sizeAbsolute
				// => container = originRelative * container - originRelative + child / 2 + sizeAbsolute
				// => (1 - originRelative) * container = child / 2 + sizeAbsolute - originRelative
				// => container = (child / 2 + sizeAbsolute - originRelative) / (1 - originRelative)
				return static_cast<unsigned>((static_cast<double>(child / 2u + sizeAbsolute) - originRelative)
						/ (1.0 - originRelative));
			case StageConstraints::CCrr:
				// post = sizeRelative * (container - 1)
				// pre + child / 2 = originRelative * (container - 1)
				// => pre = originRelative * (container - 1) - child / 2
				// => container = originRelative * (container - 1) - child / 2 + child
				//                + sizeRelative * (container - 1)
				// => container = originRelative * container - originRelative + child / 2
				//                + sizeRelative * container - sizeRelative
				// => (1 - originRelative - sizeRelative) * container = -originRelative + child / 2 - sizeRelative
				// => container = (child / 2 - originRelative - sizeRelative) / (1 - originRelative - sizeRelative)
				return static_cast<unsigned>((static_cast<double>(child / 2u) - originRelative - sizeRelative)
						/ (1.0 - originRelative - sizeRelative));
			default:
				throw ProgrammingError("Unrecognized spec encoding in StageConstraints::getEnclosingEdge()");
		}
	}

}}}
