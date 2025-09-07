//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
#include <tuple> // ← NECESARIO para usar std::tuple y std::tie para manejar las caras de la pirámide y dividirlas

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
//piramide triangular
MeshColor* meshColor = nullptr; // Piramide Rubik coloreada para cada una de sus caras jiji

void CrearPiramideColores()
{
	std::vector<GLfloat> vertices;

	// Vértices de la pirámide
	glm::vec3 V0 = { -0.5f, 0.0f, -0.289f }; // base
	glm::vec3 V1 = { 0.5f, 0.0f, -0.289f };
	glm::vec3 V2 = { 0.0f, 0.0f,  0.577f };
	glm::vec3 V3 = { 0.0f, 0.7f,  0.0f };   // punta

	// Caras de la pirámide
	std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> caras = {
		{V0, V1, V2}, // base
		{V0, V1, V3},
		{V1, V2, V3},
		{V2, V0, V3}
	};

	// Colores para cada cara
	std::vector<glm::vec3> colores = {
		{1.0f, 0.0f, 0.0f},   // rojo
		{0.0f, 0.50f, 0.0f},   // verde
		{0.0f, 0.0f, 1.0f},   // azul
		{1.0f, 1.0f, 0.0f}    // amarillo
	};

	glm::vec3 negro = { 0.0f, 0.0f, 0.0f }; //Color de las lineas que dividen los tringulitos 
	float grosor = 0.02f;
	int subdiv = 3;//para crear mas divisiones

	auto add_triangle = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color) {
		for (auto& v : { a, b, c }) {
			vertices.push_back(v.x);
			vertices.push_back(v.y);
			vertices.push_back(v.z);
			vertices.push_back(color.r);
			vertices.push_back(color.g);
			vertices.push_back(color.b);
		}
		};

	auto add_borde = [&](glm::vec3 A, glm::vec3 B) {
		glm::vec3 dir = glm::normalize(B - A);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 side = glm::normalize(glm::cross(dir, up));
		if (glm::length(side) < 0.001f)
			side = glm::normalize(glm::cross(dir, glm::vec3(1.0f, 0.0f, 0.0f)));
		side *= grosor;

		glm::vec3 A1 = A + side, A2 = A - side;
		glm::vec3 B1 = B + side, B2 = B - side;

		add_triangle(A1, B1, B2, negro);
		add_triangle(A1, B2, A2, negro);
		};

	int colorIndex = 0;
	for (auto& cara : caras) {
		glm::vec3 A, B, C;
		std::tie(A, B, C) = cara;
		glm::vec3 color = colores[colorIndex++ % colores.size()];

		// Subdividir triángulo A-B-C en rejillas (lineas que dividen)
		for (int i = 0; i < subdiv; ++i) {
			for (int j = 0; j <= i; ++j) {
				// Baricéntricas para los puntos
				float u0 = float(subdiv - i) / subdiv;
				float v0 = float(j) / subdiv;
				float w0 = 1.0f - u0 - v0;

				float u1 = float(subdiv - i - 1) / subdiv;
				float v1 = float(j) / subdiv;
				float w1 = 1.0f - u1 - v1;

				float u2 = float(subdiv - i - 1) / subdiv;
				float v2 = float(j + 1) / subdiv;
				float w2 = 1.0f - u2 - v2;

				float u3 = float(subdiv - i) / subdiv;
				float v3 = float(j + 1) / subdiv;
				float w3 = 1.0f - u3 - v3;

				glm::vec3 p0 = A * u0 + B * v0 + C * w0;
				glm::vec3 p1 = A * u1 + B * v1 + C * w1;
				glm::vec3 p2 = A * u2 + B * v2 + C * w2;
				glm::vec3 p3 = A * u3 + B * v3 + C * w3;

				// Triángulo inferior
				add_triangle(p0, p1, p2, color);
				add_borde(p0, p1);
				add_borde(p1, p2);
				add_borde(p2, p0);

				// Triángulo superior (si aplica)
				if (j != i) {
					add_triangle(p0, p2, p3, color);
					add_borde(p0, p2);
					add_borde(p2, p3);
					add_borde(p3, p0);
				}
			}
		}
	}

	// Crear la malla final
	meshColor = new MeshColor();
	meshColor->CreateMeshColor(vertices.data(), vertices.size());
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

int main()
{
	//Control de camara para la figura
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;
	float rotationSpeed = 2.0f; // grados por segundo
	
	// Crear ventana
	mainWindow = Window(2160, 1440); //le cambie las dimensiones a la ventana, para mi monitor original (800x600 se veia muy chico)
	mainWindow.Initialise();

	CreateShaders();
	CrearPiramideColores();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),-60.0f, 0.0f,
		0.05f, 0.3f); //cambiar estos para la velocidad de movimiento y la sens xd


	// Uniforms y proyección
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),0.1f, 100.0f);

	// Variables comunes
	glm::mat4 model(1.0f);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;

	while (!mainWindow.getShouldClose())
	{
		// Tiempo
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Eventos
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);

		// Para mover la figura con las teclas E, R, T
		bool* keys = mainWindow.getsKeys();

		if (keys[GLFW_KEY_E]) {
			rotX += rotationSpeed * deltaTime;
		}
		if (keys[GLFW_KEY_R]) {
			rotY += rotationSpeed * deltaTime;
		}
		if (keys[GLFW_KEY_T]) {
			rotZ += rotationSpeed * deltaTime;
		}
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		// Limpieza de buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Usar shader (color)
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

	//================ PIRAMIDE RUBIK =) =========================
		model = glm::mat4(5.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		//============== Mov er figura con teclas==============
		model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		//==========================================================
		model = glm::scale(model, glm::vec3(1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColor->RenderMeshColor();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

