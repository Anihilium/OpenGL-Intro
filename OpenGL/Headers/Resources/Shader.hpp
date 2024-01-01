#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Resources/IResource.hpp>
#include <Debug/Assertion.hpp>

namespace Resources
{
	class Shader : public IResource
	{
	private:
		int mVertexShader;
		int mFragmentShader;
		int mShaderProgram;

		std::string mVertexShaderSource;
		std::string mFragmentShaderSource;

		int mSuccess;
		char mInfoLog[512];

	public:
		Shader(const char* path);
		~Shader() {};
		bool SetVertexShader(const char* path);
		bool SetFragmentShader(const char* path);
		bool Link();
		int GetShaderProgram() { return mShaderProgram; }

	private:
		void LoadData(const char* path) override {};

	};
}