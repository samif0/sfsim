#include "scene/scene.hpp"
#include "ecs/transform_component.hpp"
#include "ecs/render_component.hpp"
#include <algorithm>

namespace SFSim {

Scene::Scene()
    : _activeCamera(nullptr)
    , _nextEntityId(1)
{
}

Scene::~Scene() {
    clear();
}

Entity* Scene::createEntity() {
    auto entity = std::make_unique<Entity>(_nextEntityId++);
    Entity* ptr = entity.get();
    
    _entities.push_back(std::move(entity));
    updateEntityIndexMap();
    
    for (const auto& system : _systems) {
        system->onEntityAdded(ptr);
    }
    
    return ptr;
}

Entity* Scene::createEntity(const std::string& name) {
    Entity* entity = createEntity();
    entity->setName(name);
    return entity;
}

void Scene::destroyEntity(EntityID id) {
    auto it = _entityIndexMap.find(id);
    if (it != _entityIndexMap.end()) {
        size_t index = it->second;
        Entity* entity = _entities[index].get();
        
        for (const auto& system : _systems) {
            system->onEntityRemoved(entity);
        }
        
        _entities.erase(_entities.begin() + index);
        updateEntityIndexMap();
    }
}

void Scene::destroyEntity(Entity* entity) {
    if (entity) {
        destroyEntity(entity->getId());
    }
}

Entity* Scene::getEntity(EntityID id) {
    auto it = _entityIndexMap.find(id);
    if (it != _entityIndexMap.end()) {
        return _entities[it->second].get();
    }
    return nullptr;
}

const Entity* Scene::getEntity(EntityID id) const {
    auto it = _entityIndexMap.find(id);
    if (it != _entityIndexMap.end()) {
        return _entities[it->second].get();
    }
    return nullptr;
}

Entity* Scene::findEntityByName(const std::string& name) {
    for (const auto& entity : _entities) {
        if (entity->getName() == name) {
            return entity.get();
        }
    }
    return nullptr;
}

const Entity* Scene::findEntityByName(const std::string& name) const {
    for (const auto& entity : _entities) {
        if (entity->getName() == name) {
            return entity.get();
        }
    }
    return nullptr;
}

std::vector<Entity*> Scene::getAllEntities() {
    std::vector<Entity*> result;
    result.reserve(_entities.size());
    
    for (const auto& entity : _entities) {
        result.push_back(entity.get());
    }
    
    return result;
}

std::vector<const Entity*> Scene::getAllEntities() const {
    std::vector<const Entity*> result;
    result.reserve(_entities.size());
    
    for (const auto& entity : _entities) {
        result.push_back(entity.get());
    }
    
    return result;
}

void Scene::removeSystem(System* system) {
    auto it = std::find_if(_systems.begin(), _systems.end(),
        [system](const std::unique_ptr<System>& ptr) {
            return ptr.get() == system;
        });
    
    if (it != _systems.end()) {
        for (const auto& entity : _entities) {
            (*it)->onEntityRemoved(entity.get());
        }
        _systems.erase(it);
    }
}

void Scene::update(float deltaTime) {
    for (const auto& entity : _entities) {
        entity->update(deltaTime);
    }
    
    for (const auto& system : _systems) {
        system->update(deltaTime, _entities);
    }
}

void Scene::render(sf::RenderWindow& window) {
    if (!_activeCamera) return;
    
    Matrix4x4 viewProjection = _activeCamera->getViewProjectionMatrix();
    
    auto renderableEntities = getEntitiesWith<TransformComponent, RenderComponent>();
    
    for (Entity* entity : renderableEntities) {
        auto* transform = entity->getComponent<TransformComponent>();
        auto* render = entity->getComponent<RenderComponent>();
        
        if (render->isVisible() && render->getGeometry()) {
            render->getGeometry()->draw(window, transform->getWorldMatrix(), viewProjection);
        }
    }
}

void Scene::clear() {
    for (const auto& system : _systems) {
        for (const auto& entity : _entities) {
            system->onEntityRemoved(entity.get());
        }
    }
    
    _entities.clear();
    _systems.clear();
    _entityIndexMap.clear();
    _nextEntityId = 1;
}

void Scene::updateEntityIndexMap() {
    _entityIndexMap.clear();
    for (size_t i = 0; i < _entities.size(); ++i) {
        _entityIndexMap[_entities[i]->getId()] = i;
    }
}

} // namespace SFSim