#pragma once

#include <boost/optional.hpp>
#include <vector>
#include <deque>
#include <mutex>

namespace ann { namespace async {


template<typename T>
struct AsyncCommand
{
    using buffer = std::vector<uint8_t>;
    using commandType = T;

    T mCommand;
    buffer mBuffer;
};

template<typename T>
class CommandQueue
{
    using lock = std::lock_guard<std::mutex>;
public:
    using commandType = AsyncCommand<T>;
    using optCommandType = boost::optional<commandType>;

private:
    std::mutex mMutex;
    std::deque<commandType> mData;

public:

    void pushCommand(commandType command)
    {
        const lock l(mMutex);
        mData.push_back(std::move(command));
    }

    optCommandType getCommand()
    {
        const lock l(mMutex);
        if (mData.size() > 0)
        {
            commandType result = mData.front();
            mData.pop_front();
            return result;
        }
        return {};
    }

};

}}