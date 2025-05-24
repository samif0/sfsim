#include "ecs/render_component.hpp"

namespace SFSim {
namespace ECS {

RenderComponent::RenderComponent()
    : _geometry(nullptr)
    , _visible(true)
{
}

RenderComponent::RenderComponent(std::unique_ptr<Geometry> geometry)
    : _geometry(std::move(geometry))
    , _visible(true)
{
}

void RenderComponent::setGeometry(std::unique_ptr<Geometry> geometry) {
    _geometry = std::move(geometry);
}

void RenderComponent::setColor(const sf::Color& color) {
    if (_geometry) {
        _geometry->setColor(color);
    }
}

} // namespace ECS
} // namespace SFSim