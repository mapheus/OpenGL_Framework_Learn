#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
	std::string ComputeSource;
};

class Shader
{
private:
	const std::string& m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath, bool compute);
	~Shader();

	void Bind() const;
	void Unbind() const;

	unsigned int GetShaderID();

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4(const std::string& name, const glm::mat4& mat);
	void SetUniform1i(const std::string& name, int v);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform3f(const std::string& name, glm::vec3 v);
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	std::string ParseComputeShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateComputeShader(const std::string& computeShader);
	int GetUniformLocation(const std::string& name);
};