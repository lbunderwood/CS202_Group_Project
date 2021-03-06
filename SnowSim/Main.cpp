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

void mouseStrengthInput(GLFWwindow* window, float& s) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
		s -= 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
		s += 0.1;
	}
}

void cameraInput(GLFWwindow* window, float& theta) {
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		theta -= (M_PI / 180.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		theta += (M_PI / 180.0f);
	}
}

// sets field around cursor to blow particles away
void blowFromCursor(double xpos, double ypos, Field& field, const Field& baseField, float angle, float s)
{
	field = baseField;

	double realX = xpos / (windowSizeX / 4) - 2;
	double realY = ypos / (windowSizeY / -2) + 1;

	angle = fmod(angle, 2 * M_PI);

	  // FRONT
	if (abs(angle) < (M_PI / 4) || abs(angle) > (7 * M_PI / 4)) {
		field.setField(Vec3f(s, s, 0), Vec3f(realX, realY, -1), Vec3f(realX + 0.2, realY + 0.2, 1));
		field.setField(Vec3f(-s, s, 0), Vec3f(realX - 0.2, realY, -1), Vec3f(realX, realY + 0.2, 1));
		field.setField(Vec3f(-s, -s, 0), Vec3f(realX - 0.2, realY - 0.2, -1), Vec3f(realX, realY, 1));
		field.setField(Vec3f(s, -s, 0), Vec3f(realX, realY - 0.2, -1), Vec3f(realX + 0.2, realY, 1));
	} // BACK
	else if (abs(angle) > (3 * M_PI / 4) && abs(angle) < (5 * M_PI / 4)) {
		field.setField(Vec3f(s, s, 0), Vec3f(-realX, realY, -1), Vec3f(-realX + 0.2, realY + 0.2, 1));
		field.setField(Vec3f(-s, s, 0), Vec3f(-realX - 0.2, realY, -1), Vec3f(-realX, realY + 0.2, 1));
		field.setField(Vec3f(-s, -s, 0), Vec3f(-realX - 0.2, realY - 0.2, -1), Vec3f(-realX, realY, 1));
		field.setField(Vec3f(s, -s, 0), Vec3f(-realX, realY - 0.2, -1), Vec3f(-realX + 0.2, realY, 1));
	} // LEFT
	else if (angle >= (M_PI / 4) && angle <= (3 * M_PI / 4) ||
		angle <= (-5 * M_PI / 4) && angle >= (-7 * M_PI / 4)) {
		field.setField(Vec3f(0, s, s), Vec3f(-1, realY, realX), Vec3f(1, realY + 0.2, realX + 0.2));
		field.setField(Vec3f(0, s, -s), Vec3f(-1, realY, realX - 0.2), Vec3f(1, realY + 0.2, realX));
		field.setField(Vec3f(0, -s, -s), Vec3f(-1, realY - 0.2, realX - 0.2), Vec3f(1, realY, realX));
		field.setField(Vec3f(0, -s, s), Vec3f(-1, realY - 0.2, realX), Vec3f(1, realY, realX + 0.2));
	} // RIGHT
	else {
		field.setField(Vec3f(0, s, s), Vec3f(-1, realY, -realX), Vec3f(1, realY + 0.2, -realX + 0.2));
		field.setField(Vec3f(0, s, -s), Vec3f(-1, realY, -realX - 0.2), Vec3f(1, realY + 0.2, -realX));
		field.setField(Vec3f(0, -s, -s), Vec3f(-1, realY - 0.2, -realX - 0.2), Vec3f(1, realY, -realX));
		field.setField(Vec3f(0, -s, s), Vec3f(-1, realY - 0.2, -realX), Vec3f(1, realY, -realX + 0.2));
	}
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

	float theta = 0;

	float mouseStrength = 2;

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
				blowFromCursor(xpos, ypos, forceField, baseField, theta, mouseStrength);
			}
		}
		else
		{
			forceField = baseField;
		}

		checkInput(window, forceField, baseField, perlin);

		mouseStrengthInput(window, mouseStrength);

		cameraInput(window, theta);

		processInput(window);
		
		float radius = 3.75;
		float camX = radius * std::sin(theta);
		float camY = 0.0f;
		float camZ = radius * std::cos(theta);
		Vec3f dir = (Vec3f(camX, camY, camZ) - Vec3f(0.0f, 0.0f, 0.0f)).unit();
		Vec3f tempUp(0.0f, 1.0f, 0.0f);
		Vec3f right = tempUp.cross(dir);
		Vec3f up = dir.cross(right);
		float viewX = (right.x_ * camX + right.y_ * camY + right.z_ * camZ);
		float viewY = (up.x_ * camX + up.y_ * camY + up.z_ * camZ);
		float viewZ = (dir.x_ * camX + dir.y_ * camY + dir.z_ * camZ);

		// View Matrix (Camera Pos.)
		float view[16] = {
			right.x_, right.y_, right.z_, 0,
			   up.x_,    up.y_,    up.z_, 0,
			  dir.x_,   dir.y_,   dir.z_, 0,
			  -viewX,   -viewY,   -viewZ, 1 };

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
		float m00 = n / r;
		float m11 = n / t;
		float m22 = (f + n) / (n - f);
		float m23 = 2 * f * n / (n - f);
		float projection[16] = {
			m00, 0,   0,   0,
			0, m11,   0,   0,
			0,   0, m22,  -1,
			0,   0, m23,   0 };

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