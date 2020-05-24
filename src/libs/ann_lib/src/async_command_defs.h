#pragma once
#include <memory>
#include <db/idb.h>
#include "async_command.h"

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

using ThreadCommandQueuePtr = std::shared_ptr<CommandQueue<commandToThread>>;
using MainCommandQueuePtr = std::shared_ptr<CommandQueue<commandToMain>>;

using SharedDbPtr = std::shared_ptr<db::DBInterface>;

}
}