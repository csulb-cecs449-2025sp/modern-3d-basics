#pragma once
#include <glm/ext.hpp>
#include <string>
class ShaderProgram {
	uint32_t m_programId;

public:
	ShaderProgram();
	void load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void activate();

	void setUniform(const std::string& uniformName, bool value);
	void setUniform(const std::string& uniformName, int32_t value);
	void setUniform(const std::string& uniformName, float value);
	void setUniform(const std::string& uniformName, const glm::vec2& value);
	void setUniform(const std::string& uniformName, const glm::vec3& value);
	void setUniform(const std::string& uniformName, const glm::vec4& value);
	void setUniform(const std::string& uniformName, const glm::mat2& value);
	void setUniform(const std::string& uniformName, const glm::mat3& value);
	void setUniform(const std::string& uniformName, const glm::mat4& value);
};