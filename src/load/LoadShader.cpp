#include "LoadShader.hpp"

GLchar* LoadShader::Load(const std::string &file)
{
	std::ifstream shaderFile;
	long shaderFileLength;

	shaderFile.open(file, std::ios::binary);

	if (shaderFile.fail()) {
		throw std::runtime_error("Failed to open file: " + file);
	}

	shaderFile.seekg(0, shaderFile.end);
	shaderFileLength = shaderFile.tellg();
	shaderFile.seekg(0, shaderFile.beg);

	GLchar *shaderCode = new GLchar[shaderFileLength + 1];
	shaderFile.read(shaderCode, shaderFileLength);

	shaderFile.close();

	shaderCode[shaderFileLength] = '\0';

	return shaderCode;
}