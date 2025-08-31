// Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList; //puedes usar cualquier valor dentro de ellos al ser una LISTA
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderAzul = "shaders/shaderAzul.vert";
static const char* vShaderCafe = "shaders/shaderCafe.vert";
static const char* vShaderRojo = "shaders/shaderRojo.vert";
static const char* vShaderVerde = "shaders/shaderVerde.vert";
static const char* vShaderVerdeOscuro = "shaders/shaderVerdeOscuro.vert";



float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36); //24 vertices, entre 3 = 8
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	//aqui para dibujar las letras
	GLfloat vertices_letras[] = {
		
		// ====== Letra A ======== rojo
		// Triangulo grande izquierdo
		//X	   Y	 Z			R	 G     B
		-0.6f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.9f, 0.2f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.6f, 0.2f, 0.0f,    1.0f, 0.0f, 0.0f,

		// Triangulo grande derecha
		-0.4f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.4f, 0.2f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.1f, 0.2f, 0.0f,    1.0f, 0.0f, 0.0f,

		// Triangulo medio superior derecha 
		-0.4f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.5f, 0.8f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.4f, 0.6f, 0.0f,    1.0f, 0.0f, 0.0f,

		// Triangulo medio superior izq
		-0.6f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.6f, 0.6f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.4f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,

		// Triángulo medio inferior izq
		-0.6f, 0.6f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.6f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.4f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,

		// Triángulo medio inferior der
		-0.4f, 0.6f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.6f, 0.6f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.4f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,

		// ====== Letra G ======== (verde)

		// Triangulo arriba der
		0.8f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.4f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.8f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,

		// arriba izq
		0.4f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.2f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.8f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,

		// medio izq
		0.4f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.2f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.2f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,

		// medio der
		0.4f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.2f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.4f, 0.2f, 0.0f,    0.0f, 1.0f, 0.0f,

		// abajo izq
		0.4f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.4f, 0.2f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.8f, 0.2f, 0.0f,    0.0f, 1.0f, 0.0f,

		// abajo der
		0.8f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.4f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.8f, 0.2f, 0.0f,    0.0f, 1.0f, 0.0f,

		// boca der
		0.8f, 0.6f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.7f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.8f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,

		// boca izq
		0.7f, 0.6f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.7f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.8f, 0.6f, 0.0f,    0.0f, 1.0f, 0.0f,

		// diente arriba
		0.5f, 0.6f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.7f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.7f, 0.6f, 0.0f,    0.0f, 1.0f, 0.0f,

		// diente abajo
		0.5f, 0.6f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.55f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		0.7f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,

		// curva medio abajo
		0.4f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.4f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.5f, 0.4f, 0.0f,    0.0f, 1.0f, 0.0f,

		// curva medio
		0.4f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.4f, 0.7f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.5f, 0.8f, 0.0f,    0.0f, 1.0f, 0.0f,

		// ====== Letra C ======== (cyan)

		// arriba der
		-0.2f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.2f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,

		// arriba izq
		-0.6f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.2f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,

		// medio izq
		-0.8f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,

		// medio der
		-0.6f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.8f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,

		// abajo izq
		-0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.2f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,

		// abajo der
		-0.2f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.2f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,

		// curva arriba
		-0.6f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,

		// curva abajo
		-0.6f, -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.6f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,


	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 78*6);
	meshColorList.push_back(letras);
	 
}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//-------------- SHADERS NUEVOS --------------
	//------ AZUL --> 2
	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shaderAzul);
	//------ CAFE --> 3
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shaderCafe);
	//------ ROJO --> 4
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shaderRojo);
	//------ VERDE --> 5
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shaderVerde);
	//------ VERDE OSCURO --> 6
	Shader* shaderVerdeOscuro = new Shader();
	shaderVerdeOscuro->CreateFromFiles(vShaderVerdeOscuro, fShader);
	shaderList.push_back(*shaderVerdeOscuro);
}


int main()
{
	mainWindow = Window(800, 600); //crear ventana
	mainWindow.Initialise();

	CreaPiramide(); //índice 0 en MeshList. el indice cambia para llamar, si estuviera abajo seria 1 y crearcubo seria 0
	CrearCubo();//índice 1 en MeshList
	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //arriba, abajo, izq, der, visualizacion de 0.1f , hasta 100.0f
	//para esta practica se queda todo igual

	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//la linea de arriba comentada es el POV, angulo de apertura que esta en radianes, y el aspecratio es w/h, el otro valor es near y far (cerca o lejos que veo (unidades en z))
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.


	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad, para no crear muchas matrices en ejecucion
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad, por que ya vamos a ver el Z

		/*============= Para pintar las letras, descomentar todo este bloque =============*/
		////Pintar letras
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();


		/*============================== Descomentar hasta aquí=================================*/
		



		//===================== CASA =========================
		
		//------Piramide(0) azul(2)--------
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.65f, -3.0f));
		model = glm::scale(model, glm::vec3(1.30f, 0.70f, -1.00f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//------Cubo(1) rojo(4) Ladrillos --------
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.35f, -3.1f));
		model = glm::scale(model, glm::vec3(1.0f, 1.30f, -0.10f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//------Cubo(1) verde(5) ventana izquierda--------
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.25f, -0.15f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, -0.10f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//------Cubo(1) verde(5) ventana derecha--------
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.25f, -0.15f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, -0.10f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//------Cubo(1) verde(5) puerta centro--------
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.80f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, -0.10f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//----------------------- ÁRBOLES -----------------------

		//------Cubo(1) café(3) Tronco izquierda--------
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.80f, -0.90f, -3.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, -0.23f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//------Cubo(1) café(3) Tronco derecha--------
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.80f, -0.90f, -3.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, -0.23f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//----- Piramide(0) verde oscuro(6) árbol izquierda--------

		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(-0.80f, -0.44f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.70f, 0.35f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//----- Piramide(0) verde oscuro(6) árbol derecha--------
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.80f, -0.44f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.70f, 0.35f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}