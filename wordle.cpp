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

using namespace std;

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



int main() {
	srand(time(NULL));
	int const sizeFile = 6;
	ifstream myfile;
	myfile.open ("words.txt");
	
	string word;
	string wordArr[sizeFile];
	for (int i = 0;i < sizeFile;i++) {
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
	GLuint program = LinkProgramViaCode(&vertexShader, &pixelShader);
	glUseProgram(program);
	GLuint vBuffer = 0;
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	float pts[][2] = { {-1,-1}, {-1,1}, {1,1}, {1,-1} };
	glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);
	VertexAttribPointer(program, "point", 2, 0, (void*)0);
	while (!glfwWindowShouldClose(w)) {
		glDrawArrays(GL_QUADS, 0, 4);
		glFlush();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwDestroyWindow(w);
	glfwTerminate();
}