//Wordle.cpp

#include <glad.h>											// GL header file
#include <glfw3.h>											// GL toolkit
#include <stdio.h>											// printf, etc.
#include "GLXtras.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include  "Text.h"

using namespace std;

GLuint vBuffer = 0, program = 0;

const char* vertexShader = R"(
	#version 130                                           
	in vec2 point;
	void main() { gl_Position = vec4(point, 0, 1); }
)";

const char* pixelShader = R"(
	#version 130                                           
	out vec4 pColor;                                       
	void main() { pColor = vec4(255, 255, 255, 1); }
)";

void Display() {
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	VertexAttribPointer(program, "point", 2, 0, (void*)0);
	glDrawArrays(GL_QUADS, 0, 4);
	Text(500, 800, vec3(1, 1, 0), 48, "WORDLE!");
	glFlush();
}

void InitVertexBuffer() {
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	vec2 pts[] = { {-1,-1}, {-1,1}, {1,1}, {1,-1} };
	glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);
}

int main() {
	srand(time(NULL));
	int const sizeFile = 6;

	ifstream myfile;
	myfile.open("words.txt");

	string word;
	string wordArr[sizeFile];
	for (int i = 0; i < sizeFile; i++) {
		getline(myfile, word);
		wordArr[i] = word;
		cout << wordArr[i] << " is wordArr[" << i << "]" << endl;
	}
	string guessWord = wordArr[rand() % sizeFile];
	cout << "Guess word is " << guessWord << endl;

	glfwInit();
	GLFWwindow* w = glfwCreateWindow(1300, 900, "Welcome to our Wordle game, Supreet + Hope!", NULL, NULL);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	InitVertexBuffer();
	while (!glfwWindowShouldClose(w)) {
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwDestroyWindow(w);
	glfwTerminate();

}
