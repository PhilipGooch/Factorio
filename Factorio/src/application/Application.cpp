#include "Application.h"

#include <glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/noise.hpp"

#include "imgui/imgui.h"

#include <iostream>

Application::Application(Input& input) :
	m_Input(input),
    m_GridExtent(256),
    m_TileExtent(16),
    m_AspectRatio(1024.0f / 512.0f),
    m_Zoom(200.0f),
    m_Speed(8.0f),
    m_ZoomSpeed(4.0f),
    m_Position((m_GridExtent* m_TileExtent) / 2, (m_GridExtent* m_TileExtent) / 2, 10.0f),
    m_Projection(glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio* m_Zoom, -m_Zoom, m_Zoom, -1000.0f, 1000.0f)),
    m_View(glm::translate(glm::mat4(1.0f), m_Position)),
    m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
    for (int i = 0; i < m_GridExtent; i++)
    {
        m_Grid.push_back(std::vector<Tile>());
        for (int j = 0; j < m_GridExtent; j++)
        {
            Tile tile;
            float m_NoiseZoom = 0.05f;
            float m_HeightWater = 0.25f;
            float m_HeightSand = 0.35f;
            float m_HeightGrass = 1.0f;
            float m_HeightRock = 0.718f;
            float noise = glm::perlin<float>(glm::vec3(j * m_NoiseZoom, i * m_NoiseZoom, 0.5f)) + 0.5f;

            tile.type = noise < m_HeightWater ? 0 : noise < m_HeightSand ? 1 : noise < m_HeightGrass ? 2 : 3;
            tile.targeted = false;
            m_Grid[i].push_back(tile);
        }
    }

    for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            Vertex v0;
            v0.gridPosition[0] = j;
            v0.gridPosition[1] = i;
            v0.type = m_Grid[i][j].type;
            v0.targeted = m_Grid[i][j].targeted;
            m_Vertices.push_back(v0);

            Vertex v1;
            v1.gridPosition[0] = (j + 1);
            v1.gridPosition[1] = i;
            v1.type = m_Grid[i][j].type;
            v1.targeted = m_Grid[i][j].targeted;
            m_Vertices.push_back(v1);

            Vertex v2;
            v2.gridPosition[0] = (j + 1);
            v2.gridPosition[1] = (i + 1);
            v2.type = m_Grid[i][j].type;
            v2.targeted = m_Grid[i][j].targeted;
            m_Vertices.push_back(v2);

            Vertex v3;
            v3.gridPosition[0] = j;
            v3.gridPosition[1] = (i + 1);
            v3.type = m_Grid[i][j].type;
            v3.targeted = m_Grid[i][j].targeted;
            m_Vertices.push_back(v3);
        }
    }

    for (int i = 0; i < powf(m_GridExtent, 2); i++)
    {
        m_Indices.push_back(i * 4);
        m_Indices.push_back(i * 4 + 1);
        m_Indices.push_back(i * 4 + 2);

        m_Indices.push_back(i * 4 + 2);
        m_Indices.push_back(i * 4 + 3);
        m_Indices.push_back(i * 4);
    }

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->Bind();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, m_GridExtent * m_GridExtent * 4 * sizeof(Vertex), true);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(1);
    layout.Push<float>(1);
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
    m_IndexBuffer = std::make_unique<IndexBuffer>(&m_Indices[0], m_GridExtent * m_GridExtent * 6);
    m_IndexBuffer->Bind();
    m_Shader = std::make_unique<Shader>("src/shaders/vertex.vert.glsl", "src/shaders/fragment.frag.glsl");
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_Projection", m_Projection);
    m_Shader->SetUniformMat4f("u_View", m_View);
    m_Shader->SetUniformMat4f("u_Model", m_Model);
    m_Shader->SetUniform1f("u_TileExtent", m_TileExtent);
}

Application::~Application()
{
}

void Application::OnUpdate(float deltaTime)
{
    if (m_Input.GetKeyDown('A')) m_Position.x -= m_Speed;
    if (m_Input.GetKeyDown('D')) m_Position.x += m_Speed;
    if (m_Input.GetKeyDown('W')) m_Position.y += m_Speed;
    if (m_Input.GetKeyDown('S')) m_Position.y -= m_Speed;
    if (m_Input.GetKeyDown('E')) m_Zoom -= m_ZoomSpeed;
    if (m_Input.GetKeyDown('Q')) m_Zoom += m_ZoomSpeed;

    if (m_Zoom < 100.0f) m_Zoom = 100.0f;
    if (m_Zoom > 800.0f) m_Zoom = 800.0f;


    m_View = glm::translate(glm::mat4(1.0f), -m_Position);
    m_Projection = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, -1000.0f, 1000.0f);

    m_MousePosition = glm::vec2(m_Input.GetMousePosition().x - 1024 / 2, 512 - m_Input.GetMousePosition().y - 512 / 2);

    m_TileScreenExtent = (m_Projection * glm::vec4(m_TileExtent, m_TileExtent, 0, 1)).x * 512;

    m_MouseGridPosition.x = (int)((m_MousePosition.x + 1) / m_TileScreenExtent + m_Position.x / m_TileExtent);
    m_MouseGridPosition.y = (int)((m_MousePosition.y + 1) / m_TileScreenExtent + m_Position.y / m_TileExtent);

    //m_MouseGridPosition.x = (int)((m_MousePosition.x + 1) / m_TileScreenExtent);
    //m_MouseGridPosition.y = (int)((m_MousePosition.y + 1) / m_TileScreenExtent);

    //if (m_MousePosition.x < 0) m_MouseGridPosition.x -= 1;
    //if (m_MousePosition.y < 0) m_MouseGridPosition.y -= 1;


    for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            m_Grid[i][j].targeted = 0;
        }
    }

    //std::cout << m_MouseGridPosition.x << " " << m_MouseGridPosition.y << std::endl;

    if (m_MouseGridPosition.x >= 0 && m_MouseGridPosition.x < m_GridExtent &&
        m_MouseGridPosition.y >= 0 && m_MouseGridPosition.y < m_GridExtent)
    {
        m_Grid[m_MouseGridPosition.y][m_MouseGridPosition.x].targeted = 1;
    }
}

void Application::OnRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_View", m_View);
    m_Shader->SetUniformMat4f("u_Projection", m_Projection);


    for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                m_Vertices[i * m_GridExtent * 4 + j * 4 + k].targeted = m_Grid[i][j].targeted;
            }
        }
    }

    m_VertexBuffer->Bind();
    m_VertexBuffer->Update(m_GridExtent * m_GridExtent * 4 * sizeof(Vertex), &m_Vertices[0]);

    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Application::OnImGuiRender()
{
}
