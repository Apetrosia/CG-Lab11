#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <iostream>

// ID ��������� ���������
GLuint Program;
// ID ��������
GLint Attrib_vertex;
// ID Vertex Buffer Object
GLuint VBO;

struct Vertex
{
    GLfloat x;
    GLfloat y;
};

// �������� ��� ���������� �������
const char* VertexShaderSource = R"(
 #version 330 core
 in vec2 coord;
 void main() {
 gl_Position = vec4(coord, 0.0, 1.0);
 }
)";

// �������� ��� ������������ �������
const char* FragShaderSource = R"(
 #version 330 core
 out vec4 color;
 void main() {
 color = vec4(0, 1, 0, 1);
 }
)";

void ShaderLog(unsigned int shader)
{
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

    if (infologLen > 1)
    {
        int charsWritten = 0;
        std::vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        std::cout << "InfoLog: " << infoLog.data() << std::endl;
    }
}

void InitShader()
{
    // ������� ��������� ������
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);

    // �������� �������� ���
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);

    // ����������� ������
    glCompileShader(vShader);
    std::cout << "vertex shader \n";

    // ������� ������ ���� �������
    ShaderLog(vShader);

    // ������� ����������� ������
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    // �������� �������� ���
    glShaderSource(fShader, 1, &FragShaderSource, NULL);

    // ����������� ������
    glCompileShader(fShader);
    std::cout << "fragment shader \n";

    // ������� ������ ���� �������
    ShaderLog(fShader);

    // ������� ��������� � ����������� ������� � ���
    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);

    // ������� ��������� ���������
    glLinkProgram(Program);

    // ��������� ������ ������
    int link_ok;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);

    if (!link_ok)
    {
        std::cout << "error attach shaders \n";
        return;
    }

    // ���������� ID �������� �� ��������� ���������
    const char* attr_name = "coord"; //��� � �������
    Attrib_vertex = glGetAttribLocation(Program, attr_name);

    if (Attrib_vertex == -1)
    {
        std::cout << "could not bind attrib " << attr_name << std::endl;
        return;
    }
}

void InitVBO()
{
    glGenBuffers(1, &VBO);
    // ������� ������ ������������
    Vertex figure[4] =
    {
        ///*
        { -0.5f, -0.5f },
        { -0.3f, 0.5f },
        { 0.3f, 0.5f },
        { 0.5f, -0.5f }
        //*/
        /*
        { -0.5f, -0.5f },
        { 0.0f, 0.5f },
        { 0.5f, -0.5f }
        */
        /*
        { -1.0f, -1.0f },
        { 0.0f, 1.0f },
        { 1.0f, -1.0f }
        */
    };

    // �������� ������� � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(figure), figure, GL_STATIC_DRAW);
}

void Init()
{
    // �������
    InitShader();
    // ��������� �����
    InitVBO();
}

void Draw()
{
    glUseProgram(Program); // ������������� ��������� ��������� �������
    glEnableVertexAttribArray(Attrib_vertex); // �������� ������ ���������
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���������� VBO

    // �������� OpenGL ��� �� ������ ���������������� ��������� ������.
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // ��������� VBO
    //glDrawArrays(GL_LINE_STRIP, 0, 4); // �������� ������ �� ����������(������)
    glDrawArrays(GL_LINES, 0, 4);
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(Attrib_vertex); // ��������� ������ ���������
    glUseProgram(0); // ��������� ��������� ���������
}

// ������������ ������
void ReleaseVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
}

// ������������ ��������
void ReleaseShader()
{
    // ��������� ����, �� ��������� ��������� ���������
    glUseProgram(0);
    // ������� ��������� ���������
    glDeleteProgram(Program);
}

void Release()
{
    // �������
    ReleaseShader();
    // ��������� �����
    ReleaseVBO();
}


int main()
{
    sf::Window window(sf::VideoMode(600, 600), "The Green Triangle", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    glewInit();

    Init();

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Draw();

        window.display();
    }

    Release();
    return 0;
}