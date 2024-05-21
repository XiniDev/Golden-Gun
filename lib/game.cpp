#include "Game.hpp"

// constructors & destructors

Game::Game()
    :gameRunning(true)
{
    width = 1920;
    height = 1080;

    // width = 2560;
    // height = 1440;

    gc = GameConfig
    {
        .scale = (float) height / 360,
        .fps = 100
    };

    window = SDL_CreateWindow(
        "Golden Gun",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (window == NULL)
        std::cout << "Failed to create window." << "\nError: " << SDL_GetError() << std::endl;

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL)
        std::cout << "Failed to create renderer." << "\nError: " << SDL_GetError() << std::endl;

    // after the window has been created, get the pixel dimensions that drawing will occur within
    int PIXEL_WIDTH;
    int PIXEL_HEIGHT;
    SDL_GL_GetDrawableSize(window, &PIXEL_WIDTH, &PIXEL_HEIGHT);

    // set scales for each dimension
    const float SCALE_X = (float)PIXEL_WIDTH / (float) width;
    const float SCALE_Y = (float)PIXEL_HEIGHT / (float) height;
    SDL_RenderSetScale(renderer, SCALE_X, SCALE_Y);

    ecs = ECSManager();
}

Game::~Game()
{}

// getters
bool Game::isGameRunning()
{
    return gameRunning;
}

std::size_t Game::getWidth()
{
    return width;
}

std::size_t Game::getHeight()
{
    return height;
}

// setters

// logic

void Game::loadResources()
{
    // load all needed textures from assets
    // characters
    resourceManager.loadTexture(renderer, "predator_sprite", "assets/gfx/sprites/predator.png");
    resourceManager.loadTexture(renderer, "dummy_sprite", "assets/gfx/sprites/dummy.png");

    // weapons
    resourceManager.loadTexture(renderer, "predator_weapon", "assets/gfx/weapons/predator_weapon.png");
    resourceManager.loadTexture(renderer, "predator_bullet", "assets/gfx/weapons/predator_bullet.png");

    // tiles
    resourceManager.loadTexture(renderer, "test_tiles", "assets/gfx/tiles/test_tiles.png");
}

void Game::setUpECS()
{
    // register components
    ecs.registerComponent<Animation>();
    ecs.registerComponent<BoxCollider>();
    ecs.registerComponent<Damage>();
    ecs.registerComponent<ControllableComponent>();
    ecs.registerComponent<EntityLink>();
    ecs.registerComponent<Health>();
    ecs.registerComponent<MoveProperties>();
    ecs.registerComponent<RigidBody>();
    ecs.registerComponent<Sprite>();
    ecs.registerComponent<Transform>();
    ecs.registerComponent<WeaponProperties>();

    // register systems and set signatures
    auto animationSystem = ecs.registerSystem<AnimationSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<Animation>());
        signature.set(ecs.getComponentType<Sprite>());

        ecs.setSystemSignature<AnimationSystem>(signature);
    }

    auto collisionSystem = ecs.registerSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<BoxCollider>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<CollisionSystem>(signature);
    }

    auto entityLinkSystem = ecs.registerSystem<EntityLinkSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<EntityLink>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<EntityLinkSystem>(signature);
    }

    auto jumpMapCollisionSystem = ecs.registerSystem<JumpMapCollisionSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<BoxCollider>());
        signature.set(ecs.getComponentType<MoveProperties>());
        signature.set(ecs.getComponentType<RigidBody>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<JumpMapCollisionSystem>(signature);
    }

    auto mapCollisionSystem = ecs.registerSystem<MapCollisionSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<BoxCollider>());
        signature.set(ecs.getComponentType<RigidBody>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<MapCollisionSystem>(signature);
    }

    auto movementInputSystem = ecs.registerSystem<MovementInputSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<ControllableComponent>());
        signature.set(ecs.getComponentType<MoveProperties>());

        ecs.setSystemSignature<MovementInputSystem>(signature);
    }

    auto movementSystem = ecs.registerSystem<MovementSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<MoveProperties>());
        signature.set(ecs.getComponentType<RigidBody>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<MovementSystem>(signature);
    }

    auto physicsSystem = ecs.registerSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<RigidBody>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<PhysicsSystem>(signature);
    }

    auto playerLookSystem = ecs.registerSystem<PlayerLookSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<ControllableComponent>());
        signature.set(ecs.getComponentType<Sprite>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<PlayerLookSystem>(signature);
    }

    auto playerWeaponSystem = ecs.registerSystem<PlayerWeaponSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<ControllableComponent>());
        signature.set(ecs.getComponentType<EntityLink>());
        signature.set(ecs.getComponentType<Sprite>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<PlayerWeaponSystem>(signature);
    }

    auto spriteSystem = ecs.registerSystem<SpriteSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<Sprite>());
        signature.set(ecs.getComponentType<Transform>());

        ecs.setSystemSignature<SpriteSystem>(signature);
    }

    auto weaponInputSystem = ecs.registerSystem<WeaponInputSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<ControllableComponent>());
        signature.set(ecs.getComponentType<WeaponProperties>());

        ecs.setSystemSignature<WeaponInputSystem>(signature);
    }

    auto weaponSystem = ecs.registerSystem<WeaponSystem>();
    {
        Signature signature;
        signature.set(ecs.getComponentType<Transform>());
        signature.set(ecs.getComponentType<WeaponProperties>());

        ecs.setSystemSignature<WeaponSystem>(signature);
    }

    systems.push_back(movementInputSystem);
    systems.push_back(weaponInputSystem);
    systems.push_back(movementSystem);
    systems.push_back(weaponSystem);
    systems.push_back(physicsSystem);
    systems.push_back(entityLinkSystem);
    systems.push_back(playerLookSystem);
    systems.push_back(playerWeaponSystem);
    systems.push_back(spriteSystem);
    systems.push_back(animationSystem);
    systems.push_back(collisionSystem);
    systems.push_back(mapCollisionSystem);
    systems.push_back(jumpMapCollisionSystem);
}

