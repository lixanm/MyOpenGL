#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    //初始化GLFW库
    /* Initialize the library */
    if (!glfwInit())
        return -1;


    //创建一个窗口
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        //如果创建失败，终止GLFW并返回错误
        glfwTerminate();
        return -1;
    }

    //将窗口的 OpenGL 上下文设为当前上下文
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //初始化GLEW
	if(glewInit()!=GLEW_OK)
		std::cout << "Error initializing GLEW" << std::endl;

	//输出OpenGL版本信息
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


    //定义一个顶点缓冲区，将数据发向显存，然后调用绘制函数
    //顶点缓冲区
    float position[] = {
        -0.5f, -0.5f, 0.0f, // 左下角
         0.0f,  0.5f, 0.0f, // 顶部
         0.5f, -0.5f, 0.0f  // 右下角
	};

	unsigned int buffer; //顶点缓冲区对象的ID
	glGenBuffers(1, &buffer); //生成一个顶点缓冲区对象
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //绑定顶点缓冲区对象

    //缓冲区实际布局
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),position,GL_STATIC_DRAW); //分配缓冲区空间,将数据放入缓冲区中


	glEnableVertexAttribArray(0); //启用顶点属性数组，0表示顶点属性的位置（在着色器中定义）

    // 设置顶点属性指针
    //索引：0，这是顶点属性的位置，
    // 2表示每个顶点有两个分量（x和y），
    // GL_FLOAT表示数据类型，GL_FALSE表示不需要归一化，2 * sizeof(float)表示每个顶点的步长，(void*)0表示从缓冲区的起始位置开始读取数据
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //主渲染循环
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //每次循环开始清除颜色缓冲区
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


		//为缓冲区发出绘制调用，两个办法：glDrawArrays()(没有索引缓冲器) 或 glDrawElements()
		glDrawArrays(GL_TRIANGLES, 0, 3); //绘制三角形，参数分别为绘制模式、起始索引、顶点数量

        //告诉OpenGL顶点如何布局


        /* Swap front and back buffers */
        glfwSwapBuffers(window); //避免画面撕裂

        //处理事件
        /* Poll for and process events */
        glfwPollEvents();
    }

    //清除GLFW资源
    glfwTerminate();
    return 0;
}