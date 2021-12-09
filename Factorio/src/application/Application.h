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
	Application(Input& input, float width, float height);
	~Application();

	bool OnUpdate(float deltaTime);
	void OnRender();
	void OnImGuiRender();

	

private:
	struct Tile
	{
		int type;
		bool targeted;
	};

	struct Vertex
	{
		float position[2];
		float texCoord[2];
	};

	Input& m_Input;
	float m_Width, m_Height;

	glm::vec2 m_MousePosition;
	glm::vec2 m_MouseGridPosition;

	std::unique_ptr<VertexArray> m_VertexArray_Grid;
	std::unique_ptr<VertexBuffer> m_VertexBuffer_Grid;
	std::unique_ptr<IndexBuffer> m_IndexBuffer_Grid;
	std::unique_ptr<Shader> m_Shader_Grid;

	std::unique_ptr<VertexArray> m_VertexArray_YellowSelection1x1;
	std::unique_ptr<Texture> m_Texture_YellowSelection1x1;
	std::unique_ptr<VertexBuffer> m_VertexBuffer_YellowSelection1x1;
	std::unique_ptr<IndexBuffer> m_IndexBuffer_YellowSelection1x1;
	std::unique_ptr<Shader> m_Shader_YellowSelection1x1;


	int m_GridExtent;
	const int m_TileExtent;

	std::vector<std::vector<Tile>> m_Grid;

	std::vector<Vertex> m_Vertices_Grid;
	std::vector<unsigned int> m_Indices_Grid;

	float m_AspectRatio;
	int m_Zoom;
	float m_Speed;
	float m_ZoomSpeed;
	glm::vec3 m_Position;
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_Model_Grid;
	glm::mat4 m_Model_YellowSelection1x1;

	float m_TileScreenExtent;

	unsigned char* m_GridTextureBuffer;
	unsigned int m_GridTextureID;
};

