#ifndef REDSTRAIN_MOD_VFS_HIERARCHICALURISYNTAXERROR_HPP
#define REDSTRAIN_MOD_VFS_HIERARCHICALURISYNTAXERROR_HPP

#include "InvalidURIPartError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API HierarchicalURISyntaxError : public InvalidURIPartError {

	  public:
		HierarchicalURISyntaxError(const std::string&, util::MemorySize);
		HierarchicalURISyntaxError(const text::String16&, util::MemorySize);
		HierarchicalURISyntaxError(const text::String32&, util::MemorySize);
		HierarchicalURISyntaxError(const HierarchicalURISyntaxError&);

		REDSTRAIN_DECLARE_ERROR(HierarchicalURISyntaxError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_HIERARCHICALURISYNTAXERROR_HPP */
