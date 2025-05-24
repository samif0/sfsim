#include "ecs/transform_component.hpp"

namespace SFSim {
namespace ECS {

TransformComponent::TransformComponent() 
    : _transform()
{
}

TransformComponent::TransformComponent(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
    : _transform(position, rotation, scale)
{
}

} // namespace ECS
} // namespace SFSim