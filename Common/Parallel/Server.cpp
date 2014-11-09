#include "stdafx.h"

#include "../Defs.h"

#include "Message.h"
#include "Server.h"

COMMON_PARALLEL_NS_BEGIN

void Server::OnMessage(const Message &message)
{
    UNUSED_PARAMETER(message);
}

COMMON_PARALLEL_NS_END