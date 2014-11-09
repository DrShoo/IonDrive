#pragma once

COMMON_PARALLEL_NS_BEGIN

class Server
{
public:
    virtual void OnMessage(const class Message &message) = 0;

};

COMMON_PARALLEL_NS_END