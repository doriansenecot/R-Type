
# Architecture

The project uses a **modular ECS architecture**, which allows for flexibility, decoupled logic, and easy extensibility. The ECS pattern divides entities, components, and systems, each handling a distinct responsibility in the game.

## ECS Overview

- **Entity**: The basic object in the game, an entity is simply an ID and has no behavior or data on its own.
- **Component**: A data structure that holds specific information for an entity, such as its position, velocity, or health.
- **System**: Logic or functionality that operates on entities possessing specific components.

## Systems

- **MovementSystem**: Updates entities' positions based on their velocity components.
- **RenderSystem**: Responsible for rendering the graphical representation of entities.
- **HealthSystem**: Manages health bars and health-related interactions.
- **LaserSystem**: Handles shooting logic and manages laser projectiles.
- **LevelSystem**: Manages player experience points and levels.
- **NameRenderSystem**: Renders entity names on the screen.
- **InputSystem**: Processes input for local players.
- **ExplosionSystem**: Handles explosions and their effects.
- **ScoreSystem**: Manages player scores.
- **StageSystem**: Manages the game stage and level progression.

## Components

- **TransformComponent**: Stores position, scale, and rotation of an entity.
- **SpriteComponent**: Stores sprite information for rendering.
- **VelocityComponent**: Contains velocity data for moving entities.
- **HealthComponent**: Manages current and max health values.
- **XpComponent**: Handles XP and leveling up.
- **NameComponent**: Stores the name associated with an entity.
- **AnimationComponent**: Controls animation frames for sprites.
- **BulletComponent**: Contains properties related to projectiles.
- **LaserEffectComponent**: Stores effects like laser animations.
- **InputComponent**: Captures and stores player input states.
