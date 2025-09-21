/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
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
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(2160, 1440);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(10, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelLlanta(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 cilindro(1.0f);

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	glm::mat4 modelCuerpo(1.0f);   // matriz base del cuerpo
	glm::mat4 modelCola(1.0f);     // base para cola articulada

	// Para las patas
	glm::mat4 musloDD(1.0f);  // muslo pata delantera derecha
	glm::mat4 piernaDD(1.0f); // pierna pata delantera derecha

	glm::mat4 musloDI(1.0f);  // muslo pata delantera izquierda
	glm::mat4 piernaDI(1.0f); // pierna pata delantera izquierda

	glm::mat4 musloTD(1.0f);  // muslo pata trasera derecha
	glm::mat4 piernaTD(1.0f); // pierna pata trasera derecha

	glm::mat4 musloTI(1.0f);  // muslo pata trasera izquierda
	glm::mat4 piernaTI(1.0f); // pierna pata trasera izquierda}


	glm::vec3 colorPatas = glm::vec3(1.0f, 0.5f, 0.0f);       // naranja claro para piernas
	glm::vec3 colorCuerpoPatas = glm::vec3(0.0f, 0.6f, 0.4f); // naranja fuerte para articulaciones



	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));




		// =================== ORNITORRINCO =====================
		model = glm::mat4(1.0f);
		// ---------- CUERPO ----------
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		modelCuerpo = model;
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.6f, 0.4f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); 

		// ---------- PICO ----------
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(2.5f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.5f, 0.0f); // naranja
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// ---------- ARTICULACIÓN DE LA COLA (CILINDRO) ----------
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(-1.40f, -0.2f, 0.0f)); 
		model = glm::rotate(model, glm::radians(mainWindow.getRotLlanta3()), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 articulacionCola = model; 

		// Cilindro pequeño que se mueve 
		glm::mat4 cilindroCola = model;
		cilindroCola = glm::rotate(cilindroCola, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		cilindroCola = glm::scale(cilindroCola, glm::vec3(0.1f, 0.3f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindroCola));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[2]->RenderMeshGeometry();

		// ---------- COLA ----------
		model = articulacionCola;
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.6f, 0.4f, 1.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); 



		// ---------- OJOS ----------
		// ---------- OJO DERECHO  ----------
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(1.5f, 0.5f, 0.8f));
		glm::mat4 ojoDerechoBase = model; 
		model = glm::scale(model, glm::vec3(0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// ---------- IRIS  ----------
		model = ojoDerechoBase;
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.2f)); // un poco al frente
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2f, 0.0f, 0.1f); // marrón oscuro o negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();


		// ---------- OJO IZQUIERDO  ----------
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(1.5f, 0.5f, -0.8f));
		glm::mat4 ojoIzquierdoBase = model;
		model = glm::scale(model, glm::vec3(0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// ---------- IRIS  ----------
		model = ojoIzquierdoBase;
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, -0.2f)); // simétrico al otro
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2f, 0.0f, 0.1f); // mismo color del iris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();



		// ================= SOMBRERO ESTILO PERRY =================

		glm::vec3 colorSombrero = glm::vec3(0.2f, 0.0f, 0.2f); // púrpura oscuro

		// --------- Ala del sombrero (cilindro achatado y grande) ---------
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(1.2f, 1.1f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.6f, 0.1f, 1.6f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSombrero));
		meshList[2]->RenderMeshGeometry(); // cilindro como ala

		// --------- Copa del sombrero (cubo bajo y más ancho) ---------
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(1.2f, 1.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSombrero));
		meshList[0]->RenderMesh(); 

		

		// =================== PATAS CON ARTICULACIONES =====================

		// --------- Pata delantera derecha ---------
		model = modelCuerpo;
		// Articulación (cilindro hombro)
		model = glm::translate(model, glm::vec3(1.2f, -1.0f, 0.8f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[2]->RenderMeshGeometry();

		// Muslo
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		musloDD = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[0]->RenderMesh();

		// Articulación (cilindro rodilla)
		model = musloDD;
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, 0.0f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.4f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[2]->RenderMeshGeometry();

		// Pata
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		piernaDD = model;
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[0]->RenderMesh();


		// --------- Pata delantera izquierda ---------
		model = modelCuerpo;
		// Articulación (cilindro hombro)
		model = glm::translate(model, glm::vec3(-1.2f, -1.0f, 0.8f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[2]->RenderMeshGeometry();

		// Muslo
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		musloDI = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[0]->RenderMesh();

		// Articulación (cilindro rodilla)
		model = musloDI;
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, 0.0f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[2]->RenderMeshGeometry();

		// Pata
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[0]->RenderMesh();


		// --------- Pata trasera derecha ---------
		model = modelCuerpo;
		// Articulación (cilindro hombro)
		model = glm::translate(model, glm::vec3(1.2f, -1.0f, -0.8f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[2]->RenderMeshGeometry();

		// Muslo
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		musloTD = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[0]->RenderMesh();

		// Articulación (cilindro rodilla)
		model = musloTD;
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, 0.0f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[2]->RenderMeshGeometry();

		// Pata
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[0]->RenderMesh();


		// --------- Pata trasera izquierda ---------
		model = modelCuerpo;
		// Articulación (cilindro hombro)
		model = glm::translate(model, glm::vec3(-1.2f, -1.0f, -0.8f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[2]->RenderMeshGeometry();

		// Muslo
		model = glm::rotate(model, glm::radians(mainWindow.getRotLlanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		musloTI = model;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoPatas));
		meshList[0]->RenderMesh();

		// Articulación (cilindro rodilla)
		model = musloTI;
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, 0.0f));
		cilindro = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		cilindro = glm::scale(cilindro, glm::vec3(0.2f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cilindro));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[2]->RenderMeshGeometry();

		// Pata
		model = glm::rotate(model, glm::radians(mainWindow.getRotLlanta2()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		meshList[0]->RenderMesh();







		////CREANDO LA CABINA
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 5.0f, -4.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(5.0f, 3.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[0]->RenderMesh();

		//// == BASE ==
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f)); // debajo de la cabina
		//model = glm::scale(model, glm::vec3(5.5f, 2.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.3f, 0.1f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[4]->RenderMeshGeometry(); // Pirámide cuadrangular

		//// LLANTA 1 (delantera derecha)
		//modelLlanta = modelaux;
		//modelLlanta = glm::translate(modelLlanta, glm::vec3(2.0f, -2.3f, 1.2f)); // esquina base
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(mainWindow.getRotLlanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelLlanta = glm::scale(modelLlanta, glm::vec3(0.7f, 0.7f, 0.7f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		//color = glm::vec3(0.2f, 0.2f, 0.2f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[2]->RenderMeshGeometry(); // Cilindro

		//// LLANTA 2 (delantera izquierda)
		//modelLlanta = modelaux;
		//modelLlanta = glm::translate(modelLlanta, glm::vec3(-2.0f, -2.3f, 1.2f));
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(mainWindow.getRotLlanta2()), glm::vec3(0.0f, 0.0f, 1.0f));

		//modelLlanta = glm::rotate(modelLlanta, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelLlanta = glm::scale(modelLlanta, glm::vec3(0.7f, 0.7f, 0.7f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		//meshList[2]->RenderMeshGeometry();

		//// LLANTA 3 (trasera derecha)
		//modelLlanta = modelaux;
		//modelLlanta = glm::translate(modelLlanta, glm::vec3(2.0f, -2.3f, -1.2f));
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(mainWindow.getRotLlanta3()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelLlanta = glm::scale(modelLlanta, glm::vec3(0.7f, 0.7f, 0.7f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		//meshList[2]->RenderMeshGeometry();

		//// LLANTA 4 (trasera izquierda)
		//modelLlanta = modelaux;
		//modelLlanta = glm::translate(modelLlanta, glm::vec3(-2.0f, -2.3f, -1.2f));
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(mainWindow.getRotLlanta4()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelLlanta = glm::rotate(modelLlanta, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelLlanta = glm::scale(modelLlanta, glm::vec3(0.7f, 0.7f, 0.7f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		//meshList[2]->RenderMeshGeometry();

		////ARTICULACION1 CABINA-BRAZO
		//model = modelaux;
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//sp.render();

		//// Creando el brazo de una grúa
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();

		////SEGUNDA ARTICULACION 
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//sp.render(); //dibuja esfera

		////============================== BRAZO MEDIO con articulacion ================================*/

		////segundo brazo

		////model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		////TERCER ARTICULACION
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render(); //dibuja esfera

		////============================== BRAZO ALTO con articulacion ================================*/


		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.50f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();

		////SEGUNDA ARTICULACION 
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render(); //dibuja esfera


		////================================= CANASTA =================================

		//model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh();



		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}

