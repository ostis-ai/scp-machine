/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <deque>
#include <mutex>

namespace scp
{

template<typename Data>
class concurrent_deque
{
private:
    std::deque<Data> m_queue;
    mutable std::mutex m_mutex;

public:
    void push(const Data& data)
    {
        std::scoped_lock lock(m_mutex);
        m_queue.push_back(data);
    }

    bool empty() const
    {
        std::scoped_lock lock(m_mutex);
        return m_queue.empty();
    }

    Data& front()
    {
        std::scoped_lock lock(m_mutex);
        return m_queue.front();
    }

    Data const& front() const
    {
        std::scoped_lock lock(m_mutex);
        return m_queue.front();
    }

    void pop()
    {
        std::scoped_lock lock(m_mutex);
        m_queue.pop_front();
    }

    bool extract(std::function<bool (Data&)> checker, Data& result)
    {
        std::scoped_lock lock(m_mutex);
        for (auto it = m_queue.begin(); it != m_queue.end(); )
        {
            if (checker(*it))
            {
                result = *it;
                m_queue.erase(it);
                return true;
            }
            ++it;
        }
        return false;
    }
};

}

