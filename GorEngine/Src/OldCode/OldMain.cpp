//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//
////insert breakpoint
//#define ASSERT(x) if (!(x)) __debugbreak();
////clear error, execute function and then log error with assert to have a breakpoint
//#define GLCall(x) GLClearError();\
//	x;\
//	ASSERT(GLLogCall(#x, __FILE__, __LINE__)); //# turn function to string
//
////clear error stack
//static void GLClearError()
//{
//	while (glGetError() != GL_NO_ERROR);
//}
////Log the error
//static bool GLLogCall(const char* function, const char* file, int line)
//{
//	while (GLenum error = glGetError())
//	{
//		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << " : " << line << std::endl;
//
//		return false;
//	}
//
//	return true;
//}
//
////struct to store the shaders (more easy to return in reading functions
//struct ShaderProgramSource
//{
//	std::string VertexShader;
//	std::string FragmentShader;
//};
//
//static ShaderProgramSource ParseShader(const std::string& filePath)
//{
//	enum class ShaderType {
//		NONE = -1, VERTEX = 0, FRAGMENT = 1
//	};
//
//	std::ifstream stream(filePath);
//	std::string line;
//	std::stringstream ss[2];
//	ShaderType type = ShaderType::NONE;
//
//
//	while (getline(stream, line))
//	{
//		if (line.find("#shader") != std::string::npos)
//		{
//			if (line.find("vertex") != std::string::npos)
//				type = ShaderType::VERTEX;
//			else if (line.find("fragment") != std::string::npos)
//				type = ShaderType::FRAGMENT;
//		}
//		else
//		{
//			ss[(int)type] << line << '\n';
//		}
//	}
//
//	// build & return the struct
//	return { ss[0].str(), ss[1].str() };
//}
//
//static unsigned int CompileShader(unsigned int type, const std::string& source)
//{
//	unsigned int id = glCreateShader(type);
//	const char* src = source.c_str(); // or &source[0]
//	GLCall(glShaderSource(id, 1, &src, nullptr));
//	glCompileShader(id);
//
//	int result;
//	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//	if (result == GL_FALSE)
//	{
//		int length;
//		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//		char* message = (char*)alloca(length * sizeof(char));
//		glGetShaderInfoLog(id, length, &length, message);
//		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader !" << std::endl;
//		std::cout << message << std::endl;
//		glDeleteShader(id);
//		return 0;
//	}
//	return id;
//}
//
//static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//	unsigned int program = glCreateProgram();
//	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//	//attach shader in a program
//	GLCall(glAttachShader(program, vs));
//	GLCall(glAttachShader(program, fs));
//	GLCall(glLinkProgram(program));
//	GLCall(glValidateProgram(program));
//
//	//we don't need shder file anymor until they are linked with program
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//
//	return program;
//}
//
//int oldmain(void)
//{
//	GLFWwindow* window;
//
//	/* Initialize the library */
//	if (!glfwInit())
//		return -1;
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	/* Create a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//	//slow down frame rate
//	glfwSwapInterval(5);
//
//	/* After we create a valid context, we can init GLEW */
//	if (glewInit() != GLEW_OK)
//		std::cout << "Error GLEW init" << std::endl;
//
//	/* Print version of OpenGL */
//	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
//
//
//	/* Create a rectangle */
//	float positions[] = {
//		-0.5f, -0.5f, // 0
//		 0.5f, -0.5f, // 1
//		 0.5f,  0.5f, // 2
//		-0.5f,  0.5f  // 3
//	};
//	//index buffer
//	unsigned int indices[] = {
//		0, 1, 2,
//		2, 3, 0
//	};
//
//	//Create and bind vertex array object
//	unsigned int vao;
//	GLCall(glGenVertexArrays(1, &vao));
//	GLCall(glBindVertexArray(vao));
//
//	//generate vertex buffer
//	unsigned int buffer; // id of buffer
//	//create one buffer and store the id
//	GLCall(glGenBuffers(1, &buffer)); // 1 is the number of buffer
//	//bind vertex buffer
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); //select vertex buffer
//	GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW)); //define size of buffer
//	//specify layout
//	GLCall(glEnableVertexAttribArray(0));//enable vertex array on index 0
//	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));  //we store vertex attribute to index 0, each vertex attribute have 2 components, with type of float,, we dont normalise coordianate, we determine the siez between each vertex attribute (each vertex attribute is caraterize by 2float, so the these between attribute is 2*float),  then we give the position were the vertex data begins it's useful in case  in our array we store other datas like uvcoord or normal, we tell were the data we want to exploite begin
//
//	//Create and bind index buffer object (IBO)
//	unsigned int ibo;
//	GLCall(glGenBuffers(1, &ibo));
//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); //select buffer
//	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); //define size of buffer
//
//	/* Shader Config */
//	//relative to project file (defined by properties "Debugging -> Working Directory")
//	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
//	unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);
//	GLCall(glUseProgram(shader));
//
//	//retrieve id of uniform
//	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
//	ASSERT(location != -1);
//	//use id to set values 
//	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.f));
//
//
//
//	//Unbind all buffer and array and shader
//	GLCall(glBindVertexArray(0));
//	GLCall(glUseProgram(0));
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//
//
//	float r = 0.f;
//	float increment = 0.05f;
//	/* Loop until the user closes the window */
//	while (!glfwWindowShouldClose(window))
//	{
//		/* Render here */
//		GLCall(glClear(GL_COLOR_BUFFER_BIT));
//
//		GLCall(glUseProgram(shader));
//		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.f));
//
//		//Bind vertex array and index buffer (no need to bind vertex buffer with vertex array)
//		GLCall(glBindVertexArray(vao));
//		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
//
//
//		//animate color with uniform
//		if (r > 1.0f)
//			r = 0.f;
//		else
//			r += increment;
//
//		//we give the start index of array, and the number of vertices to draw
//		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//		//we check error
//
//		/* Swap front and back buffers */
//		GLCall(glfwSwapBuffers(window));
//
//		/* Poll for and process events */
//		GLCall(glfwPollEvents());
//	}
//
//	glDeleteProgram(shader);
//	glfwTerminate();
//	return 0;
//}