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
#include "Field.h"
#include <iostream>
#include <vector>
#include <random>


const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float yOffset;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n";


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
		Particle newParticle(dist(gen), dist(gen) * 0.1 + 1.0, dist(gen));
		particles.push_back(newParticle);
	}

	std::vector<float> vertices;
	for (auto p : particles) {
		p.pushData(vertices);
	}

	/*for (auto v : vertices) {
		std::cout << v << "\n";
	}*/

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

	glPointSize(2.0f);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	const double dt = 1.0 / 60.0;

	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	while (!glfwWindowShouldClose(window)) {
		int end = particles.size();
		for (int i = 0; i < end; ++i) {
			if (particles[i].checkBounds()) {
				particles.erase(particles.begin() + i);
				i--;
				end--;
			}
		}

		for (int i = 0; i < 100; i++) {
			Particle newParticle(dist(gen), dist(gen) * 0.1 + 1.0, dist(gen));
			particles.push_back(newParticle);
		}

		processInput(window);


		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while(accumulator >= dt) {
			vertices.clear();
			for (size_t i = 0; i < particles.size(); ++i) {
				particles[i].update(dt);
				particles[i].pushData(vertices);
			}

			accumulator -= dt;
		}


		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STREAM_DRAW);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, particles.size());

		glfwSwapBuffers(window);
		glfwPollEvents();

		glBindVertexArray(0);

		//std::cout << "N = " << particles.size() << "\n";
	}

	glfwTerminate();
	return 0;
}