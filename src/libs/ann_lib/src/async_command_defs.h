#pragma once
#include <memory>
#include <db/idb.h>
#include "async_command_queue.h"

namespace ann { namespace async {


using ThreadCommandQueue = CommandQueue<commandToThread>;
using MainCommandQueue = CommandQueue<commandToMain>;

using ThreadCommandQueuePtr = std::shared_ptr<ThreadCommandQueue>;
using MainCommandQueuePtr = std::shared_ptr<MainCommandQueue>;

}
}