#ifndef REDSTRAIN_MOD_LOG_MESSAGEMEMBERFORMATTER_HPP
#define REDSTRAIN_MOD_LOG_MESSAGEMEMBERFORMATTER_HPP

#include <redstrain/text/types.hpp>

namespace redengine {
namespace log {

	template<typename MemberT>
	class MessageMemberFormatter {

	  public:
		MessageMemberFormatter();
		MessageMemberFormatter(const MessageMemberFormatter&);
		virtual ~MessageMemberFormatter();

		virtual text::String16 formatMember(const MemberT&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOG_MESSAGEMEMBERFORMATTER_HPP */
