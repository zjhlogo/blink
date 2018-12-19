#pragma once
#include <unordered_map>

namespace blink
{
    template <class ID_TYPE, class INSTANCE_TYPE> class InstanceManager
    {
    private:
        class InstanceInfo
        {
        public:
            ID_TYPE id;
            int ref{};
        };

        typedef std::unordered_map<ID_TYPE, INSTANCE_TYPE*> IdInstanceMap;
        typedef std::unordered_map<INSTANCE_TYPE*, InstanceInfo> InstanceInfoMap;

    public:
        INSTANCE_TYPE* insertInstance(const ID_TYPE& id, INSTANCE_TYPE* instance = nullptr)
        {
            auto it = m_idInstanceMap.find(id);
            if (it != m_idInstanceMap.end())
            {
                assert(instance == nullptr);
                ++m_instanceInfoMap[it->second].ref;
                return it->second;
            }

            if (instance == nullptr) return nullptr;

            m_idInstanceMap.insert(std::make_pair(id, instance));

            InstanceInfo info;
            info.id = id;
            info.ref = 1;

            m_instanceInfoMap.insert(std::make_pair(instance, info));
            return instance;
        }

        bool removeInstance(INSTANCE_TYPE* instance)
        {
            auto it = m_instanceInfoMap.find(instance);
            if (it == m_instanceInfoMap.end()) return false;

            --it->second.ref;
            if (it->second.ref <= 0)
            {
                m_idInstanceMap.erase(m_idInstanceMap.find(it->second.id));
                m_instanceInfoMap.erase(it);
                return true;
            }

            return false;
        }

    private:
        IdInstanceMap m_idInstanceMap;
        InstanceInfoMap m_instanceInfoMap;

    };
}