// game loop logic

void Game::integrate(float p_dt)
{
    for (auto const& system: systems)
    {
        system->update(ecs, gc, resourceManager, p_dt);
    }
}

void Game::handleEvents(SDL_Event p_event)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    // event handling loop
    if (SDL_PollEvent(&p_event))
    {
        // window quit event
        if (p_event.type == SDL_QUIT)
        {
            gameRunning = false;
        }

        // ecs handle events
        for (auto const& system: systems)
        {
            system->handleEvents(ecs, &p_event, keys);
        }

        // if (p_event.type == SDL_MOUSEMOTION)
        // {
        //     cursor.getMouseState();
        // }
    }
}

void Game::render(const float p_alpha)
{
    // render loop

    // reset render draw color
    SDL_SetRenderDrawColor(renderer, 122, 122, 122, 255);

    // clear -> render -> display
    SDL_RenderClear(renderer);

    for (auto const& system: systems)
    {
        system->render(ecs, renderer, p_alpha);
    }

    SDL_RenderPresent(renderer);
}

void Game::initializeEntities(std::vector<Entity>& p_entities)
{
    for (int i = 0; i < 25; i++)
    {
        Entity tile = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>((50 + (i * 24)) * gc.scale), static_cast<int>(300 * gc.scale)));
        p_entities.push_back(tile);
    }

    Entity tile1 = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>(290 * gc.scale), static_cast<int>(276 * gc.scale)));
    Entity tile2 = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>(338 * gc.scale), static_cast<int>(228 * gc.scale)));
    Entity tile3 = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>(386 * gc.scale), static_cast<int>(276 * gc.scale)));
    Entity tile4 = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>(434 * gc.scale), static_cast<int>(204 * gc.scale)));
    Entity tile5 = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>(482 * gc.scale), static_cast<int>(276 * gc.scale)));
    Entity tile6 = entityFactory::makeTile(ecs, gc, resourceManager, Vector2(static_cast<int>(482 * gc.scale), static_cast<int>(252 * gc.scale)));
    p_entities.push_back(tile1);
    p_entities.push_back(tile2);
    p_entities.push_back(tile3);
    p_entities.push_back(tile4);
    p_entities.push_back(tile5);
    p_entities.push_back(tile6);

    
    Entity dummy = entityFactory::makeDummy(ecs, gc, resourceManager, Vector2(static_cast<int>(325 * gc.scale), static_cast<int>(50 * gc.scale)));
    p_entities.push_back(dummy);

    // Entity tile = entityFactory::makeTile(ecs, gc, resourceManager, (Vector2(100 + (10 * 18), 100)) * static_cast<int>(gc.scale));
    // p_entities.push_back(tile);

    // make the player at the end to ensure the player entity is rendered at the end (front of all entities)
    Entity player = entityFactory::makePlayer(ecs, gc, resourceManager);
    Entity playerWeapon = entityFactory::makePlayerWeapon(ecs, gc, resourceManager, player);

    // add all entities into the "entities" vector (using pointer so push_back copies the pointer instead of the object)
    p_entities.push_back(player);
    p_entities.push_back(playerWeapon);
}

void Game::runGameLoop()
{
    // set up entity vector
    std::vector<Entity> entities;

    initializeEntities(entities);

    SDL_Event event;

    const float dt = 1 / (float) gc.fps;
    float accumulator = 0.0f;
    float currentTime = utils::hires_time_in_seconds();

    while (gameRunning)
    {
        // get mouse state
        SDL_GetMouseState(&gc.mouseState.x, &gc.mouseState.y);

        // calculate frame time and limit it to 0.25 seconds
        float newTime = utils::hires_time_in_seconds();
        float frameTime = newTime - currentTime;

        if (frameTime > 0.25)
            frameTime = 0.25;

        currentTime = newTime;

        accumulator += frameTime;

        // update the game loop in fixed time steps
        while (accumulator >= dt)
        {
            // event handling
            handleEvents(event);

            // integrate loop
            integrate(dt);

            accumulator -= dt;
        }

        const float alpha = accumulator / dt;

        // State state = currentState * alpha + previousState * ( 1.0 - alpha );

        // render loop
        render(alpha);
    }
}

// free memory

void Game::cleanUp()
{
    // free memory
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}