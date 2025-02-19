#pragma once
#include <vector>
#include <glm/mat4x4.hpp>

#include "../Patterns/Observer.h"
#include "../ViewPortsHolder.h"

class ApplyTransformationCallBack : public Callback {
public:
    virtual void execute() override
    {
        m_modelMatrix = glm::mat4(1.0f);

        // Apply scaling
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

        // Apply rotation
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotationAngleDegree), m_rotationAngle);

        // Apply translation
        m_modelMatrix = glm::translate(m_modelMatrix, m_translation);

        /*
        auto it = ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.find(m_currentMesh);
        if (it != ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.end()) {
            // Get the tuple containing MeshRenderingShaderData
            auto& shaderDataTuple = it->second;

            // Extract MeshRenderingShaderData from the tuple
            ViewPortHolder::MeshRenderingShaderData& shaderData = std::get<1>(shaderDataTuple);
            shaderData.EdgeShader.bind();
            shaderData.EdgeShader.setMat4("u_model", m_modelMatrix);
            shaderData.EdgeShader.unbind();

            shaderData.FaceShader.bind();
            shaderData.FaceShader.setMat4("u_model", m_modelMatrix);
            shaderData.FaceShader.unbind();

            shaderData.NormalShader.bind();
            shaderData.NormalShader.setMat4("u_model", m_modelMatrix);
            shaderData.NormalShader.unbind();

            shaderData.PointShader.bind();
            shaderData.PointShader.setMat4("u_model", m_modelMatrix);
            shaderData.PointShader.unbind();
        }
        */

    }

    void setTranslationVector(const glm::vec3& translation)
    {
		m_translation = translation;
	}

    void setRotationAngle(const glm::vec3& rotationAngle)
    {
	    m_rotationAngle = rotationAngle;
    }

    void setRotationAngleDegree(float rotationAngleDegree)
    {
	    m_rotationAngleDegree = rotationAngleDegree;
	}

    void setScale(const glm::vec3& scale)
    {
		m_scale = scale;
    }


    Mesh* m_currentMesh;

    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    glm::vec3 m_translation{1.0f, 1.0f, 1.0f};
    glm::vec3 m_rotationAngle{0.0f, 1.0f, 0.0f};
    float m_rotationAngleDegree = 0;
    glm::vec3 m_scale{1.0f, 1.0f, 1.0f};
};