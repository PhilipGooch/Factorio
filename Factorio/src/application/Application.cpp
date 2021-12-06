#include "Application.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/noise.hpp"

#include "imgui/imgui.h"

Application::Application(Input& input) :
	m_Input(input),
    m_GridExtent(256),
    m_TileExtent(16),
    m_AspectRatio(1024.0f / 512.0f),
    m_Zoom(200.0f),
    m_Speed(4.0f),
    m_ZoomSpeed(10.0f),
    m_Position((m_GridExtent* m_TileExtent) / 2, (m_GridExtent* m_TileExtent) / 2, 10.0f),
    m_Projection(glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio* m_Zoom, -m_Zoom, m_Zoom, -1000.0f, 1000.0f)),
    m_View(glm::translate(glm::mat4(1.0f), m_Position)),
    m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
    for (int i = 0; i < m_GridExtent; i++)
    {
        m_Grid.push_back(std::vector<int>());
        for (int j = 0; j < m_GridExtent; j++)
        {
            m_Grid[i].push_back(3);
        }
    }

    for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            Vertex v0;
            v0.gridPos[0] = j;
            v0.gridPos[1] = i;
            v0.type = m_Grid[i][j];
            m_Vertices.push_back(v0);

            Vertex v1;
            v1.gridPos[0] = (j + 1);
            v1.gridPos[1] = i;
            v1.type = m_Grid[i][j];
            m_Vertices.push_back(v1);

            Vertex v2;
            v2.gridPos[0] = (j + 1);
            v2.gridPos[1] = (i + 1);
            v2.type = m_Grid[i][j];
            m_Vertices.push_back(v2);

            Vertex v3;
            v3.gridPos[0] = j;
            v3.gridPos[1] = (i + 1);
            v3.type = m_Grid[i][j];
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
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, m_GridExtent * m_GridExtent * 4 * sizeof(Vertex), true);
    m_VertexBuffer->Bind();
    m_VertexBuffer->Update(m_GridExtent * m_GridExtent * 4 * sizeof(Vertex), &m_Vertices[0]);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(1);
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
    m_IndexBuffer = std::make_unique<IndexBuffer>(&m_Indices[0], m_GridExtent * m_GridExtent * 6);
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
    if (m_Input.getKeyDown('A')) m_Position.x -= m_Speed;
    if (m_Input.getKeyDown('D')) m_Position.x += m_Speed;
    if (m_Input.getKeyDown('W')) m_Position.y += m_Speed;
    if (m_Input.getKeyDown('S')) m_Position.y -= m_Speed;
    if (m_Input.getKeyDown('E')) m_Zoom -= m_ZoomSpeed;
    if (m_Input.getKeyDown('Q')) m_Zoom += m_ZoomSpeed;
    
    if (m_Zoom < 100.0f) m_Zoom = 100.0f;
    if (m_Zoom > 800.0f) m_Zoom = 800.0f;

    m_View = glm::translate(glm::mat4(1.0f), -m_Position);
    m_Projection = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, -1000.0f, 1000.0f);
}

void Application::OnRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_View", m_View);
    m_Shader->SetUniformMat4f("u_Projection", m_Projection);

    m_VertexArray->Bind();
    m_IndexBuffer->Bind();

    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Application::OnImGuiRender()
{
}
