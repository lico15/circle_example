#include "context.h"

ContextUPtr Context::Create()
{
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init()
{

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);

    /// per_vertex_color.vs, per_vertex_color.vs
    ///ShaderPtr vertShader = Shader::CreateFromFile("./shader/per_vertex_color.vs", GL_VERTEX_SHADER);
    ///ShaderPtr fragShader = Shader::CreateFromFile("./shader/per_vertex_color.fs", GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.3f, 0.3f, 0.3f, 0.3f);

    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void Context::CreateCircle(float radius, float radiusin, int segment, float circleDegreeStart, float circleDegreeEnd, float circle_colorRad,
                           float circle_colorGreen, float circle_colorBlue)
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    const float pi = 3.141592f;

    for (int i = 0; i <= segment; i++)
    {
        float angle = (360.f / segment * i) * pi / 180.0f;
        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    for (int i = 0; i < segment; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        if (i == segment)
            indices.push_back(0);
        else
            indices.push_back(i + 2);
    }

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
                                            GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size());

    m_indexCount = (int)indices.size();

    auto loc = glGetUniformLocation(m_program->Get(), "vertexColor");
    m_program->Use();
    glUniform4f(loc, circle_colorRad, circle_colorGreen, circle_colorBlue, 0.0f);

///    for (int i = 0; i <= segment; i++)
///    {
///        float angle = (360.f / segment * i) * pi / 180.0f;
///        float x = cosf(angle) * radiusin;
///        float y = sinf(angle) * radiusin;
///        vertices.push_back(x);
///        vertices.push_back(y);
///        vertices.push_back(0.0f);
///    }
///   for (int i = 0; i < segment; i++)
///    {
///        indices.push_back(0);
///        indices.push_back(i + 1);
///        if (i == segment)
///            indices.push_back(0);
///        else
///            indices.push_back(i + 2);
///    }
///    m_vertexLayout = VertexLayout::Create();
///    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());
///    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
///    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size());
///    m_indexCount = (int)indices.size();
///    loc = glGetUniformLocation(m_program->Get(), "vertexColor");
///    m_program->Use();
}
