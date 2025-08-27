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
	//GLfloat vertices_letras[] = {
	//	//X			Y			Z			R		G		B
	//	-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	//	1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	//	1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	//	/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	//	-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	//	-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/

	//};
	//MeshColor* letras = new MeshColor();
	//letras->CreateMeshColor(vertices_letras, 18);
	//meshColorList.push_back(letras);


	//===============Triangulo verde -> 0 --
	GLfloat vertices_trianguloVerde[] = {
		//X			Y			Z			R		G		B
		0.0f,	0.2f,		0.2f,			0.0f,	0.5f,	0.0f,
		-0.15f, -0.1f,		0.2f,			0.0f,	0.5f,	0.0f,
		0.15f,	-0.1f,		0.2f,			0.0f,	0.5f,	0.0f,

	};
	MeshColor* trianguloVerde = new MeshColor();
	trianguloVerde->CreateMeshColor(vertices_trianguloVerde, 18);
	meshColorList.push_back(trianguloVerde);


	//===============Triangulo azul -> 1
	GLfloat vertices_trianguloAzul[] = {
		//X			Y			Z			R		G		B
		-0.7f,  0.0f,		0.2f,   	0.0f,	0.0f,	1.0f,
		 0.7f,  0.0f,		0.2f,   	0.0f,	0.0f,	1.0f,
		 0.0f,  0.6f,		0.2f,   	0.0f,	0.0f,	1.0f,

	};
	MeshColor* trianguloAzul = new MeshColor();
	trianguloAzul->CreateMeshColor(vertices_trianguloAzul, 18);
	meshColorList.push_back(trianguloAzul);

	//===============Cuadrado verde -> 2 --
	GLfloat vertices_cuadradoVerde[] = {
		//X			Y			Z			R		G		B
		0.2f,	0.2f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.2f,	0.2f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.2f,	-0.2f,		0.5f,			0.0f,	1.0f,	0.0f,

		-0.2f,  0.2f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.2f, -0.2f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.2f,	-0.2f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* cuadradoVerde = new MeshColor();
	cuadradoVerde->CreateMeshColor(vertices_cuadradoVerde, 36);
	meshColorList.push_back(cuadradoVerde);

	//===============Cuadrado rojo -> 3
	GLfloat vertices_cuadradoRojo[] = {
		//X			Y			Z			R		G		B
		-0.6f,	 0.7f,       0.4f,			1.0f,	0.0f,	0.0f,
		-0.6f,	-0.7f,		 0.4f,			1.0f,	0.0f,	0.0f,
		0.6f,   0.7f,		 0.4f,			1.0f,	0.0f,	0.0f,

		0.6f,	0.7f,		0.4f,			1.0f,	0.0f,	0.0f,
		-0.6f,	-0.7f,		0.4f,			1.0f,	0.0f,	0.0f,
		0.6f,	-0.7f,		0.4f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* cuadradoRojo = new MeshColor();
	cuadradoRojo->CreateMeshColor(vertices_cuadradoRojo, 36);
	meshColorList.push_back(cuadradoRojo);

	//===============Cuadrado café -> 4 --
	GLfloat vertices_cuadradoCafe[] = {
		//X			Y			Z			R		G		B
		-0.1f,  -0.1f,		0.1f,			0.478f, 0.255f, 0.067f,
		-0.1f,  0.1f,		0.1f,			0.478f, 0.255f, 0.067f,
		0.1f,   0.1f,		0.1f,			0.478f, 0.255f, 0.067f,

		0.1f,	-0.1f,		0.1f,			0.478f, 0.255f, 0.067f,
		0.1f,	0.1f,		0.1f,			0.478f, 0.255f, 0.067f,
		-0.1f,	-0.1f,		0.1f,			0.478f, 0.255f, 0.067f,


	};
	MeshColor* cuadradoCafe = new MeshColor();
	cuadradoCafe->CreateMeshColor(vertices_cuadradoCafe, 36);
	meshColorList.push_back(cuadradoCafe);



}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600); //crear ventana
	mainWindow.Initialise();

	CreaPiramide(); //índice 0 en MeshList. el indice cambia para llamar, si estuviera abajo seria 1 y crearcubo seria 0
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
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


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader(); //Vas a llamar el shader 1 que seria shader y el otro es 0
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas


		//Estas lineas se usan para pintar las figuras
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); //el -4 es hacia dentro del monitor
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[3]->RenderMeshColor(); //usar el numero de lista correspondiente



		/*Lista de objetos Meshcolor
		Triangulo verde		-> 0
		Triangulo azul		-> 1
		Cuadrado verde		-> 2
		Cuadrado rojo		-> 3
		Cuadrado cafe		-> 4
		*/
		/*==================VERDE FUERTE (ARBOL)================== */
		/*Arbol verde izq (triángulo verde 0)*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.80f, -0.7f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		/*Arbol verde der (triángulo verde 0)*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.80f, -0.70f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		/* ==================AZUL================== */

		/*Tejado azul (triángulo azul 1)*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.40f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		/* ==================VERDE CLARO================== */
		/*Puerta verde (cuadrado verde 2)*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.80f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		/*Ventana verde izq */
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		/*Ventana verde der */
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, 0, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		/* ==================ROJO================== */

		/*Ladrillos rojos (Cuadrado rojo 3)*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.30f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		/* ==================CAFE================== */

		/*Maceta para arbol 1*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.80f, -0.90f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		/*Maceta para arbol 2*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.80f, -0.90f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();







		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList

		/*shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;*/

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f)); //para moverla de lugar, esta es mas cerca 
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[0]->RenderMesh(); // objeto de la lista mesh, que en este caso es cubo y si paso a 0, voy a ver la piramide



		//para intanciar otra figura

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); //para moverla de lugar, esta es mas cerca que el de meshcolor
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh(); // objeto de la lista mesh, que en este caso es cubo y si paso a 0, voy a ver la piramide



		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); //el verctor es para que gira en el vector que desees x,y,z
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));//el verctor es para que gira en el vector que desees x,y,z
*/
