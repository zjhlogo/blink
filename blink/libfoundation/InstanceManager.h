/*!
 * \file InstanceManager.h
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 * 
 */
#pragma once
#include "BaseTypes.h"
#include <unordered_map>
#include <memory>
#include <cassert>

namespace blink
{
    class InstanceManagerBase
    {
    public:
        InstanceManagerBase()
        {
            m_managers.push_back(this);
        }

        virtual void releaseInstances() = 0;

        static void globalRelease()
        {
            for (auto it = m_managers.rbegin(); it != m_managers.rend(); ++it)
            {
                (*it)->releaseInstances();
            }
        }

    private:
        static std::vector<InstanceManagerBase*> m_managers;

    };

    template <class ID_TYPE, class INSTANCE_TYPE> class InstanceManager : public InstanceManagerBase
    {
    private:
        typedef std::unordered_map<ID_TYPE, std::shared_ptr<INSTANCE_TYPE>> IdInstanceMap;

    public:
        std::shared_ptr<INSTANCE_TYPE> insertInstance(const ID_TYPE& id, std::shared_ptr<INSTANCE_TYPE> instance = nullptr)
        {
            auto it = m_idInstanceMap.find(id);
            if (it != m_idInstanceMap.end())
            {
                assert(instance == nullptr);
                return it->second;
            }

            if (instance == nullptr) return nullptr;

            m_idInstanceMap.insert(std::make_pair(id, instance));
            return instance;
        }

        virtual void releaseInstances() override
        {
            for (auto& it : m_idInstanceMap)
            {
                if (it.second.use_count() > 1)
                {
                    // TODO: logout
                }
            }

            m_idInstanceMap.clear();
        }

        uint32 genUniqueId() { return m_nextGenId++; };

    private:
        uint32 m_nextGenId{ 1001 };
        IdInstanceMap m_idInstanceMap;

    };
}
