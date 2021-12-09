#include "Application.h"

#include <glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/noise.hpp"

#include "imgui/imgui.h"

#include <iostream>

Application::Application(Input& input, float width, float height) :
    m_Width(width),
    m_Height(height),
	m_Input(input),
    m_GridExtent(64),
    m_TileExtent(16),
    m_AspectRatio(m_Width / m_Height),
    m_Zoom(512),
    m_Speed(8.0f),
    m_ZoomSpeed(0.1f),
    m_Position((m_GridExtent* m_TileExtent) / 2, (m_GridExtent* m_TileExtent) / 2, 0.0f),
    //m_Position(0.0f, 0.0f, 10.0f),
    m_Projection(glm::ortho(-m_AspectRatio * (float)m_Zoom, m_AspectRatio* (float)m_Zoom, -(float)m_Zoom, (float)m_Zoom, 0.1f, 100.0f)),
    m_View(glm::translate(glm::mat4(1.0f), m_Position)), 
    m_Model_Grid(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_Model_YellowSelection1x1(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
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

    Vertex v0;
    v0.position[0] = 0;
    v0.position[1] = 0;
    v0.texCoord[0] = 0;
    v0.texCoord[1] = 0;
    m_Vertices_Grid.push_back(v0);

    Vertex v1;
    v1.position[0] = m_GridExtent * m_TileExtent;
    v1.position[1] = 0;
    v1.texCoord[0] = 1;
    v1.texCoord[1] = 0;
    m_Vertices_Grid.push_back(v1);

    Vertex v2;
    v2.position[0] = m_GridExtent * m_TileExtent;
    v2.position[1] = m_GridExtent * m_TileExtent;
    v2.texCoord[0] = 1;
    v2.texCoord[1] = 1;
    m_Vertices_Grid.push_back(v2);

    Vertex v3;
    v3.position[0] = 0;
    v3.position[1] = m_GridExtent * m_TileExtent;
    v3.texCoord[0] = 0;
    v3.texCoord[1] = 1;
    m_Vertices_Grid.push_back(v3);


    m_Indices_Grid.push_back(0);
    m_Indices_Grid.push_back(1);
    m_Indices_Grid.push_back(2);

    m_Indices_Grid.push_back(2);
    m_Indices_Grid.push_back(3);
    m_Indices_Grid.push_back(0);

    m_GridTextureBuffer = (unsigned char*)malloc(m_GridExtent * m_GridExtent * 4 * sizeof(unsigned char));
    
    for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            m_GridTextureBuffer[(i * 4) * m_GridExtent + (j * 4)] = m_Grid[i][j].type;
            m_GridTextureBuffer[(i * 4) * m_GridExtent + (j * 4) + 1] = 0;
            m_GridTextureBuffer[(i * 4) * m_GridExtent + (j * 4) + 2] = 0;
            m_GridTextureBuffer[(i * 4) * m_GridExtent + (j * 4) + 3] = 0;
        }
    }
    
    GLCall(glGenTextures(1, &m_GridTextureID));
    GLCall(glActiveTexture(GL_TEXTURE0 + m_GridTextureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_GridTextureID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_GridExtent, m_GridExtent, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_GridTextureBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    m_VertexArray_Grid = std::make_unique<VertexArray>();
    m_VertexArray_Grid->Bind();
    m_VertexBuffer_Grid = std::make_unique<VertexBuffer>(&m_Vertices_Grid[0], 4 * sizeof(Vertex), false);
    VertexBufferLayout bufferLayout_Grid;
    bufferLayout_Grid.Push<float>(2);
    bufferLayout_Grid.Push<float>(2);
    m_VertexArray_Grid->AddBuffer(*m_VertexBuffer_Grid, bufferLayout_Grid);
    m_IndexBuffer_Grid = std::make_unique<IndexBuffer>(&m_Indices_Grid[0], 6);
    m_Shader_Grid = std::make_unique<Shader>("src/shaders/Grid.vert.glsl", "src/shaders/Grid.frag.glsl");
    m_Shader_Grid->Bind();
    m_Shader_Grid->SetUniformMat4f("u_Projection", m_Projection);
    m_Shader_Grid->SetUniformMat4f("u_View", m_View);
    m_Shader_Grid->SetUniformMat4f("u_Model", m_Model_Grid);
    m_Shader_Grid->SetUniform1i("u_GridTexture", m_GridTextureID);

    float selection1x1Vertices [] =
    {
        0.0f, 0.0f, 0.0f, 0.0f,
        m_TileExtent, 0.0f, 1.0f, 0.0f,
        m_TileExtent, m_TileExtent, 1.0f, 1.0f,
        0.0f, m_TileExtent, 0.0f, 1.0f
    };
    unsigned int selection1x1Indices[] =
    {
        0, 1, 2, 
        2, 3, 0
    };
    m_VertexArray_YellowSelection1x1 = std::make_unique<VertexArray>();
    m_VertexArray_YellowSelection1x1->Bind();
    m_VertexBuffer_YellowSelection1x1 = std::make_unique<VertexBuffer>(selection1x1Vertices, 16 * sizeof(float), false);
    VertexBufferLayout bufferLayout_YellowSelection1x1;
    bufferLayout_YellowSelection1x1.Push<float>(2);
    bufferLayout_YellowSelection1x1.Push<float>(2);
    m_VertexArray_YellowSelection1x1->AddBuffer(*m_VertexBuffer_YellowSelection1x1, bufferLayout_YellowSelection1x1);
    m_IndexBuffer_YellowSelection1x1 = std::make_unique<IndexBuffer>(selection1x1Indices, 6);
    m_Texture_YellowSelection1x1 = std::make_unique<Texture>("res/textures/Selection1x1.png", true);
    m_Shader_YellowSelection1x1 = std::make_unique<Shader>("src/shaders/Selection.vert.glsl", "src/shaders/Selection.frag.glsl");
    m_Shader_YellowSelection1x1->Bind();
    m_Shader_YellowSelection1x1->SetUniformMat4f("u_Projection", m_Projection);
    m_Shader_YellowSelection1x1->SetUniformMat4f("u_View", m_View);
    m_Shader_YellowSelection1x1->SetUniformMat4f("u_Model", m_Model_Grid);
    m_Shader_YellowSelection1x1->SetUniform1i("u_Texture", m_Texture_YellowSelection1x1->m_RendererID);
}

Application::~Application()
{
}

bool Application::OnUpdate(float deltaTime)
{
    if (m_Input.GetKeyDown('A')) m_Position.x -= m_Speed;
    if (m_Input.GetKeyDown('D')) m_Position.x += m_Speed;
    if (m_Input.GetKeyDown('W')) m_Position.y += m_Speed;
    if (m_Input.GetKeyDown('S')) m_Position.y -= m_Speed;
    if (m_Input.GetKeyPressed('E') && m_Zoom > 64) m_Zoom = m_Zoom >> 1;
    if (m_Input.GetKeyPressed('Q') && m_Zoom < 512) m_Zoom = m_Zoom << 1;
    if (m_Input.GetKeyDown(256 /* Escape */)) 
        return false;

    //if (m_Zoom < 100.0f) m_Zoom = 100.0f;
    //if (m_Zoom > 800.0f) m_Zoom = 800.0f;

    if (m_Input.GetKeyPressed('F'))
    {
        std::cout << "#" << std::endl;
    }


    m_View = glm::translate(glm::mat4(1.0f), -m_Position);
    m_Projection = glm::ortho(-m_AspectRatio * (float)m_Zoom, m_AspectRatio * (float)m_Zoom, -(float)m_Zoom, (float)m_Zoom, -1000.0f, 1000.0f);

    m_MousePosition = glm::vec2(m_Input.GetMousePosition().x - m_Width / 2, m_Height - m_Input.GetMousePosition().y - m_Height / 2);

    m_TileScreenExtent = (m_Projection * glm::vec4(m_TileExtent, m_TileExtent, 0, 1)).x * m_Width / 2;


    m_MouseGridPosition.x = (int)((m_MousePosition.x) / m_TileScreenExtent + m_Position.x / m_TileExtent);
    m_MouseGridPosition.y = (int)((m_MousePosition.y) / m_TileScreenExtent + m_Position.y / m_TileExtent);

    m_Model_YellowSelection1x1 = glm::translate(glm::mat4(1.0f), glm::vec3(m_MouseGridPosition.x * m_TileExtent, m_MouseGridPosition.y * m_TileExtent, 0));

    /*for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            m_Grid[i][j].targeted = 0;
        }
    }

    if (m_MouseGridPosition.x >= 0 && m_MouseGridPosition.x < m_GridExtent &&
        m_MouseGridPosition.y >= 0 && m_MouseGridPosition.y < m_GridExtent)
    {
        m_Grid[m_MouseGridPosition.y][m_MouseGridPosition.x].targeted = 1;
    }

    for (int i = 0; i < m_GridExtent; i++)
    {
        for (int j = 0; j < m_GridExtent; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                m_Vertices_Grid[i * m_GridExtent * 4 + j * 4 + k].targeted = m_Grid[i][j].targeted;
            }
        }
    }*/

    m_Input.Update();

    return true;
}

void Application::OnRender()
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    
    m_VertexArray_Grid->Bind();
    m_VertexBuffer_Grid->Bind();
    m_Shader_Grid->Bind();
    m_Shader_Grid->SetUniformMat4f("u_View", m_View);
    m_Shader_Grid->SetUniformMat4f("u_Projection", m_Projection);
    m_Shader_Grid->SetUniformMat4f("u_Model", m_Model_Grid);
    GLCall(glActiveTexture(GL_TEXTURE0 + m_GridTextureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_GridTextureID));

    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer_Grid->GetCount(), GL_UNSIGNED_INT, nullptr));

   m_VertexArray_YellowSelection1x1->Bind();
   m_VertexBuffer_YellowSelection1x1->Bind();
   m_Shader_YellowSelection1x1->Bind();
   m_Shader_YellowSelection1x1->SetUniformMat4f("u_View", m_View);
   m_Shader_YellowSelection1x1->SetUniformMat4f("u_Projection", m_Projection);
   m_Shader_YellowSelection1x1->SetUniformMat4f("u_Model", m_Model_YellowSelection1x1);
   m_Texture_YellowSelection1x1->Bind(m_Texture_YellowSelection1x1->m_RendererID);
   
   GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer_Grid->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Application::OnImGuiRender()
{
}
