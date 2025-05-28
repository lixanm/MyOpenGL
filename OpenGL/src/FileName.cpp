#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main01(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f, // ���½�
         0.0f,  0.5f, // ����
         0.5f, -0.5f  // ���½�
    };

    // �������󶨶����������(VAO)
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // �������󶨶��㻺����
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);


	

    // ���ö������Բ�����ָ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // �򵥵Ķ�����ɫ��Դ����
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "}\0";

    // �򵥵�Ƭ����ɫ��Դ����
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n" // ��ɫ
        "}\0";

    // ���붥����ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ����Ƭ����ɫ��
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ������ɫ����������
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // ʹ����ɫ������
        glUseProgram(shaderProgram);

        // ��VAO
        glBindVertexArray(vao);

        // ����������
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &buffer);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}