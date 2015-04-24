#ifndef REDSTRAIN_MOD_UTIL_ADDRESSSPACE_HPP
#define REDSTRAIN_MOD_UTIL_ADDRESSSPACE_HPP

#include "IntegerBounds.hpp"

/**
 * @file
 * Definition of the @link redengine::util::AddressSpace integer typecast
 * safety checker @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Integer typecast safety checker. When converting between integer
	 * types, numerical precision may be lost; e.g. converting @c -1
	 * to @c unsigned will underrun @c unsigned and converting @c 200
	 * to @c char will overrun @c char. AddressSpace provides a mechanism
	 * for checking at runtime whether such a conversion will lose precision
	 * in such a manner, i.e. whether there exists no instance of the
	 * target type that has the same sign and the same significant mantissa
	 * digits as the source value. If such an instance exists (i.e. lossless
	 * conversion is possible), the source value is said not to @e exceed
	 * the target type. Conversely, if no such instance exists (i.e.
	 * conversion will lose precision), the source value is said to
	 * @e exceed the target type.
	 *
	 * The target type of the conversion is called the <em>address space
	 * type</em> (or simply <em>space type</em>) and the range of values
	 * instances of that type may take is called the <em>address space</em>.
	 * In the first example above @c unsigned is the space type; in the
	 * second example, @c char is the space type. The source type of the
	 * conversion is called the <em>address type</em> and any instance
	 * of that type is called an @e address. In both examples above,
	 * @c int is the address type. Note that this is merely terminology;
	 * the meaning a client assigns to the types and their instances is
	 * arbitrary. However, this class works under the assumption that
	 * <b>both types involved are primitive integer types</b>.
	 *
	 * The check as to whether a particular address exceeds the given
	 * address type (and thereby its address space) is performed by
	 * the #exceededBy function. The space and address types are
	 * given to the class as template parameters:
	 * @code
	   AddressSpace<unsigned, int>::exceededBy(-1), // true: -1 exceeds unsigned
	   AddressSpace<unsigned, int>::exceededBy(0);  // false: 0 does not exceed unsigned
	 * @endcode
	 *
	 * @tparam SpaceT the address space type
	 * @tparam AddressT the address type
	 * @tparconstr
	 * - all constraints applying to IntegerBounds with <tt>IntegerT = SpaceT</tt>
	 * - all constraints applying to IntegerBounds with <tt>IntegerT = AddressT</tt>
	 *
	 * @since 0.1.0
	 */
	template<typename SpaceT, typename AddressT>
	class AddressSpace {

	  public:
		/**
		 * Type width relationship between address space and address types.
		 * Given an address space arising from the value range of an
		 * integral data type (the <em>space type</em>) and an address into
		 * that address space as an instance of an integral <em>address
		 * type</em>, there arises an indication of whether or not it is
		 * possible to exceed the address space, i.e. whether there exist
		 * such addresses whose numerical value can be represented by the
		 * address type but not by the space type. The most significant such
		 * indication is given by comparing the allocation sizes (<em>type
		 * widths</em>) of the two data types involved.
		 *
		 * @since 0.1.0
		 */
		enum AddressSpaceWidth {
			/**
			 * Address type is wider than space type. That is, the allocation
			 * size of the address type is at least one byte larger than that
			 * of the space type, meaning that <tt>sizeof(SpaceT) &lt;
			 * sizeof(AddressT)</tt>. Consequently, addresses are definitely
			 * capable of exceeding the address space, regardless of the
			 * signedness of the types.
			 *
			 * @since 0.1.0
			 */
			ASW_ADDRESS_IS_WIDER,
			/**
			 * Space type is wider than address type. That is, the allocation
			 * size of the space type is at least one byte larger than that
			 * of the address type, meaning that <tt>sizeof(SpaceT) &gt;
			 * sizeof(AddressT)</tt>. Consequently, addresses are likely
			 * incapable of exceeding the address space, unless the space type
			 * is unsigned, but the address type is signed
			 * (<tt>@ref AddressSpaceSignedness
			 * "ASS_ADDRESS_IS_SIGNED_BUT_SPACE_IS_UNSIGNED"</tt>), in which case
			 * a negative address may still exceed the address space.
			 *
			 * @since 0.1.0
			 */
			ASW_SPACE_IS_WIDER,
			/**
			 * Address type and space type are the same width. That is, the
			 * allocation size of the address and space types are equal, meaning
			 * that <tt>sizeof(SpaceT) == sizeof(AddressT)</tt>. Consequently,
			 * addresses may or may not be capable of exceeding the address
			 * space, depending on the signedness of the types.
			 *
			 * @since 0.1.0
			 */
			ASW_ADDRESS_AND_SPACE_ARE_SAME_WIDTH
		};

		/**
		 * Signedness relationship between address space and address types.
		 * Given an address space arising from the value range of an
		 * integral data type (the <em>space type</em>) and an address into
		 * that address space as an instance of an integral <em>address
		 * type</em>, there arises an indication of whether or not it is
		 * possible to exceed the address space, i.e. whether there exist
		 * such addresses whose numerical value can be represented by the
		 * address type but not by the space type. One such indication is
		 * given by comparing the signedness of the two data types involved,
		 * although this indicator is less significant than the
		 * @ref AddressSpaceWidth "width relationship" between the types.
		 *
		 * @since 0.1.0
		 */
		enum AddressSpaceSignedness {
			/**
			 * Both address and space types are signed.
			 *
			 * @since 0.1.0
			 */
			ASS_ADDRESS_AND_SPACE_ARE_UNSIGNED,
			/**
			 * Address type is unsigned, space type is signed.
			 *
			 * @since 0.1.0
			 */
			ASS_ADDRESS_IS_UNSIGNED_BUT_SPACE_IS_SIGNED,
			/**
			 * Address type is signed, space type is unsigned.
			 *
			 * @since 0.1.0
			 */
			ASS_ADDRESS_IS_SIGNED_BUT_SPACE_IS_UNSIGNED,
			/**
			 * Both address and space types are unsigned.
			 *
			 * @since 0.1.0
			 */
			ASS_ADDRESS_AND_SPACE_ARE_SIGNED
		};

	  private:
		template<
			typename ImplSpaceT,
			typename ImplAddressT,
			AddressSpaceWidth WidthRelationship,
			AddressSpaceSignedness SignednessRelationship
		>
		struct AddressSpaceImpl {

			static inline bool exceededBy(ImplAddressT address) {
				return ImplSpaceT::addressSpaceExcessCaseNotCovered(address);
			}

		};

	#define REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(widthRel, signednessRel, param) \
		template< \
			typename ImplSpaceT, \
			typename ImplAddressT \
		> \
		struct AddressSpaceImpl<ImplSpaceT, ImplAddressT, widthRel, signednessRel> { \
			static inline bool exceededBy(param) { \
				return
	#define REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END \
				; \
			} \
		};

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_IS_WIDER,
			ASS_ADDRESS_AND_SPACE_ARE_UNSIGNED,
			ImplAddressT address
		)
			// address might exceed space width, but only upwards, as both are unsigned
			address > static_cast<AddressT>(IntegerBounds<SpaceT>::MAX)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_IS_WIDER,
			ASS_ADDRESS_IS_UNSIGNED_BUT_SPACE_IS_SIGNED,
			ImplAddressT address
		)
			// address might exceed space width or overrun into negative space
			// by exceeding space mantissa (which is exceeding width, too)
			address > static_cast<AddressT>(IntegerBounds<SpaceT>::MAX)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_IS_WIDER,
			ASS_ADDRESS_IS_SIGNED_BUT_SPACE_IS_UNSIGNED,
			ImplAddressT address
		)
			// address might be negative or exceed space width
			address < static_cast<AddressT>(0)
			// even with the additional mantissa bit, the bound will fit into
			// an AddressT, as address is as least 8 bits wider than space
			|| address > static_cast<AddressT>(IntegerBounds<SpaceT>::MAX)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_IS_WIDER,
			ASS_ADDRESS_AND_SPACE_ARE_SIGNED,
			ImplAddressT address
		)
			// address might exceed space width in either direction
			address < static_cast<AddressT>(IntegerBounds<SpaceT>::MIN)
			|| address > static_cast<AddressT>(IntegerBounds<SpaceT>::MAX)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_SPACE_IS_WIDER,
			ASS_ADDRESS_AND_SPACE_ARE_UNSIGNED,
			ImplAddressT
		)
			false
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_SPACE_IS_WIDER,
			ASS_ADDRESS_IS_UNSIGNED_BUT_SPACE_IS_SIGNED,
			ImplAddressT
		)
			// even with the additional mantissa bit, address cannot
			// overrun space, as address is at least 8 bits wider that space;
			// being unsigned, address cannot underrun space, either
			false
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_SPACE_IS_WIDER,
			ASS_ADDRESS_IS_SIGNED_BUT_SPACE_IS_UNSIGNED,
			ImplAddressT address
		)
			address < static_cast<AddressT>(0)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_SPACE_IS_WIDER,
			ASS_ADDRESS_AND_SPACE_ARE_SIGNED,
			ImplAddressT
		)
			false
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_AND_SPACE_ARE_SAME_WIDTH,
			ASS_ADDRESS_AND_SPACE_ARE_UNSIGNED,
			ImplAddressT
		)
			false
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_AND_SPACE_ARE_SAME_WIDTH,
			ASS_ADDRESS_IS_UNSIGNED_BUT_SPACE_IS_SIGNED,
			ImplAddressT address
		)
			// address might overrun space into the sign bit;
			// with the additional mantissa bit, the upper space bound
			// will fit into an AddressT
			address > static_cast<AddressT>(IntegerBounds<SpaceT>::MAX)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_AND_SPACE_ARE_SAME_WIDTH,
			ASS_ADDRESS_IS_SIGNED_BUT_SPACE_IS_UNSIGNED,
			ImplAddressT address
		)
			// address cannot overrun space, as space has one mantissa bit more,
			// but address might be negative
			address < static_cast<AddressT>(0)
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_BEGIN(
			ASW_ADDRESS_AND_SPACE_ARE_SAME_WIDTH,
			ASS_ADDRESS_AND_SPACE_ARE_SIGNED,
			ImplAddressT
		)
			false
		REDSTRAIN_UTIL_ADDRESSSPACE_IMPL_END

	  private:
		typedef IntegerBounds<SpaceT> SpaceBounds;
		typedef IntegerBounds<AddressT> AddressBounds;

	  public:
		/**
		 * Address space type. Present so that the type parameter can be
		 * extracted from a template instance.
		 *
		 * @since 0.1.0
		 */
		typedef SpaceT Space;
		/**
		 * Address type. Present so that the type parameter can be
		 * extracted from a template instance.
		 *
		 * @since 0.1.0
		 */
		typedef AddressT Address;

	  public:
		/**
		 * Type width relationship between address space and address types.
		 * That is, the correct constant of type #AddressSpaceWidth for
		 * the given template parameters.
		 *
		 * @since 0.1.0
		 */
		static const AddressSpaceWidth WIDTH_RELATIONSHIP =
				sizeof(SpaceT) < sizeof(AddressT) ? ASW_ADDRESS_IS_WIDER
				: (sizeof(SpaceT) > sizeof(AddressT) ? ASW_SPACE_IS_WIDER
				: ASW_ADDRESS_AND_SPACE_ARE_SAME_WIDTH);
		/**
		 * Signedness relationship between address space and address types.
		 * That is, the correct constant of type #AddressSpaceSignedness
		 * for the given template parameters.
		 *
		 * @since 0.1.0
		 */
		static const AddressSpaceSignedness SIGNEDNESS_RELATIONSHIP =
				static_cast<AddressSpaceSignedness>(AddressBounds::SIGNED * 2 + SpaceBounds::SIGNED);
		/**
		 * Whether address space excess is statically impossible. That is,
		 * @c true if and only if the @link #WIDTH_RELATIONSHIP width
		 * relationship @endlink and @link #SIGNEDNESS_RELATIONSHIP signedness
		 * relationship @endlink between the address space and address types
		 * indicate that there exists no address that exceeds the address
		 * space. In this case, @c exceededBy will always return @c false,
		 * regardless of its argument.
		 *
		 * Note that the converse is not definite, i.e. if this constant is
		 * @c false, there exist addresses that may or may not exceed the
		 * address space and the client must call #exceededBy to find
		 * out.
		 *
		 * @since 0.1.0
		 */
		static const bool CANNOT_EXCEED =
				(sizeof(SpaceT) > sizeof(AddressT) && SpaceBounds::SIGNED >= AddressBounds::SIGNED)
				|| (sizeof(SpaceT) == sizeof(AddressT) && SpaceBounds::SIGNED == AddressBounds::SIGNED);

	  public:
		/**
		 * Determine whether an address exceeds the address space. That is,
		 * returns @c true if and only if the value of @c address cannot
		 * be numerically represented as an <tt>AddressT</tt>.
		 *
		 * @param address an address whose value should be checked
		 * @return whether @c address exceeds the address space type
		 *
		 * @since 0.1.0
		 */
		static inline bool exceededBy(AddressT address) {
			return AddressSpaceImpl<SpaceT, AddressT, WIDTH_RELATIONSHIP, SIGNEDNESS_RELATIONSHIP>
					::exceededBy(address);
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_ADDRESSSPACE_HPP */
