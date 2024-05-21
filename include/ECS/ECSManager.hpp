#pragma once

#include <memory>

#include "Types.hpp"
#include "System.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

class ECSManager
{
    private:
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<SystemManager> systemManager;

    public:
        // constructors & destructors
        ECSManager()
            :entityManager(), componentManager(), systemManager()
        {
            // create pointers to each manager
            entityManager = std::make_unique<EntityManager>();
            componentManager = std::make_unique<ComponentManager>();
            systemManager = std::make_unique<SystemManager>();
        }

        // entity logic
        Entity createEntity()
        {
            return entityManager->createEntity();
        }

        void destroyEntity(Entity p_entity)
        {
            entityManager->destroyEntity(p_entity);
            componentManager->entityDestroyed(p_entity);
            systemManager->entityDestroyed(p_entity);
        }

        // component setters
        template <typename T> T& getComponent(Entity p_entity)
        {
            return componentManager->getComponent<T>(p_entity);
        }

        template <typename T> ComponentType getComponentType()
        {
            return componentManager->getComponentType<T>();
        }

        // component logic
        template <typename T> void registerComponent()
        {
            componentManager->registerComponent<T>();
        }

        template <typename T> void addComponent(Entity p_entity, T p_component)
        {
            componentManager->addComponent<T>(p_entity, p_component);

            Signature signature = entityManager->getSignature(p_entity);
            signature.set(componentManager->getComponentType<T>(), true);
            entityManager->setSignature(p_entity, signature);

            systemManager->entitySignatureChanged(p_entity, signature);
        }

        template <typename T> void removeComponent(Entity p_entity)
        {
            componentManager->removeComponent<T>(p_entity);

            Signature signature = entityManager->getSignature(p_entity);
            signature.set(componentManager->getComponentType<T>(), false);
            entityManager->setSignature(p_entity, signature);

            systemManager->entitySignatureChanged(p_entity, signature);
        }

        // system setters
        template <typename T> void setSystemSignature(Signature p_signature)
        {
            systemManager->setSignature<T>(p_signature);
        }

        // system logic
        template <typename T> std::shared_ptr<T> registerSystem()
        {
            return systemManager->registerSystem<T>();
        }
};