#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include<fstream>//文件流
#include <string>//字符串
#include <sstream>//字符串流

//#include "imGui.h"




#define ASSERT(x) if (!(x)) __debugbreak(); //断言宏，如果条件不成立，则触发异常

#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x,__FILE__,__LINE__)); //宏定义，清除错误并检查错误

static void GLClearError() //清除OpenGL错误
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) //检查OpenGL错误
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true; //返回true表示没有错误
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)//当用户改变窗口的大小的时候，视口也应该被调整
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)//处理输入
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath); //打开文件流
	std::string line;//行
	std::stringstream ss[2]; //创建两个字符串流，分别存储顶点着色器和片段着色器
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};//枚举类型，表示着色器类型
	ShaderType type = ShaderType::NONE; //初始类型为NONE
	while (getline(stream, line)) //逐行读取文件内容
	{
		if (line.find("#shader") != std::string::npos) //查找"#shader"关键字
		{
			if (line.find("vertex") != std::string::npos) //如果是顶点着色器
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos) //如果是片段着色器
				type = ShaderType::FRAGMENT;
		}
		else //如果不是"#shader"关键字
		{
			ss[(int)type] << line << "\n"; //将当前行添加到对应的字符串流中
		}
	}

	return { ss[0].str(), ss[1].str() }; //返回顶点着色器和片段着色器的源代码
}

//编译着色器k
//先指定类型，再指定着色器源代码
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	//创建一个着色器对象，注意还是用ID来引用的。所以我们储存这个顶点着色器为unsigned int，然后用glCreateShader创建这个着色器
	unsigned int id = glCreateShader(type);//创建一个着色器对象，返回一个非零的标识符

	const char* src = source.c_str(); //将std::string转换为const char*类型


	//把这个着色器源码附加到着色器对象上，然后编译它
	//要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。
	// 第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO:
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //获取编译状态
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //获取错误信息长度
		char* message = (char*)alloca(length * sizeof(char)); //分配内存
		glGetShaderInfoLog(id, length, &length, message); //获取错误信息
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
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
	unsigned int program = glCreateProgram(); //创建一个程序对象，程序对象是可以附加着色器对象的对象

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //创建顶点着色器对象,确定着色器类型为顶点着色器
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);//将着色器对象附加到程序对象
	glAttachShader(program, fs);
	glLinkProgram(program);//成功链接程序对象
	glValidateProgram(program);

	glDeleteShader(vs);//删除着色器
	glDeleteShader(fs);

	return program;
}


int main(int, char**)
{


	GLFWwindow* window;//窗口指针

	//初始化GLFW库
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//配置glfw
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//3.3版本
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//苹果系统

	//创建一个窗口及其OpenGL上下文
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);/*监测（窗口模式），共享（不共享资源）*/

	if (!window)
	{
		//如果创建失败，终止GLFW并返回错误
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//此函数使指定窗口的 OpenGL 或 OpenGL ES 上下文在调用线程上为当前上下文



	glfwSwapInterval(1);//交换间隔为1；


	//创建上下文后初始化GLEW
	if (glewInit() != GLEW_OK)
		std::cout << "Error initializing GLEW" << std::endl;

	//输出OpenGL版本信息
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//设置视口大小
	glViewport(0, 0, 640, 480);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//每次调整窗口时调用里面的函数


	//顶点数组对象：Vertex Array Object，VAO
	//顶点缓冲对象：Vertex Buffer Object，VBO
	//索引缓冲对象 Index Buffer Object，IBO


	//顶点缓冲区 //输入顶点数据
	//OpenGL仅当3D坐标在3个轴（x、y和z）上-1.0到1.0的范围内时才处理它，这些坐标被成为标准化设备坐标
	float position[] = {
		-0.5f, -0.5f, 0.0f,  // 左下角
		 0.5f, -0.5f, 0.0f,  // 顶部
		 0.5f,  0.5f, 0.0f,  // 右下角

		-0.5f,  0.5f, 0.0f   // 左上角
	};
	//索引缓冲区
	unsigned int indices[] = { //索引数组
		0, 1, 2, // 第一个三角形
		2, 3, 0  // 第二个三角形
	};

	//这个就是顶点缓冲对象
	unsigned int buffer;//地址
	glGenBuffers(1, &buffer);//在缓冲区中返回 n 个缓冲区对象名称，&buffer是指定存储生成的缓冲区对象名称的数组
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //将缓冲区对象绑定到指定的缓冲区绑定点，即将顶点缓冲区对象绑定到GL_ARRAY_BUFFER绑定点，这样OpenGL就知道接下来要操作哪个缓冲区对象
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), position, GL_STATIC_DRAW);
	//创建并初始化缓冲区对象的数据存储
	//（指定目标缓冲区对象，指定缓冲区对象的新数据存储的大小，指定指向将复制到数据存储中进行初始化的数据的指针，指定数据存储的预期使用模式）



	glEnableVertexAttribArray(0); //启用顶点属性数组，0表示通用顶点属性的索引
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void* const)0);
	//定义通用顶点属性数据的数组
	//（指定要修改的通用顶点属性的索引，
	// 指定每个通用顶点属性的组件数，
	// 指定数组中每个组件的数据类型，
	// 指定连续通用顶点属性之间的字节偏移量，
	// 指定当前绑定到 GL_ARRAY_BUFFER 目标的缓冲区的数据存储中数组中第一个泛型顶点属性的第一个组件的偏移量。初始值为 0。
	// （(void*)0表示从缓冲区的起始位置开始读取数据））

	//索引缓冲对象
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);



	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); //解析着色器文件，获取顶点着色器和片段着色器的源代码
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); //创建着色器程序
	glUseProgram(shader); //使用着色器程序，这些可执行文件成为当前状态的一部分

	unsigned int location = glGetUniformLocation(shader, "u_Color"); //获取片段着色器中颜色变量的地址
	ASSERT(location != -1);
	glUniform4f(location, 0.5f, 0.2f, 0.8f, 1.0f); //设置片段着色器中的颜色变量




	float r = 0.0f;
	float increment = 0.5f;
	//主渲染循环
	while (!glfwWindowShouldClose(window))//GLfw是否被要求退出
	{


		processInput(window);//处理输入

		////渲染指令


		//当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
		//深蓝绿色：背景颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//每次循环开始清除颜色缓冲区
		glClear(GL_COLOR_BUFFER_BIT);


		glUniform4f(location, r, 0.2f, 0.8f, 1.0f); //设置片段着色器中的颜色变量


		//为缓冲区发出绘制调用，两个办法：glDrawArrays()(没有索引缓冲器) 或 glDrawElements()
		//glDrawArrays(GL_TRIANGLES, 0, 6); //绘制三角形，参数分别为绘制模式、起始索引、顶点数量
		//使用索引缓冲区绘制
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //绘制三角形，参数分别为绘制模式、索引数量、索引类型、索引偏移量



		if (r > 1.0f)increment = -0.05f;
		else if (r < 0.0f)increment = 0.05f;

		r += increment;




		//// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window); //避免画面撕裂，交换颜色缓冲
		glfwPollEvents();//处理事件
	}

	glDeleteProgram(shader); //删除着色器程序


	glfwDestroyWindow(window);
	glfwTerminate();//清除GLFW资源
	return 0;

}