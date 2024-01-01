#pragma once
#include <LowRenderer/Mesh.hpp>
#include <Physics/Transform.hpp>

namespace DataStructure
{
	class GameObject
	{
	public:
		Physics::Transform Transform;
		LowRenderer::Mesh Mesh;

	public:
		GameObject(const Physics::Transform& transform = Physics::Transform(), const LowRenderer::Mesh& mesh = LowRenderer::Mesh());
	};
}