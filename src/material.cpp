#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "material.h"

void Material::SetFloat(const std::string& paramName, float value)
{
	int location = m_shader.GetPramLocation(paramName);

	if (location >= 0)
		m_floatUniforms[location] = value;
}

void Material::SetVec4(const std::string& paramName, glm::vec4 value)
{
	int location = m_shader.GetPramLocation(paramName);

	if (location >= 0)
		m_vec4Uniforms[location] = value;
}

void Material::ApplySun(const Sun& sun) const
{
	if (m_sunDirectionLocation >= 0)
		glUniform3fv(m_sunDirectionLocation, 1, glm::value_ptr(sun.direction));

	if (m_sunAmbientLocation >= 0)
		glUniform3fv(m_sunAmbientLocation, 1, glm::value_ptr(sun.ambient));

	if (m_sunDiffuseLocation >= 0)
		glUniform3fv(m_sunDiffuseLocation, 1, glm::value_ptr(sun.diffuse));

	if (m_sunSpecularLocation >= 0)
		glUniform3fv(m_sunSpecularLocation, 1, glm::value_ptr(sun.specular));
}

void Material::InitializeStandardUniforms()
{
	m_shader.Use();

	m_colorLocation = m_shader.GetPramLocation("material.color");
	m_diffuseMapLocation = m_shader.GetPramLocation("material.diffuseMap");
	if (m_diffuseMapLocation >= 0) glUniform1i(m_diffuseMapLocation, 0);
	m_diffuseOverrideLocation = m_shader.GetPramLocation("material.diffuseOverride");
	m_specularMapLocation = m_shader.GetPramLocation("material.specularMap");
	if (m_specularMapLocation >= 0) glUniform1i(m_specularMapLocation, 1);
	m_specularOverrideLocation = m_shader.GetPramLocation("material.specularOverride");
	m_shininessLocation = m_shader.GetPramLocation("material.shininess");

	m_sunDirectionLocation = m_shader.GetPramLocation("sun.direction");
	m_sunAmbientLocation = m_shader.GetPramLocation("sun.ambient");
	m_sunDiffuseLocation = m_shader.GetPramLocation("sun.diffuse");
	m_sunSpecularLocation = m_shader.GetPramLocation("sun.specular");
}

void Material::ApplyUniforms() const
{
	if (m_colorLocation >= 0)
		glUniform3fv(m_colorLocation, 1, &m_color[0]);

	if (m_diffuseOverrideLocation >= 0)
		glUniform1i(m_diffuseOverrideLocation, m_diffuseMap == nullptr);

	if (m_specularOverrideLocation >= 0)
		glUniform1i(m_specularOverrideLocation, m_specularMap == nullptr);

	if (m_shininessLocation >= 0)
		glUniform1f(m_shininessLocation, m_shininess);

	for (const auto& [location, value] : m_floatUniforms)
		glUniform1f(location, value);

	for (const auto& [location, value] : m_vec4Uniforms)
		glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Material::ApplyTextures() const
{
	if (m_diffuseMap)
	{
		glActiveTexture(GL_TEXTURE0);
		m_diffuseMap->Use();
	}

	if (m_specularMap)
	{
		glActiveTexture(GL_TEXTURE1);
		m_specularMap->Use();
	}
}

void Material::Use(const Sun& sun) const
{
	m_shader.Use();
	ApplyUniforms();
	ApplyTextures();
	ApplySun(sun);
}