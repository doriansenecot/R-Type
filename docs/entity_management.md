
# Entity Management

Entities represent everything from players to projectiles in the game. The **EntityManager** is responsible for:

- **Creating and Destroying Entities**: Ensures entities are properly initialized and cleaned up.
- **Tracking Signatures**: A signature is a bitmask representing the components an entity has. Systems use signatures to determine which entities they operate on.

Each entity is a unique ID and has components attached to it that store its state. The Coordinator facilitates entity creation, updates, and destruction.
