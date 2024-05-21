#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "Types.hpp"
#include "ComponentArray.hpp"

class ComponentManager
{
    private:
        // map from type string pointer to component type
        std::unordered_map<const char*, ComponentType> componentTypes;

        // map from type string pointer to component array
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

        // the component type to be assigned to the next registered component - starting at 0
        ComponentType nextComponentType;

        // convenience function to get the statistically casted pointer to the ComponentArray of type T
        template<typename T> std::shared_ptr<ComponentArray<T>> getComponentArray()
        {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) == componentTypes.end())
                throw std::runtime_error("Cannot get component array for an unregistered component. Component has typeName: " + std::string(typeName));

            return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
        }

    public:
        // constructors & destructors
        ComponentManager()
            :componentTypes(), componentArrays(), nextComponentType(0)
        {}

        // register component
        template<typename T> void registerComponent()
        {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) != componentTypes.end())
                throw std::runtime_error("Cannot register a component that already exists in the component map. Component has typeName: " + std::string(typeName));

            // add this component type to the component type map
            componentTypes.insert({typeName, nextComponentType});

            // create a ComponentArray pointer and add it to the component arrays map
            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            // increment the value so that the next component registered will be different
            ++nextComponentType;
        }

        // getters
        template<typename T> ComponentType getComponentType()
        {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) == componentTypes.end())
                throw std::runtime_error("Cannot get component type for an unregistered component. Component has typeName: " + std::string(typeName));

            // return this component's type - used for creating signatures
            return componentTypes[typeName];
        }

        template<typename T> T& getComponent(Entity p_entity)
        {
            // get a reference to a component from the array for an entity
            return getComponentArray<T>()->getData(p_entity);
        }

        // logic
        template<typename T> void addComponent(Entity p_entity, T p_component)
        {
            // add a component to the array for an entity
            getComponentArray<T>()->insertData(p_entity, p_component);
        }

        template<typename T> void removeComponent(Entity p_entity)
        {
            // remove a component from the array for an entity
            getComponentArray<T>()->removeData(p_entity);
        }

        void entityDestroyed(Entity p_entity)
        {
            // notify each component array that an entity has been destroyed
            // if it has a component for that entity, it will remove it
            for (auto const& pair: componentArrays)
            {
                auto const& component = pair.second;
                component->entityDestroyed(p_entity);
            }
        }
};