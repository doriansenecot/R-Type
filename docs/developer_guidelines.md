
# Developer Guidelines

This document outlines best practices and important guidelines for developers working on the R-Type project.

## Protocol Binary Format

Ensure all messages sent between client and server adhere to the binary format described in the `binary_protocol.md` file. Use the **MessageHeader** for validation and keep message sizes consistent with padding where needed.

## Message Serialization/Deserialization

Handle serialization of all messages to byte arrays before sending, and deserialize incoming messages upon reception. This ensures efficient data transmission over UDP.

## Adding New Message Types

When adding new message types, extend the protocol accordingly and update both the server and client to handle the new message. Follow the structure in `protocol.hpp` for defining new messages and ensure you update both serialization and deserialization logic.

## Handling Entity Management

- Avoid creating unnecessary entities in-game.
- Ensure that every entity created in the game is properly destroyed once no longer needed, to avoid memory leaks.

## ECS Architecture

When adding new components or systems:
- Register the new component with the **Coordinator**.
- Define the system signature to ensure it operates on the correct entities.
