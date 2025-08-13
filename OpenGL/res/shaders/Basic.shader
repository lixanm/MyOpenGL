
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
void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0); //设置片段颜色为红色
}