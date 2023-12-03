#c-cpp #software-design 
## 2023.12.02

## What is a ecs, Why ecs

### Data-based

### Component-based

### Archetype-based

## How does a ecs been composed

### 1. Entity

Entity can be seen as a combination of a set of components. And it stores **NO** real data in it.   
All data should be stored together in somewhere else. It's just a identifier of a collection of component objects.

**Attributes**

- `Id`
  - `version(gen)`: indicates the version of the entity, or like Valid flag   
    Incremented every time an entity is deleted, in case of use invalid id access
  - `is_entity`: Something indicates it's a entity or component(optional)  
    In some implementation, entities and components may not that clear, an entity can also be a component, vice versa.
    ```cpp
    // gaia-ecs
    ecs::World w;
    ecs::Entity player0 = w.add();
    ecs::Entity player1 = w.add();
    ecs::Entity player2 = w.add();
    ecs::Entity teamA = w.add();
    ecs::Entity teamB = w.add();
    // Add player0 and player1 to teamA
    w.add(player0, teamA);
    w.add(player1, teamA);
    // Add player2 to teamB
    w.add(player2, teamB);
    ```

- `component_store`: Something indicates where its components stores
  - maybe a pointer
  - maybe use template to implement
  - maybe stores in entity manage

- `component_flag`: Something indicates which components does it has
  - maybe a bitset
  - maybe stores in entity manage
  - archetype based implementation doesn't need

- `world`: world reference

- `Name(optional)`
  - maybe store in class
  - maybe only store a pointer to it, managered by user

**Methods**


### 2. World(Registry/Entity Manager)

**Attributes**

- `components_storage`
  - index components just by entity id
    - Store components in entities' object
      ```cpp
      std::unordered_map<TypeIndex, Internal::BaseComponentContainer*> components;
      ```
    - Use several vectors for every components
    - Use sparse vectors for every components
    - Use pool for every components
      ```cpp
      // Placement new into the component pool.
      Pool<C> *pool = accomodate_component<C>();
      ::new(pool->get(id.index())) C(std::forward<Args>(args) ...);
      ```
  - `Archetype-based`: Classify a set of components into a class(`Archetype`), store all entities with the same collection of components(like all `Entity<Transform, Tag>`) in it  
    Every `Archetype` containes a bunch of `chunks` which containes the entity data(which is components),
    containes a entity list which containes entities

- `entity_to_component_map`: Something indicates which components does an entity has
  - maybe a vector of bitset
  - maybe classified entities into `Archetype`

- `component_types(optional)`: Something manage all components' type description
  like a map or vector something
  ```cpp
  // gaia-ecs
  //! Fast-lookup cache for the first FastComponentCacheSize components
  cnt::darray<const ComponentCacheItem*> m_descIdArr;
  //! Slower but more memory-friendly lookup cache for components with ids beyond FastComponentCacheSize
  cnt::map<detail::ComponentDescId, const ComponentCacheItem*> m_descByDescId;
  ```
  `gaia-ecs` use both of vector for the first `FastCount` of components and map for those `id > FastCount` components.  
  `ComponentCacheItem` is used to trait components' constructors or something, to fit its specialized store strategy

**Methods**

- Create entity
- Add component to an entity
- Get component from an entity
- Iterator(View)

### 3. Component

- `component type id`: the entity manager need to know this to classify the components.  
  Always computed use meta-programming
  - maybe store in ComponentBase class 
    ```cpp
    // entityx
    template <typename C>
    static BaseComponent::Family EntityManager::component_family() {
        return Component<typename std::remove_const<C>::type>::family();
    }
    template <typename C>
    BaseComponent::Family Component<C>::family() {
        static Family family = family_counter_++;
        assert(family < entityx::MAX_COMPONENTS);
        return family;
    }
    ```
  - maybe use another trait class
    ```cpp
    // gaia-ecs
    template <typename...>
    class type_group {
        inline static uint32_t s_identifier{};
    public:
        template <typename... Type>
        inline static const uint32_t id = s_identifier++;
    };
    ```
  - maybe use `TypeList` like something(need to use all component class declare `ComponentList` in source code)
    ```cpp
    template<typename ...T>
    using ComponentList = MPL::TypeList<T...>

    template<typename T>
    static constexpr std::size_t ID() noexcept
    {
        return MPL::IndexOf<T, ComponentList>{};
    }
    ```

### 4. Component handle

Provide a safe way to access component

- `version`: 

### 5. System

System is designed to only focus on one concrete set of components, rather than a bunch of entities

### 6. Serializition

### 7. Event