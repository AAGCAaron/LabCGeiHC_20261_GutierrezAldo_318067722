
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h" //nuestro sol
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaedroTexture;

Model CarroBase;
Model CarroCofre;
Model Llanta_M_Der;
Model Llanta_M_IZQ;
Model Blackhawk_M;
Model antorcha_M;
Model rayGun;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


//void CrearOctaedro()
//{
//	unsigned int octaedro_indices[] = {
//		// 8 triángulos
//		0, 1, 2,
//		3, 4, 5,
//		6, 7, 8,
//		9, 10,11,
//		12,13,14,
//		15,16,17,
//		18,19,20,
//		21,22,23
//	};
//
//	GLfloat octaedro_vertices[] = {
//		// Cara 4
//		//x      y      z	 S	   T			NX		NY		NZ
//		-0.5f, 0.0f,  0.5f,   0.02f, .750f,    0.0f,   0.0f, -1.0f,
//		 0.5f, 0.0f,  0.5f,   0.49f, 0.75f,    0.0f,   0.0f, -1.0f,
//		 0.0f, 0.75f, 0.0f,   0.26f, 1.00f,    0.0f,   0.0f, -1.0f,
//
//		// Cara 8
//		//x      y      z	 S	   T			NX		NY		NZ
//		 0.5f, 0.0f,  0.5f,  0.28f, 0.51f,     -1.0f, 0.0f, 0.0f,
//		 0.5f, 0.0f, -0.5f,  0.73f, 0.51f,     -1.0f, 0.0f, 0.0f,
//		 0.0f, 0.75f, 0.0f,  0.50f, 0.75f,     -1.0f, 0.0f, 0.0f,
//
//		// Cara 2
//		//x      y      z	 S	   T			NX		NY		NZ
//		 0.5f, 0.0f, -0.5f,  0.5f, 0.25f,      0.0f, 0.0f, 1.0f,
//		-0.5f, 0.0f, -0.5f,  0.98f, 0.25f,     0.0f, 0.0f, 1.0f,
//		 0.0f, 0.75f, 0.0f,  0.73f, 0.48f,     0.0f, 0.0f, 1.0f,
//
//		// Cara 6
//		//x      y      z	 S	   T			NX		NY		NZ
//		-0.5f, 0.0f, -0.5f,  0.07f, 0.24f,     1.0f, 0.0f, 0.0f,
//		-0.5f, 0.0f,  0.5f,  0.43f, 0.24f,     1.0f, 0.0f, 0.0f,
//		 0.0f, 0.75f, 0.0f,  0.24f, 0.47f,     1.0f, 0.0f, 0.0f,
//
//		// Cara 5 
//		//x      y      z	 S	   T			NX		NY		NZ
//		 0.5f, 0.0f,  0.5f,  1.0f, 0.740f,     0.0f, 0.0f, -1.0f,
//		-0.5f, 0.0f,  0.5f,  0.50f, 0.740f,    0.0f, 0.0f, -1.0f,
//		 0.0f, -0.75f, 0.0f, 0.74f, 0.49f,     0.0f, 0.0f, -1.0f,
//
//		// Cara 1
//		//x      y      z	 S	   T			NX		NY		NZ
//		 0.5f, 0.0f, -0.5f,  0.49f, 0.74f,     -1.0f, 0.0f, 0.0f,
//		 0.5f, 0.0f,  0.5f,  0.01f, 0.74f,     -1.0f, 0.0f, 0.0f,
//		 0.0f, -0.75f, 0.0f, 0.26f, 0.49f,     -1.0f, 0.0f, 0.0f,
//
//		// Cara 7
//		//x      y      z	 S	   T			NX		NY		NZ
//		-0.5f, 0.0f, -0.5f,  0.50f, 0.24f,     0.0f, 0.0f, 1.0f,
//		 0.5f, 0.0f, -0.5f,  0.0f, 0.24f,      0.0f, 0.0f, 1.0f,
//		 0.0f, -0.75f, 0.0f, 0.25f, 0.0f,      0.0f, 0.0f, 1.0f,
//
//		// Cara 3
//		//x      y      z	 S	   T			NX		NY		NZ
//		-0.5f, 0.0f,  0.5f,  0.75f, 0.49f,     1.0f, 0.0f, 0.0f,
//		-0.5f, 0.0f, -0.5f,  0.25f, 0.49f,     1.0f, 0.0f, 0.0f,
//		 0.0f, -0.75f, 0.0f, 0.50f, 0.24f,     1.0f, 0.0f, 0.0f,
//	};
//
//	Mesh* octaedro = new Mesh();
//	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 8*3*8 , 8 * 3);
//	meshList.push_back(octaedro);
//	calcAverageNormals(octaedro_indices, 24, octaedro_vertices, 192, 8, 5);
//}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	//CrearOctaedro();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	//octaedroTexture = Texture("Textures/octa.tga");
	//octaedroTexture.LoadTextureA();


	/*Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");*/
	
	//Carro PRACTICA 8
	CarroBase = Model();
	CarroBase.LoadModel("Models/CarroBase.fbx");
	CarroCofre = Model();
	CarroCofre.LoadModel("Models/CarroCofre.fbx");
	Llanta_M_Der = Model();
	Llanta_M_Der.LoadModel("Models/LlantaLucasDer.fbx");
	Llanta_M_IZQ = Model();
	Llanta_M_IZQ.LoadModel("Models/LlantaLucasIzq.fbx");

	//luces practica 8
	antorcha_M = Model();
	antorcha_M.LoadModel("Models/Antorcha.fbx");
	rayGun = Model();
	rayGun.LoadModel("Models/RayGun.fbx");

	
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	float luzOscilacion = 0.0f;


	//Aqui es para cambiar la luz de las cosas
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,   // color blanco
		0.2f, 0.3f,         // ambient y diffuse
		-1.0f, -1.0f, -1.0f // dirección
	);

	unsigned int pointLightCount = 0;
	//Luz puntual antorcha
	pointLights[0] = PointLight(
		1.0f, 1.0f, 1.0f, // COLOR
		1.5f, 1.2f,       // Intensidad ambiental y difusa 
		0.0f, 0.0f, 0.0f, // Posición inicial
		1.0f, 0.15f, 0.05f  // ATENUACION
	);
	pointLightCount++;

	//Luz puntual Rayo laser
	pointLights[1] = PointLight(
		1.0f, 1.0f, 0.0f, // COLOR
		1.5f, 1.2f,       // Intensidad ambiental y difusa 
		0.0f, 0.0f, 0.0f, // Posición inicial
		0.50f, 0.15f, 0.05f  // ATENUACION
	);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna de la camara
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f,
	//	0.0f, 0.0f, 0.0f, //posicion
	//	0.0f, -1.0f, 0.0f, //direccion
	//	1.0f, 0.0f, 0.0f, //atenuación
	//	5.0f);
	//spotLightCount++;

	//cofre carro
	spotLights[0] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 0.30f,
		0.0f, 0.0f, 0.0f, //posicion
		1.0f, 0.00f, 0.0f, //direccion
		0.40f, 0.1f, 0.0f, //atenuación
		20.0f); //Circunferencia
	spotLightCount++;

	//delantera coche
	spotLights[1] = SpotLight(0.9f, 0.9f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		0.40f, 0.1f, 0.0f,
		25.0f);
	spotLightCount++;

	//trasera coche
	spotLights[2] = SpotLight(0.9f, 0.90f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.40f, 0.1f, 0.0f,
		25.0f);
	spotLightCount++;




	





	
	//se crean mas luces puntuales y spotlight 
	float velocidad = 0.1f;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, posblackhawk=0;
	GLuint uniformColor = 0;

	//Matrices 
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition;
	glm::vec3 lightDirection;
	glm::vec3 lowerLight;
	glm::vec3 modelauxfuegoPos;
	glm::vec3 modelauxRayoLaser;
	glm::mat4 cofre;



	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);



		/* ========================   LUCES  ================================ */

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//Practica 8 luces del coche según movimiento
		SpotLight active[3];
		int n = 0;
		if (mainWindow.getLuzCofre())         active[n++] = spotLights[0]; // cofre
		if (mainWindow.getLuzAdelanteActiva()) active[n++] = spotLights[1]; // delanteras
		if (mainWindow.getLuzAtrasActiva())    active[n++] = spotLights[2]; // traseras
		shaderList[0].SetSpotLights(active, n);

		// PointLights independientes: Antorcha (0) y RayGun (1)
		PointLight pactive[2];
		unsigned int pc = 0;
		if (mainWindow.getAntorchaEncendida()) pactive[pc++] = pointLights[0];
		if (mainWindow.getRayGunLuz())        pactive[pc++] = pointLights[1];
		shaderList[0].SetPointLights(pactive, pc);

		/* ================== COCHE  =========================*/

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f + mainWindow.getmuevex(), 2.24f, -6.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		spotLights[1].SetPos(glm::vec3(model[3].x + 17.0f, model[3].y + 1.5f, model[3].z));
		spotLights[2].SetPos(glm::vec3(model[3].x - 23.0f, model[3].y + 1.5f, model[3].z));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroBase.RenderModel();

		// === Cofre del coche (jerarquía) ===
		cofre = modelaux;                                 
		cofre = glm::rotate(cofre, -90.0f * toRadians, glm::vec3(1, 0, 0));
		cofre = glm::rotate(cofre, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1, 0, 0));
		spotLights[0].SetFlash(glm::vec3(cofre[3].x + 5.0f, cofre[3].y + 1.12f, cofre[3].z - 0.05f),     
		glm::normalize(glm::vec3(cofre * glm::vec4(0.0f, -0.30f, -1.0f, 0.0f))));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cofre));
		CarroCofre.RenderModel();


		// ====================== LLANTAS ============================
		color = glm::vec3(1.0f, 1.0f, 1.0f); // blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Llanta delantera derecha
		model = modelaux; // jerarquía desde el coche
		model = glm::translate(model, glm::vec3(0.70f, -0.45f, -0.52f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M_Der.RenderModel();

		// Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.70f, -0.45f, -0.52f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M_IZQ.RenderModel();

		// Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.64f, -0.45f, 1.83f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M_Der.RenderModel();

		// Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.64f, -0.45f, 1.83f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M_IZQ.RenderModel();


		/* ========================   MODELOS ================================ */
		//======================= ANTORCHA CON LUZ LIGADA =======================

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.2f, 0.0f)); // posición base
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		// Calcular posición del fuego (luz)
		modelauxfuegoPos = glm::vec3(model * glm::vec4(0.0f, 5.5f, 0.0f, 1.0f));
		pointLights[0].SetPos(modelauxfuegoPos);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		antorcha_M.RenderModel();



		//======================= RAYGUN =======================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		// posición de la point light de la RayGun
		pointLights[1].SetPos(glm::vec3(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rayGun.RenderModel();


		
		/*Modelo piso*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

////agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
//model = glm::mat4(1.0);
//model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
//gluniformmatrix4fv(uniformmodel, 1, gl_false, glm::value_ptr(model));
//
////blending: transparencia o traslucidez
//glenable(gl_blend);
//glblendfunc(gl_src_alpha, gl_one_minus_src_alpha);
//agavetexture.usetexture();
//material_opaco.usematerial(uniformspecularintensity, uniformshininess);
//meshlist[3]->rendermesh();
//gldisable(gl_blend);


//helicoptero
/*model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 5.0f, 6.0));
model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Blackhawk_M.RenderModel();*/





