#shader vertex
#version 330 core
layout(location = 0) in vec4 position; //顶点位置

void main()
{
	gl_Position = position; //将顶点位置转换为裁剪空间坐标
}
#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

uniform vec4 u_Color; //统一变量，用于传递颜色

void main()
{
	color = u_Color; //设置片段颜色为红色
}