#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    //��ʼ��GLFW��
    /* Initialize the library */
    if (!glfwInit())
        return -1;


    //����һ������
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        //�������ʧ�ܣ���ֹGLFW�����ش���
        glfwTerminate();
        return -1;
    }

    //�����ڵ� OpenGL ��������Ϊ��ǰ������
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //��ʼ��GLEW
	if(glewInit()!=GLEW_OK)
		std::cout << "Error initializing GLEW" << std::endl;

	//���OpenGL�汾��Ϣ
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


    //����һ�����㻺�����������ݷ����Դ棬Ȼ����û��ƺ���
    //���㻺����
    float position[] = {
        -0.5f, -0.5f, 0.0f, // ���½�
         0.0f,  0.5f, 0.0f, // ����
         0.5f, -0.5f, 0.0f  // ���½�
	};

	unsigned int buffer; //���㻺���������ID
	glGenBuffers(1, &buffer); //����һ�����㻺��������
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //�󶨶��㻺��������

    //������ʵ�ʲ���
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),position,GL_STATIC_DRAW); //���仺�����ռ�,�����ݷ��뻺������


	glEnableVertexAttribArray(0); //���ö����������飬0��ʾ�������Ե�λ�ã�����ɫ���ж��壩

    // ���ö�������ָ��
    //������0�����Ƕ������Ե�λ�ã�
    // 2��ʾÿ������������������x��y����
    // GL_FLOAT��ʾ�������ͣ�GL_FALSE��ʾ����Ҫ��һ����2 * sizeof(float)��ʾÿ������Ĳ�����(void*)0��ʾ�ӻ���������ʼλ�ÿ�ʼ��ȡ����
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //����Ⱦѭ��
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //ÿ��ѭ����ʼ�����ɫ������
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


		//Ϊ�������������Ƶ��ã������취��glDrawArrays()(û������������) �� glDrawElements()
		glDrawArrays(GL_TRIANGLES, 0, 3); //���������Σ������ֱ�Ϊ����ģʽ����ʼ��������������

        //����OpenGL������β���


        /* Swap front and back buffers */
        glfwSwapBuffers(window); //���⻭��˺��

        //�����¼�
        /* Poll for and process events */
        glfwPollEvents();
    }

    //���GLFW��Դ
    glfwTerminate();
    return 0;
}