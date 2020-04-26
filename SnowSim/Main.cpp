// Luke Underwood and Andrew Ng
// 2/27/2020
// CS202_Group_Project
// SnowSim
// Main.cpp
// main source file for SnowSim

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vec3f.h"
#include "Particle.h"
#include "Field.h"
#include "Modes.h"
#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <cmath>

namespace
{
	// Current window size
	int windowSizeX{ 1280 };
	int windowSizeY{ 720 };
}

// Code for vertex shader
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Code for fragment shader
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n";


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowSizeX = width;
	windowSizeY = height;
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// sets field around cursor to blow particles away
void blowFromCursor(double xpos, double ypos, Field& field, const Field& baseField)
{
	field = baseField;

	double realX = xpos / (windowSizeX / 2) - 1;
	double realY = ypos / (windowSizeY / -2) + 1;

	field.setField(Vec3f(1, 2, 0), Vec3f(realX, realY, -1), Vec3f(realX + 0.2, realY + 0.2, 1));
	field.setField(Vec3f(-1, 2, 0), Vec3f(realX - 0.2, realY, -1), Vec3f(realX, realY + 0.2, 1));
	field.setField(Vec3f(-1, -2, 0), Vec3f(realX - 0.2, realY - 0.2, -1), Vec3f(realX, realY, 1));
	field.setField(Vec3f(1, -2, 0), Vec3f(realX, realY - 0.2, -1), Vec3f(realX + 0.2, realY, 1));
}


int main()
{
	// Random number generator for testing particle emission
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	// Linked list containing Particle objects
	// Testing list for faster element erasing
	// Initialized with 100 Particles
	std::list<Particle> particles;
	for (int i = 0; i < 100; i++) {
		particles.push_back(Particle(dist(gen), dist(gen) * 0.1f + 0.9f, dist(gen)));
	}

	// vector<float> vertices
	// contains particle data values
	// (what is actually passed to OpenGL functions)
	// pushData method pushes Particle data into a vector
	std::vector<float> vertices;
	for (auto p : particles) {
		p.pushData(vertices);
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


	// Enabling depth testing and circular points
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);

	glViewport(0, 0, 1280, 720);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Size of rendered points
	glPointSize(0.01f);

	// View Matrix (Camera Pos.)
	float view[16] = {
		1,   0,   0,   0,
		0,   1,   0,   0,
		0,   0,   1,   0,
		0,   0,-4.0,   1 };

	// Model Matrix
	float model[16] = {
		1,   0,   0,   0,
		0,   1,   0,   0,
		0,   0,   1,   0,
		0,   0,   0,   1 };

	std::vector<float> box{
		1, 1, 1, -1, 1, 1,
		-1, 1, 1, -1, 1, -1,
		-1, 1, -1, 1, 1, -1,
		1, 1, -1, 1, 1, 1,

		1, -1, 1, -1, -1, 1,
		-1, -1, 1, -1, -1, -1,
		-1, -1, -1, 1, -1, -1,
		1, -1, -1, 1, -1, 1,

		1, 1, 1, 1, -1, 1,
		-1, 1, 1, -1, -1, 1,
		-1, 1, -1, -1, -1, -1,
		1, 1, -1, 1, -1, -1,
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	unsigned int vao2;
	glGenVertexArrays(1, &vao2);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	unsigned int vbo2;
	glGenBuffers(1, &vbo2);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, box.size() * sizeof(float), &box.front(), GL_STATIC_DRAW);
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

	// Timestep for 60 updates/sec
	const double dt = 1.0 / 60.0;

	// Setup for time accumulator
	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	// set up force field
	Field forceField;

	// PERLIN NOISE START
	// perlinMode(forceField);
	// PERLIN NOISE END
	
	// VORTEX FIELD START
	// vortexMode(forceField);
	// VORTEX FIELD END

	modeMenu();

	bool perlin = false;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	// Base Field to reset to - MAKE SURE THIS IS THE LAST THING BEFORE MAIN LOOP
	Field baseField = forceField;

	while (!glfwWindowShouldClose(window)) {

		// Adds some number of particles each loop
		for (int i = 0; i < 100; i++) {
			if (perlin)
			{
				particles.push_back(Particle(dist(gen) * 0.05, dist(gen) * 0.05f, dist(gen) * 0.05));
			}
			else
			{
				particles.push_back(Particle(dist(gen), dist(gen) * 0.1f + 0.9f, dist(gen)));
			}
		}

		// Mouse interaction
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			if (xpos >= 0 && xpos <= windowSizeX && ypos >= 0 && ypos <= windowSizeY)
			{
				blowFromCursor(xpos, ypos, forceField, baseField);
			}
		}
		else
		{
			forceField = baseField;
		}

		checkInput(window, forceField, baseField, perlin);

		processInput(window);

		// More stuff for time accumulator
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		// Once time accumulated passes timestep
		// update particles
		while(accumulator >= dt) {

			// clear vertices vector
			vertices.clear();

			// For each Particle in linked list:
			// update position using dt timestep
			// if out of bounds, erase Particle
			// push data into vertices format for OpenGL
			for (auto it = particles.begin(); it != particles.end(); it++) {
				it->update(dt, forceField);
				if (it->checkBounds()) {
					it = particles.erase(it);
					//it--;
					continue;
				}
				it->pushData(vertices);
			}

			// subtract from time accumulated
			accumulator -= dt;
		}


		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
		// Constructing perspective projection matrix
		// based on near, far, Y FOV, aspect ratio
		int w = 1;
		int h = 1;
		glfwGetWindowSize(window, &w, &h);
		float aspectRatio = (float)w / (float)h;
		float fovY = 45.0f * M_PI / 180.0f;
		float n = 0.01f;
		float f = 10.0f;
		float t = std::tan(fovY * 0.5f) * n;
		float b = -t;
		float r = t * aspectRatio;
		float l = -r;
		float m11 = n / r;
		float m22 = n / t;
		float m33 = (f + n) / (n - f);
		float m34 = 2 * f * n / (n - f);
		float projection[16] = {
			m11, 0,   0,   0,
			0, m22,   0,   0,
			0,   0, m33,  -1,
			0,   0, m34,   0 };

		int modelLoc = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

		int viewLoc = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);

		int projectionLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STREAM_DRAW);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);

		// Draw points
		glDrawArrays(GL_POINTS, 0, particles.size());

		glBindVertexArray(vao2);
		glDrawArrays(GL_LINES, 0, box.size());

		glfwSwapBuffers(window);
		glfwPollEvents();

		glBindVertexArray(0);

	}

	glfwTerminate();
	return 0;
}