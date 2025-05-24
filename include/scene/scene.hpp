#pragma once

#include "ecs/entity.hpp"
#include "ecs/system.hpp"
#include "camera.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace SFSim {

using namespace ECS;

class Scene {
public:
    Scene();
    ~Scene();
    
    Entity* createEntity();
    Entity* createEntity(const std::string& name);
    void destroyEntity(EntityID id);
    void destroyEntity(Entity* entity);
    
    Entity* getEntity(EntityID id);
    const Entity* getEntity(EntityID id) const;
    Entity* findEntityByName(const std::string& name);
    const Entity* findEntityByName(const std::string& name) const;
    
    std::vector<Entity*> getAllEntities();
    std::vector<const Entity*> getAllEntities() const;
    
    template<typename T>
    std::vector<Entity*> getEntitiesWith() {
        std::vector<Entity*> result;
        for (const auto& entity : _entities) {
            if (entity->isActive() && entity->hasComponent<T>()) {
                result.push_back(entity.get());
            }
        }
        return result;
    }
    
    template<typename T, typename U, typename... Rest>
    std::vector<Entity*> getEntitiesWith() {
        std::vector<Entity*> result;
        for (const auto& entity : _entities) {
            if (entity->isActive() && 
                entity->hasComponent<T>() && 
                entity->hasComponent<U>() &&
                hasAllComponents<Rest...>(entity.get())) {
                result.push_back(entity.get());
            }
        }
        return result;
    }
    
    template<typename T, typename... Args>
    T* addSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = system.get();
        _systems.push_back(std::move(system));
        
        for (const auto& entity : _entities) {
            ptr->onEntityAdded(entity.get());
        }
        
        return ptr;
    }
    
    void removeSystem(System* system);
    
    void setActiveCamera(Camera* camera) { _activeCamera = camera; }
    Camera* getActiveCamera() const { return _activeCamera; }
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    void clear();
    
    size_t getEntityCount() const { return _entities.size(); }
    size_t getSystemCount() const { return _systems.size(); }
    
private:
    std::vector<std::unique_ptr<Entity>> _entities;
    std::vector<std::unique_ptr<System>> _systems;
    std::unordered_map<EntityID, size_t> _entityIndexMap;
    
    Camera* _activeCamera;
    EntityID _nextEntityId;
    
    template<typename T>
    bool hasAllComponents(Entity* entity) {
        return entity->hasComponent<T>();
    }
    
    template<typename T, typename U, typename... Rest>
    bool hasAllComponents(Entity* entity) {
        return entity->hasComponent<T>() && hasAllComponents<U, Rest...>(entity);
    }
    
    void updateEntityIndexMap();
};

} // namespace SFSim