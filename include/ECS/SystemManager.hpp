#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "Types.hpp"
#include "System.hpp"
// #include "ECSManager.hpp"

class SystemManager
{
    private:
        // map from system type string pointer to a signature
        std::unordered_map<const char*, Signature> signatures;

        // map from system type string pointer to a system pointer
        std::unordered_map<const char*, std::shared_ptr<System>> systems;

    public:
        // constructors & destructors
        SystemManager()
            :signatures(), systems()
        {}

        // register system
        template <typename T> std::shared_ptr<T> registerSystem()
        {
            const char* typeName = typeid(T).name();

            if (systems.find(typeName) != systems.end())
                throw std::runtime_error("Cannot register a system that already exists in the system map. System has typeName: " + std::string(typeName));

            // create a pointer to the system and return it so it can be used externally
            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        // setters
        template <typename T> void setSignature(Signature p_signature)
        {
            const char* typeName = typeid(T).name();

            if (systems.find(typeName) == systems.end())
                throw std::runtime_error("Cannot set signature for an unregistered system. System has typeName: " + std::string(typeName));

            signatures.insert({typeName, p_signature});
        }

        // logic
        void entityDestroyed(Entity p_entity)
        {
            // erase a destroyed entity from all system lists
            // entities is set so no check needed
            for (auto const& pair: systems)
            {
                auto const& system = pair.second;

                system->entities.erase(p_entity);
            }
        }

        void entitySignatureChanged(Entity p_entity, Signature p_entitySignature)
        {
            // notify each system that an entity's signature changed
            for (auto const& pair: systems)
            {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = signatures[type];

                // entity signature matches system signature - insert into set
                if ((p_entitySignature & systemSignature) == systemSignature)
                {
                    system->entities.insert(p_entity);
                }
                // entity signature doesn't match system signature - erase from set
                else
                {
                    system->entities.erase(p_entity);
                }
            }
        }
};