#pragma once

#include "Input.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

class Application
{
public:
	Application(Input& input);
	~Application();

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnImGuiRender();

protected:
	Input& m_Input;

private:
	struct Vertex
	{
		float gridPos[2];
		float type;
	};

	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	int m_GridExtent;
	const int m_TileExtent;

	std::vector<std::vector<int>> m_Grid;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	float m_AspectRatio;
	float m_Zoom;
	float m_Speed;
	float m_ZoomSpeed;
	glm::vec3 m_Position;
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_Model;

};

