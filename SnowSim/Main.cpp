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
#include <list>
#include <random>

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
"uniform float yOffset;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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

// callback to manipulate field using cursor. Needs access to field, so it's here
void blowFromCursor(double xpos, double ypos, Field& field)
{
	static Field previousField = field;
	field = previousField;

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
		Particle newParticle(Particle(dist(gen) * 0.05, dist(gen) * 0.05f + 0.95f, dist(gen) * 0.05));
		particles.push_back(newParticle);
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

	// Timestep for 60 updates/sec
	const double dt = 1.0 / 60.0;

	// Setup for time accumulator
	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	// set up force field
	Field forceField;

	// PERLIN NOISE START
	/*Field gradients(24, 24, 24);
	gradients.genGradients();
	forceField.addPerlin(gradients);*/
	// PERLIN NOISE END
	
	// VORTEX FIELD START
	/*forceField.setWind(Vec3f(-0.5f, 0.0f, -1.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
	forceField.setWind(Vec3f(-1.0f, 0.0f, 0.5f), Vec3f(0.0f, 0.0f, -1.0f), Vec3f(1.0f, 1.0f, 0.0f));
	forceField.setWind(Vec3f(0.5f, 0.0f, 1.0f), Vec3f(-1.0f, 0.0f, -1.0f), Vec3f(0.0f, 1.0f, 0.0f));
	forceField.setWind(Vec3f(1.0f, 0.0f, -0.5f), Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 1.0f));

	forceField.setWind(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(1.0f, 0.0f, 1.0f));*/
	// VORTEX FIELD END

	// Base Field to reset to - MAKE SURE THIS IS THE LAST THING BEFORE MAIN LOOP
	Field baseField = forceField;

	while (!glfwWindowShouldClose(window)) {

		// Adds some number of particles each loop
		// PERLIN NOISE START
		/*for (int i = 0; i < 100; i++) {
			particles.push_back(Particle(dist(gen) * 0.05, dist(gen) * 0.05f + 0.95f, dist(gen) * 0.05));
		}*/
		// PERLIN NOISE END

		//GENERIC SNOWFALL START
		for (int i = 0; i < 3; i++) {
			particles.push_back(Particle(dist(gen), dist(gen) * 0.1f + 0.9f, dist(gen)));
		}
		//GENERIC SNOWFALL END

		// Mouse interaction
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			if (xpos >= 0 && xpos <= windowSizeX && ypos >= 0 && ypos <= windowSizeY)
			{
				blowFromCursor(xpos, ypos, forceField);
			}
		}
		else
		{
			forceField = baseField;
		}

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

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STREAM_DRAW);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);

		// Draw points
		glDrawArrays(GL_POINTS, 0, particles.size());

		glfwSwapBuffers(window);
		glfwPollEvents();

		glBindVertexArray(0);

	}

	glfwTerminate();
	return 0;
}