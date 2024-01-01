#pragma once
#include <Resources/IResource.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Resources
{
	extern unsigned int ID;
	class Texture : public IResource
	{
	public:
		unsigned int texID;

	private:
		GLuint mTexture;
		GLuint mSampler;

	public:
		Texture(const char* path);
		~Texture();

	private:
		void LoadData(const char* path) override;

	};
}
