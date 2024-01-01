#include <Resources/Shader.hpp>

namespace Resources
{
	Shader::Shader(const char* path)
	{
		mVertexShader = glCreateShader(GL_VERTEX_SHADER);
		mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		mShaderProgram = glCreateProgram();
	}

	bool Shader::SetVertexShader(const char* path)
	{
		FILE* file;
		fopen_s(&file, path, "r");
		if (!file)
			return false;

		char curChar = fgetc(file);

		while (curChar != EOF)
		{
			mVertexShaderSource += curChar;
			curChar = fgetc(file);
		}

		fclose(file);

		const char* vtxSrc = mVertexShaderSource.c_str();

		glShaderSource(mVertexShader, 1, &vtxSrc, NULL);
		glCompileShader(mVertexShader);

		// check for shader compile errors
		glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &mSuccess);
		if (!mSuccess)
		{
			glGetShaderInfoLog(mVertexShader, 512, NULL, mInfoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << mInfoLog << std::endl;
			return false;
		}

		return true;
	}
	
	bool Shader::SetFragmentShader(const char* path)
	{
		FILE* file;
		fopen_s(&file, path, "r");
		if (!file)
			return false;

		char curChar = fgetc(file);

		while (curChar != EOF)
		{
			mFragmentShaderSource += curChar;
			curChar = fgetc(file);
		}

		fclose(file);

		const char* fgmtSrc = mFragmentShaderSource.c_str();


		glShaderSource(mFragmentShader, 1, &fgmtSrc, NULL);
		glCompileShader(mFragmentShader);

		// check for shader compile errors
		glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &mSuccess);
		if (!mSuccess)
		{
			glGetShaderInfoLog(mFragmentShader, 512, NULL, mInfoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << mInfoLog << std::endl;
			return false;
		}

		return true;
	}

	bool Shader::Link()
	{
		// link shaders
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);

		// check for linking errors
		glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &mSuccess);
		if (!mSuccess) {
			glGetProgramInfoLog(mShaderProgram, 512, NULL, mInfoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << mInfoLog << std::endl;
			return false;
		}
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);

		return true;
	}
}