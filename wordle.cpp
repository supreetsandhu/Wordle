

#include <glad.h>											// GL header file
#include <glfw3.h>											// GL toolkit
#include <stdio.h>											// printf, etc.
#include "GLXtras.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "Misc.h"

#include <float.h>
#include  "Text.h"
#include <stdio.h>
#include <stdlib.h>
#include "Draw.h"

using namespace std;

GLuint vBuffer = 0, program = 0;
char userInput[100] = { 0 };  // 2D array
//char userInput[6][100] = { 0,0,0,0,0,0 };
float ptSize = 20; // size of text
float spaceF = .2f; // inter-letter space is this fraction of letter size
string guessWord = " ";
bool wordEntered = false;
bool finished = false;
int rowC = 0;

float borderF = .1f; // border around game is this fraction of game size

float titleF = .1f; // height of title space is this fraction of window height
float letter = 0, letterH = 0, letterW =0, spaceW = 0, spaceH = 0;
float border = 0, borderW = 0, borderH =0, gameW = 0, gameH = 0, titleH = 0;
int winWidth = 1300, winHeight = 900;

void SetSizes(int, int);

vec3 bgroundColor(.7f, .5f, .1f), tileColor(.1f, .1, .1), textColor(.1, .2, 1);



float GetX(int col) { return borderW + col * (letterW + spaceW); }

float GetY(int row) { return borderH + (4 - row) * (letterH + spaceH); }



void DisplayUserInput(int row) {

	int nLetters = strlen(userInput);

	float y = GetY(row) + .5f * (letterH - ptSize);
		for (int col = 0; col < nLetters; col++) {

			float x = GetX(col) + .5f * (letterW - ptSize);

				Text(x, y, textColor, ptSize, "%c", userInput[col]);
				
			}
			
	}
/*
void DisplayUserInput(int row, int ck ) {

	int nLetters = strlen(userInput);

	float y = GetY(row) + .5f * (letterH - ptSize);
	for (int col = 0; col < nLetters; col++) {

		float x = GetX(col) + .5f * (letterW - ptSize);

		Text(x, y, (0,.4,1), ptSize, "%c", userInput[col]);

	}

}
	
	*/



void DisplayTitle(const char* title, float size) {

	float w = TextWidth(size, title);

	Text((winWidth - w) / 2.f, winHeight - titleH + size / 2, vec3(.3f, .2f, .5f), size, title);

}





void Resize(GLFWwindow* w, int width, int height) {

	glViewport(0, 0, winWidth = width, winHeight = height);

	SetSizes(5, 5);

}

void SetSizes(int nRows, int nCols) {

	gameW = winWidth / (1 + 2 * borderF);

	gameH = winHeight * (1 - titleF) / (1 + 2 * borderF);

	letterW = gameW / (nCols + (nCols - 1) * spaceH);

	letterH = gameH / (nRows + (nRows - 1) * spaceF);

	spaceW = letterW * spaceF;

	spaceH = letterH * spaceF;

	borderW = gameW * borderF;

	borderH = gameH * borderF;

	titleH = winHeight * titleF;

}



const char* vertexShader = R"(
	#version 130                                           
	in vec2 point;
	void main() { gl_Position = vec4(point, 0, 1); }
)";

const char* pixelShader = R"(
	#version 130                                           
	out vec4 pColor;                                       
	void main() { pColor = vec4(240, 100, 90, 1); }
)";

//function wrote by supreet
bool yellow(char input) {
		for (int j = 0; j < 5; j++) {
			if (input == guessWord[j]) {
		//		cout << " returning true bc" << guessWord[j] << " " << input << endl;
				return true;
			}
	}
	return false;

}

//function wrote by supreet
void processWord(string word) {
	cout << "word:" << word << "gues word:" << guessWord << endl;
	for (int i = 0; i < 5; i++) {
		if (word[i] == guessWord[i]) {
			cout << "green" << endl;
		}else if(yellow(word[i])) {
			cout << "yell" << endl;
			int x = GetX(i), y = GetY(0); // currentRow
			Quad(vec3(x, y, 0), vec3(x + letterW, y, 0), vec3(x + letterW, y + letterH, 0),
				vec3(x, y + letterH, 0), true, (.2,.5,.2));
			
		}else{
			cout << "grey" << endl;
		}
	}
}


void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {


	if (action == GLFW_PRESS) {

		int bufLen = strlen(userInput); // #chars in user input

		if (key == 259 && bufLen > 0) // backspace

			userInput[bufLen - 1] = 0;

		else if (bufLen < 5 && key >= 65 && key <= 90) { // a letter

			userInput[bufLen] = key;

			userInput[bufLen + 1] = 0;

		}
		else{
			cout << "done" << endl;
			wordEntered = true;
			rowC++;
			// processWord(userInput);

		}
		

	}

}




bool winner() {
	for (int i = 0; i < 5; i++) {
		if (userInput[i] != guessWord[i])
			return false;
	}
	return true;
}

const char* pixFile = "C:/Users/supis/winner.jpg";
float scale = .5;
GLuint textureName = 0, textureUnit = 0;

void Display() {

	glClearColor(bgroundColor.x, bgroundColor.y, bgroundColor.z, 1);

	glClear(GL_COLOR_BUFFER_BIT);

	UseDrawShader(ScreenMode());

	DisplayTitle("WORDLE!", 20);
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 5; col++) {
		
			int x = GetX(col), y = GetY(row);
	//		vec3 color = userInput[col] == guessWord[col] ? vec3(0, 1, 0) : yellow(userInput[col]) ? vec3(1, 1, 0) : vec3(.5);
			Quad(vec3(x, y, 0), vec3(x + letterW, y, 0), vec3(x + letterW, y + letterH, 0),
				 vec3(x, y + letterH, 0), true, (1,1,1));			 
		}
		DisplayUserInput(0);
		//if (wordEntered == true) {
			for (int col = 0; col < 5; col++) {
				int x = GetX(col), y = GetY(row);
				vec3 color = userInput[col] == guessWord[col] ? vec3(0, 1, 0) : yellow(userInput[col]) ? vec3(1, 1, 0) : vec3(.5);
				Quad(vec3(x, y, 0), vec3(x + letterW, y, 0), vec3(x + letterW, y + letterH, 0),
					vec3(x, y + letterH, 0), true, color);
		//	}
			
		}
	}
	

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
	SetSizes(5, 5);

	ifstream myfile;
	myfile.open("words.txt");

	string word;
	string wordArr[sizeFile];
	for (int i = 0; i < sizeFile; i++) {
		getline(myfile, word);
		wordArr[i] = word;
		cout << wordArr[i] << " is wordArr[" << i << "]" << endl;
	}
	guessWord = wordArr[rand() % sizeFile];
	cout << "Guess word is " << guessWord << endl;

	glfwInit();
	GLFWwindow* w = glfwCreateWindow(1300, 900, "Welcome to our Wordle game, Supreet + Hope!", NULL, NULL);
	glfwMakeContextCurrent(w);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
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