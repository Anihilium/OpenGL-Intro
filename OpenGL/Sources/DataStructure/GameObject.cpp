#include <DataStructure/GameObject.hpp>

namespace DataStructure
{
	GameObject::GameObject(const Physics::Transform& transform, const LowRenderer::Mesh& mesh):
		Transform(transform), Mesh(mesh)
	{}
}