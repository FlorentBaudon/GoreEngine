//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//
//int CreateGLWindow(GLFWwindow** windowRef)
//{
//	// 4x antialiasing
//	glfwWindowHint(GLFW_SAMPLES, 4); 
//	//Open GL 3.3
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	//Retro compatibility
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	*windowRef = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
//
//	if (*windowRef == NULL) {
//		std::cout << "Failed to open  GLFW window.\n";
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(*windowRef);
//
//	glewExperimental = true;
//	if (glewInit() != GLEW_OK) {
//		std::cout << "Failed to initialize GLEW\n";
//		return -1;
//	}
//
//	glOrtho(0, 800, 600, 0, 0.001, 10000);
//
//	return 1;
//}
//
//
//void drawPlayer(float px, float py)
//{
//	glColor3f(1, 1, 0);
//	glPointSize(8);
//	glBegin(GL_POINTS);
//	glVertex2i(px, py);
//	glEnd();
//}
//void processInput(GLFWwindow* w) 
//{
//	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(w, true);
//}
//
//int main(void)
//{
//	float px = 300, py = 300;
//
//	if (!glfwInit())
//	{
//		std::cout << "Failed to init GLFW" << std::endl;
//		return -1;
//	}
//
//
//	GLFWwindow* window;
//	int result = CreateGLWindow(&window);
//
//	//If any errors in init windows, we close app
//	if (result != 1) {
//		return result;
//	}
//
//	while (!glfwWindowShouldClose(window))
//	{
//		processInput(window);
//
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		drawPlayer(px, py);
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}