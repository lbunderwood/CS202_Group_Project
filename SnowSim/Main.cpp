// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Main.cpp
// main source file for SnowSim

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vec3f.h"
#include "Particle.h"
#include <iostream>
#include <vector>
#include <random>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	std::vector<Particle> particles;
	for (int i = 0; i < 100; i++) {
		Particle newParticle(dist(gen), dist(gen), dist(gen));
		particles.push_back(newParticle);
	}

	std::vector<GLfloat> vertices;
	for (auto p : particles) {
		std::vector<float> pos = p.getPos();
		/*std::cout << "x: " << pos[0];
		std::cout << "y: " << pos[1];
		std::cout << "z: " << pos[2];
		std::cout << "\n";*/
		vertices.push_back(pos[0]);
		vertices.push_back(pos[1]);
		vertices.push_back(pos[2]);
	}

	for (auto v : vertices) {
		std::cout << v << "\n";
	}

	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "TestWindow", NULL, NULL);
	if (window == NULL) {
		std::cout << "failed to create windows" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);

	glViewport(0, 0, 1280, 720);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		glColor3f(1.0f, 1.0f, 1.0f);
		glPointSize(10.0f);
		/*glBegin(GL_POINTS);

		for (auto p : particles) {
			std::vector<float> pos = p.getPos();
			glVertex3f(pos[0], pos[1], pos[2]);
		}

		glEnd();*/

		glDrawArrays(GL_POINTS, 0, 100);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}