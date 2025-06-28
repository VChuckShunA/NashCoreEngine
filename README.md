# NashCoreEngine
**Nashcore** is a custom 2D game engine built in modern C++ using SFML, developed with the goals of performance, modularity, and extensibility in mind. The engine was created to support several personal gameplay-focused projects, including "Ashura's Revenge" (a WFC-based top-down shooter) and a custom Behavior Tree AI system.

This framework is designed to facilitate the rapid development of 2D games and includes a clean implementation of an **Entity Component System**, a **Wave Function Collapse** module, and a **Quadtree-based spatial partitioning system** for efficient collision handling and AI steering.

---

## ✨ Features

### 🔧 Core Architecture

* Modular **Entity Component System** (ECS) with support for:

  * Transform, Animation, Input, Physics, and AI components
  * Easily extensible for custom components and systems
* Event-based messaging between systems
* Clean separation between logic, rendering, and update loops

### 🧠 AI Systems

* Custom-built **Behavior Tree System**:

  * Includes selectors, sequences, conditions, and action nodes
  * Blackboard pattern for memory and context passing
  * Reusable across entities with modular logic trees

### 🧱 Procedural Generation

* **Wave Function Collapse** algorithm:

  * Generates tile-based, visually coherent backgrounds
  * Preserves a consistent visual language and structure
  * Can be seeded for deterministic layouts

### 🕵️‍♂️ Spatial Awareness

* **Dynamic Quadtree System**:

  * Real-time updates as entities move
  * Used for spatial queries, collision detection, and AI obstacle avoidance
  * Integrated into AI steering and pathfinding logic

### 🎮 Gameplay Utilities

* Player input handling
* Projectile and enemy systems
* Simple FSMs and utility AI stubs for different agents
* Debug overlays for collisions, AI states, and spatial structures

---

## 🧪 Example Projects Using Nashcore

### Ashura's Revengence

A 2D Shootem up using:

* Wave Function Collapse for procedural background generation
* Quadtree for enemy collision detection

### Behavior Tree Demo

A sandbox where:

* Enemies patrol, detect the player, fire with cooldown logic
* Custom BT system for enemy behaviors
* Blackboard memory is used for visibility and ammo tracking

---

## 🛠 Known Limitations

* No sound or music module (yet)
* Physics system is AABB-only (no rotation or mass-based forces)
* Not currently optimized for mobile or web export

---

## 🤝 License

MIT License

---

## 👤 Author

**Vichakshana Arangala**
Gameplay Programmer | C++ | Unreal | AI Systems
📍 Currently based in Sri Lanka — open to opportunities in the UK and beyond
🔗 [LinkedIn](https://www.linkedin.com/in/vichakshana-arangala/) • [Portfolio](https://vichakshanaarangal.wixsite.com/vichakshana-arangala) • [GitHub](https://github.com/VChuckShunA)

---

This engine is a labor of love and learning. If you're a fellow gameplay programmer, AI dev, or systems engineer, I'd love to connect!
