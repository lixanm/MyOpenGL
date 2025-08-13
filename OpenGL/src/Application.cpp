#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


//编译着色器k
//先指定类型，再指定着色器源代码
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //将std::string转换为const char*类型

	glShaderSource(id, 1, &src, nullptr); //将着色器源代码附加到着色器对象上
    glCompileShader(id);//编译着色器

    //TODO:
    int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //获取编译状态
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //获取错误信息长度
        char* message = (char*)alloca(length * sizeof(char)); //分配内存
        glGetShaderInfoLog(id, length, &length, message); //获取错误信息
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER?"vertex":"fragment") << std::endl;
        std::cout << message << std::endl; //输出错误信息
        glDeleteShader(id); //删除着色器对象
		return 0; //返回0表示编译失败
    }


    return id;
}

//静态，不会泄露到其他翻译单元
//提供实际着色器源代码，链接定点着色器和片段着色器，返回标识符
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //创建一个程序对象

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //创建顶点着色器对象,确定着色器类型为顶点着色器
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


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
    //需要创建一个有效的 OpenGL 渲染上下文并调用 glewInit() 初始化扩展入口点
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
        -0.5f, -0.5f,  // 左下角
         0.0f,  0.5f,  // 顶部
         0.5f, -0.5f  // 右下角
	};

	unsigned int buffer; //顶点缓冲区对象的ID

	glGenBuffers(1, &buffer); //生成一个顶点缓冲区并返回一个ID，参数1表示生成一个缓冲区，&buffer是存储生成的缓冲区ID的变量地址

	glBindBuffer(GL_ARRAY_BUFFER, buffer); //绑定顶点缓冲区对象（想要绑定缓冲区，只需绑定缓冲区ID即可）

    //缓冲区实际布局
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),position,GL_STATIC_DRAW); //分配缓冲区空间,将数据放入缓冲区中


	glEnableVertexAttribArray(0); //启用顶点属性数组，0表示顶点属性的位置（在着色器中定义）

    // 设置顶点属性指针
    //索引：0，这是顶点属性的位置，
    // 2表示每个顶点有两个分量（x和y），
    // GL_FLOAT表示数据类型，GL_FALSE表示不需要归一化，2 * sizeof(float)表示每个顶点的步长，(void*)0表示从缓冲区的起始位置开始读取数据
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void* const)0);

	std::string vertexShader = R"(
        #version 330 core

        layout(location = 0) in vec4 position; //顶点位置
        void main()
        {
            gl_Position = position; //将顶点位置转换为裁剪空间坐标
        }
    )"; 
    std::string fragmentShader = R"(
        #version 330 core

        layout(location = 0) out vec4 color; 
        void main()
        {
            color = vec4(1.0, 0.0, 0.0, 1.0); //设置片段颜色为红色
        }
    )";

	unsigned int shader = CreateShader(vertexShader, fragmentShader); //创建着色器程序
	glUseProgram(shader); //使用着色器程序


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

	glDeleteProgram(shader); //删除着色器程序

    //清除GLFW资源
    glfwTerminate();
    return 0;
}