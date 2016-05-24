#ifndef REDSTRAIN_MOD_QU6NTUM_PARAMETERTYPE_HPP
#define REDSTRAIN_MOD_QU6NTUM_PARAMETERTYPE_HPP

#include <cstddef>
#include <redstrain/util/BareType.hpp>
#include <redstrain/util/ClassifyType.hpp>
#include <redstrain/util/RemovePointer.hpp>
#include <redstrain/text/types.hpp>

#include "Service.hpp"

namespace redengine {
namespace qu6ntum {

	enum ParameterTypeID {
		PTI_INT8,
		PTI_INT16,
		PTI_INT32,
		PTI_INT64,
		PTI_INT8_PTR,
		PTI_INT16_PTR,
		PTI_INT32_PTR,
		PTI_INT64_PTR,
		PTI_FLOAT32,
		PTI_FLOAT64,
		PTI_STRING8,
		PTI_STRING16,
		PTI_STRING32,
		PTI_STRING8_BUFFER,
		PTI_STRING16_BUFFER,
		PTI_STRING32_BUFFER,
		PTI_SERVICE
	};

	template<typename UserT>
	class ParameterType {

	  private:
		typedef util::BareType<UserT> BT;
		typedef util::RemovePointer<typename BT::Bare> RP;
		typedef typename util::BareType<typename RP::Component>::Bare IT;
		typedef util::ClassifyType<IT> CT;

	  private:
		template<
			typename ExposedT,
			typename InnerT,
			size_t InnerSize,
			int TypeClass,
			bool WasRef,
			unsigned PointerLevel
		>
		struct TypeImpl;

#define Q6_PTYPE_STRAIGHT_PRIMITIVE(ptid) { \
		typedef ExposedT Exposed; \
		typedef InnerT Internal; \
		static const ParameterTypeID TYPE_ID = ptid; \
		static inline InnerT toInternal(ExposedT value) { \
			return value; \
		} \
		static inline ExposedT toExposed(InnerT value) { \
			return value; \
		} \
	}

#define Q6_PTYPE_INT_BUFFER(ptid) { \
		typedef ExposedT Exposed; \
		typedef InnerT* Internal; \
		static const ParameterTypeID TYPE_ID = ptid; \
		static inline InnerT* toInternal(ExposedT value) { \
			return value; \
		} \
		static inline ExposedT toExposed(InnerT* value) { \
			return value; \
		} \
	}

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(1u), util::TCLS_INTEGER, false, 0u>
		Q6_PTYPE_STRAIGHT_PRIMITIVE(PTI_INT8);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(2u), util::TCLS_INTEGER, false, 0u>
		Q6_PTYPE_STRAIGHT_PRIMITIVE(PTI_INT16);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(4u), util::TCLS_INTEGER, false, 0u>
		Q6_PTYPE_STRAIGHT_PRIMITIVE(PTI_INT32);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(8u), util::TCLS_INTEGER, false, 0u>
		Q6_PTYPE_STRAIGHT_PRIMITIVE(PTI_INT64);

		template<typename ExposedT>
		struct TypeImpl<ExposedT, float, sizeof(float), util::TCLS_FLOAT, false, 0u>
		Q6_PTYPE_STRAIGHT_PRIMITIVE(PTI_FLOAT32);

		template<typename ExposedT>
		struct TypeImpl<ExposedT, double, sizeof(double), util::TCLS_FLOAT, false, 0u>
		Q6_PTYPE_STRAIGHT_PRIMITIVE(PTI_FLOAT64);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(1u), util::TCLS_INTEGER, false, 1u>
		Q6_PTYPE_INT_BUFFER(PTI_INT8_PTR);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(2u), util::TCLS_INTEGER, false, 1u>
		Q6_PTYPE_INT_BUFFER(PTI_INT16_PTR);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(4u), util::TCLS_INTEGER, false, 1u>
		Q6_PTYPE_INT_BUFFER(PTI_INT32_PTR);

		template<typename ExposedT, typename InnerT>
		struct TypeImpl<ExposedT, InnerT, static_cast<size_t>(8u), util::TCLS_INTEGER, false, 1u>
		Q6_PTYPE_INT_BUFFER(PTI_INT64_PTR);

		//TODO

#undef Q6_PTYPE_STRAIGHT_PRIMITIVE
#undef Q6_PTYPE_INT_BUFFER

	  public:
		typedef TypeImpl<UserT, IT, sizeof(IT), CT::CLASS, BT::WAS_REF, RP::POINTER_LEVELS> Traits;

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_PARAMETERTYPE_HPP */
