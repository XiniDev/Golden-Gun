#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// basic
#include "Constants.hpp"
#include "GameConfig.hpp"
#include "Utils.hpp"

// resource manager
#include "ResourceManager.hpp"

// entity factory
#include "EntityFactory.hpp"

// ecs
#include "ECS/Types.hpp"
#include "ECS/System.hpp"
#include "ECS/ECSManager.hpp"

// components
#include "ECS/Components/Animation.hpp"
#include "ECS/Components/BoxCollider.hpp"
#include "ECS/Components/ControllableComponent.hpp"
#include "ECS/Components/Damage.hpp"
#include "ECS/Components/EntityLink.hpp"
#include "ECS/Components/Health.hpp"
#include "ECS/Components/MoveProperties.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/WeaponProperties.hpp"

// systems
#include "ECS/Systems/AnimationSystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/EntityLinkSystem.hpp"
#include "ECS/Systems/JumpMapCollisionSystem.hpp"
#include "ECS/Systems/MapCollisionSystem.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/MovementInputSystem.hpp"
#include "ECS/Systems/PhysicsSystem.hpp"
#include "ECS/Systems/PlayerLookSystem.hpp"
#include "ECS/Systems/PlayerWeaponSystem.hpp"
#include "ECS/Systems/SpriteSystem.hpp"
#include "ECS/Systems/WeaponInputSystem.hpp"
#include "ECS/Systems/WeaponSystem.hpp"

class Game
{
    private:
        std::size_t width;
        std::size_t height;

        GameConfig gc;

        SDL_Window* window;
        SDL_Renderer* renderer;

        ResourceManager resourceManager;

        ECSManager ecs;
        std::vector<std::shared_ptr<System>> systems;

        bool gameRunning;

    public:
        // constructors & destructors
        Game();
        ~Game();

        // getters
        std::size_t getWidth();
        std::size_t getHeight();
        bool isGameRunning();

        // setters
        // void setWidth(std::size_t p_width);
        // void setHeight(std::size_t p_height);

        // logic
        void loadResources();
        void setUpECS();

        // game loop logic
        void integrate(float dt);
        void handleEvents(SDL_Event p_event);
        void render(const float p_alpha);

        // initialise entities
        void initializeEntities(std::vector<Entity>& p_entities);

        // run game loop
        void runGameLoop();

        // free memory
        void cleanUp();
};