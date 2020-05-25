#pragma once

#include <vector>
#include <deque>
#include <mutex>
#include <ann/async_command.h>

namespace ann { namespace async {

template<typename T>
class CommandQueue
{
    using lock = std::unique_lock<std::mutex>;
public:
    using message = AsyncCommand<T>;
    using optMessage = boost::optional<message>;

private:
    std::mutex mMutex;
    std::deque<message> mData;
    std::condition_variable mCv;

public:

    void pushCommand(message command)
    {
        {
            const lock l(mMutex);
            mData.push_back(std::move(command));
        }
        mCv.notify_one();
    }

    optMessage getCommand(bool waitForOne = false)
    {
        lock l(mMutex);
        if (waitForOne)
        {
            mCv.wait(l, [this]() {
                return mData.size() > 0;
            });
        }
        if (mData.size() > 0)
        {
            auto result = mData.front();
            mData.pop_front();
            return result;
        }
        return {};
    }

};

}} // namespace
