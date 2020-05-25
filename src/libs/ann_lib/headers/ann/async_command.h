#pragma once

#include <boost/optional.hpp>


namespace ann { namespace async {

enum class commandToThread
{
    CONFIGURE_NETWORK,
    START_TRAINING,
    GO_IDLE,
    START_TESTING,
    STOP
};

enum class commandToMain
{
    TESTING_FINISHED
};

template<typename T>
struct AsyncCommand
{
    using buffer = std::vector<uint8_t>;
    using commandType = T;

    T mCommand;
    buffer mBuffer;
};

using ThreadMessage = AsyncCommand<commandToThread>;
using ThreadOptMessage = boost::optional<ThreadMessage>;

using MainMessage = AsyncCommand<commandToMain>;
using MainOptMessage = boost::optional<MainMessage>;

}}