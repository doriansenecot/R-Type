
# Coordinator

The **Coordinator** manages the interaction between entities, components, and systems. It is responsible for:

- **Entity Creation & Destruction**: Handles the lifecycle of entities.
- **Component Management**: Attaches and detaches components to/from entities.
- **System Management**: Registers and sets the signature for systems, ensuring they operate on entities with the required components.

## Managers in Coordinator

- **EntityManager**: Tracks which entities are currently active and manages their lifecycles.
- **ComponentManager**: Manages the different types of components and their association with entities.
- **SystemManager**: Manages all systems and ensures they are updated based on the entities they affect.
