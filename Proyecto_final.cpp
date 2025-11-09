/*
Animacion:
Simple o basica:Por banderas y condicionales (mas de 1 transformacion geometrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
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

//para iluminacion
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animacion


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Skybox skybox;
Skybox skyboxNight;
//-------
//MODELOS
//-------
Model Piso_M;
Model PisoCamino_M;

//---ESTRUCTURAS---
//-Tenochttitlan y Ring-
Model TemploMayor_M;
Model TemploTezcatlipoca_M;

Model Ehecatl_M;

Model Coacalco_M;
Model Cihuacoatl_M;
Model Xochiquetzal_M;
Model Chicomecoatl_M;

Model Calmecac_M;
Model Tlachtli_M;
Model Tonatiuht_M;

Model Ring_M;
Model Sillas_B_M;
Model Sillas_V_M;
Model Sillas_R_M;

//-Extra
Model Banca_M;

Model Bote_CSP_M;
Model Bote_M_M;
Model Bote_E_M;

//--Mundos--
Model CasaCSP_M;
Model CasaCSP_PyG_M;
Model CasaCSP_V_M;

Model CasaMarvin_M;

Model Cafe_ZZZ;


//---Para Animaciones Basicas---
Model BoteLuchaLibre_M;
Model Bote_LL_Tapa_M;

Model Lata_M;

Model Campana_M;
Model Campana_M_M;


//--- animaciones complejas ---

//Cinturon 
Model cinturon_M;
//taz
//Tornado


//---PERSONAJES---

//CAMINATA PATO LUCAS
Model patoLucasCuerpo_M;
Model patoLucas_PieIzq_M;
Model patoLucas_PieDer_M;
Model patoLucas_ManoIzq_M;
Model patoLucas_ManoDer_M;

// Modelos de animación por keyframes (nuevos)
Model Animacion1_M; // usa keyframe01.txt
Model Animacion2_M; // reservado para keyframe02.txt
Model Animacion3_M; // reservado para keyframe03.txt

//Zenless Zone Zero
Model belle;
Model Vivian_M;
Model Astra_M;
Model Eus_M;
Model Ellen;
//Looney Tunes
Model Coyote_M;
Model Marvin_M;
Model BugsBunny_M;
//Taz Demonio de Tasmania -- animacion compleja
Model taz_M;
Model taz_Tornado_M;

//Chicas Superopoderosas
Model Bombon_M;
Model Burbuja_M;
Model Bellota_M;
Model MojoJojo_M;
Model El_M;
//---------------------------------- Hasta Aqui--------------------------------


//Iluminacion Escenario Luces puntuales
Model antorcha_piso_M;
Model foco;
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





//calculo del promedio de las normales para sombreado de Phong
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
			-1.0f, -1.0f, -0.6f,    0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,     0.5f, 0.0f,        0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,    1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,      0.5f, 1.0f,        0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,    0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,    10.0f, 0.0f,       0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,    0.0f, 10.0f,       0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,     10.0f, 10.0f,      0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,      0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,       1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,        1.0f, 1.0f,        0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,       0.0f, 1.0f,        0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,      0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,       1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,        1.0f, 1.0f,        0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,       0.0f, 1.0f,        0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,       0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,        1.0f, 0.0f,        0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,       1.0f, 1.0f,        0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,      0.0f, 1.0f,        0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,       0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,        1.0f, 0.0f,        0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,       1.0f, 1.0f,        0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,      0.0f, 1.0f,        0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,       0.0f, 0.67f,       0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,        0.25f, 0.67f,      0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,       0.25f, 1.0f,       0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,      0.0f, 1.0f,        0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los numeros

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un numero

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	//Logica LUCES
	antorcha_piso_M = Model();
	antorcha_piso_M.LoadModel("Models/antorcha.obj");
	//antorcha_piso_T = Texture("Textures/TexturaAntorcha.tga");
	//antorcha_piso_T.LoadTextureA();

	foco = Model();
	foco.LoadModel("Models/foco.obj");

	//--------
	//MODELOS
	//--------
	Piso_M = Model();
	Piso_M.LoadModel("Models/piso.obj");

	PisoCamino_M = Model();
	PisoCamino_M.LoadModel("Models/piso_camino.obj");

	//---ESTRUCTURAS---

	//-Tenochtitlan-

	TemploMayor_M = Model();
	TemploMayor_M.LoadModel("Models/templo_mayor.obj");

	TemploTezcatlipoca_M = Model();
	TemploTezcatlipoca_M.LoadModel("Models/templo_tezcatlipoca.obj");

	Ehecatl_M = Model();
	Ehecatl_M.LoadModel("Models/ehecatl.obj");

	Coacalco_M = Model();
	Coacalco_M.LoadModel("Models/coalco.obj");

	Cihuacoatl_M = Model();
	Cihuacoatl_M.LoadModel("Models/cihuacoatl.obj");

	Chicomecoatl_M = Model();
	Chicomecoatl_M.LoadModel("Models/chicomecoatl.obj");

	Xochiquetzal_M = Model();
	Xochiquetzal_M.LoadModel("Models/xochiquetzal.obj");

	Calmecac_M = Model();
	Calmecac_M.LoadModel("Models/calmecac.obj");

	Tlachtli_M = Model();
	Tlachtli_M.LoadModel("Models/tlachtli.obj");

	Tonatiuht_M = Model();
	Tonatiuht_M.LoadModel("Models/tonatiuht.obj");

	//Ring

	Ring_M = Model();
	Ring_M.LoadModel("Models/ring.obj");

	Sillas_B_M = Model();
	Sillas_B_M.LoadModel("Models/sillas_blancas.obj");

	Sillas_V_M = Model();
	Sillas_V_M.LoadModel("Models/sillas_verdes.obj");

	Sillas_R_M = Model();
	Sillas_R_M.LoadModel("Models/sillas_rojas.obj");

	//-Extra

	Banca_M = Model();
	Banca_M.LoadModel("Models/banca.obj");

	Bote_CSP_M = Model();
	Bote_CSP_M.LoadModel("Models/bote_csp.obj");

	Bote_M_M = Model();
	Bote_M_M.LoadModel("Models/bote_marvin.obj");

	Bote_E_M = Model();
	Bote_E_M.LoadModel("Models/bote_elle.obj");

	//-Mundos-
	//Chicas SuperPoderosas
	CasaCSP_M = Model();
	CasaCSP_M.LoadModel("Models/casa_csp.obj");

	CasaCSP_PyG_M = Model();
	CasaCSP_PyG_M.LoadModel("Models/casa_csp_puerta_garaje.obj");

	CasaCSP_V_M = Model();
	CasaCSP_V_M.LoadModel("Models/casa_csp_ventanas.obj");

	//Casa Marvin
	CasaMarvin_M = Model();
	CasaMarvin_M.LoadModel("Models/CasaMarvin.obj");

	//Cofe ZZZ

	Cafe_ZZZ = Model();
	Cafe_ZZZ.LoadModel("Models/cafe.obj");

	//---Para Animaciones Basicas---

	BoteLuchaLibre_M = Model();
	BoteLuchaLibre_M.LoadModel("Models/bote_lucha_libre.obj");

	Bote_LL_Tapa_M = Model();
	Bote_LL_Tapa_M.LoadModel("Models/bote_ll_tapa.obj");

	Lata_M = Model();
	Lata_M.LoadModel("Models/lata.obj");


	Campana_M = Model();
	Campana_M.LoadModel("Models/campanal.obj");

	Campana_M_M = Model();
	Campana_M_M.LoadModel("Models/campana_martillo.obj");



	//---PERSONAJES---

	//Chicas SuperPoderosas

	Bombon_M = Model();
	Bombon_M.LoadModel("Models/bombon.obj");

	MojoJojo_M = Model();
	MojoJojo_M.LoadModel("Models/mojojojo.obj");

	Burbuja_M = Model();
	Burbuja_M.LoadModel("Models/burbuja.obj");

	Bellota_M = Model();
	Bellota_M.LoadModel("Models/bellota.obj");

	El_M = Model();
	El_M.LoadModel("Models/el.obj");

	Ellen = Model();
	Ellen.LoadModel("Models/ellen.obj");
	//Looney Tunes
	BugsBunny_M = Model();
	BugsBunny_M.LoadModel("Models/BugsBunny.obj");

	Coyote_M = Model();
	Coyote_M.LoadModel("Models/coyote.obj");

	Marvin_M = Model();
	Marvin_M.LoadModel("Models/Marvin.obj");

	//--------------- CAMINATA PATO LUCAS ------------------ 
	patoLucasCuerpo_M = Model();
	patoLucasCuerpo_M.LoadModel("Models/CuerpoLucas.obj");
	patoLucas_PieIzq_M = Model();
	patoLucas_PieIzq_M.LoadModel("Models/PieIzq.obj");
	patoLucas_PieDer_M = Model();
	patoLucas_PieDer_M.LoadModel("Models/PieDer.obj");
	patoLucas_ManoIzq_M = Model();
	patoLucas_ManoIzq_M.LoadModel("Models/brazoIzq.obj");
	patoLucas_ManoDer_M = Model();
	patoLucas_ManoDer_M.LoadModel("Models/brazoDer.obj");

	//Taz Demonio de Tasmania
	taz_M = Model();
	taz_M.LoadModel("Models/Taz.obj");
	taz_Tornado_M = Model();
	taz_Tornado_M.LoadModel("Models/Taz_tornado.obj");

	//ZZZ
	belle = Model();
	belle.LoadModel("Models/belle.obj");

	Vivian_M = Model();
	Vivian_M.LoadModel("Models/vivian.obj");

	Astra_M = Model();
	Astra_M.LoadModel("Models/astrayao.obj");

	Eus_M = Model();
	Eus_M.LoadModel("Models/eus.obj");

	//Cinturon
	cinturon_M = Model();
	cinturon_M.LoadModel("Models/CinturonLuchas.obj");

	// Cargar modelos de animacion (puedes cambiar las rutas si tienes los .obj)
	Animacion1_M = Model();
	Animacion1_M.LoadModel("Models/corazon.obj");
	Animacion2_M = Model();
	Animacion2_M.LoadModel("Models/carrot.obj");
	Animacion3_M = Model();
	Animacion3_M.LoadModel("Models/mascara.obj");

	// Cargar keyframes desde archivo para la animacion 1. Archivo por defecto: keyframe01.txt
	// Si deseas usar otra animacion aquí cambia el nombre del archivo o llama a LoadKeyframesFromFile
	// Removed automatic loading at startup to allow runtime selection via U/I/V
	// mainWindow.LoadKeyframesFromFile("keyframe01.txt");
	// mainWindow.LoadKeyframesFromFile("keyframe02.txt");
	// mainWindow.LoadKeyframesFromFile("keyframe03.txt");

	
	Model* keyframeParent = &BugsBunny_M; 
	glm::mat4 parentModelMatrix = glm::mat4(1.0f); 
	glm::mat4 parentModelMatrix2 = glm::mat4(1.0f);

//--------SKYBOX-------- -
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cube_right.png");
	skyboxFaces.push_back("Textures/Skybox/cube_left.png");
	skyboxFaces.push_back("Textures/Skybox/cube_down.png");
	skyboxFaces.push_back("Textures/Skybox/cube_up.png");
	skyboxFaces.push_back("Textures/Skybox/cube_back.png");
	skyboxFaces.push_back("Textures/Skybox/cube_front.png");

	skybox = Skybox(skyboxFaces);

	std::vector<std::string> skyboxFacesNight;
	skyboxFacesNight.push_back("Textures/Skybox/sky_night_rt.tga");
	skyboxFacesNight.push_back("Textures/Skybox/sky_night_lf.tga");
	skyboxFacesNight.push_back("Textures/Skybox/sky_night_dn.tga");
	skyboxFacesNight.push_back("Textures/Skybox/sky_night_up.tga");
	skyboxFacesNight.push_back("Textures/Skybox/sky_night_bk.tga");
	skyboxFacesNight.push_back("Textures/Skybox/sky_night_ft.tga");
	skyboxNight = Skybox(skyboxFacesNight);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, solo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.7f, 0.5f,
		0.0f, 0.0f, -1.0f);// Direccion de la luz

	//mainLight_night = DirectionalLight(0.2f, 0.2f, 0.5f,
	//	0.1f, 0.3f,
	//	0.0f, 0.0f, -1.0f);// Direccion de la luz


	//contador de luces puntuales
	unsigned int pointLightCount = 0;


	pointLights[0] = PointLight( //Cambiar solo el 0 por la que sea
		1.0f, 0.0f, 0.0f, // COLOR
		1.5f, 1.2f,       // Intensidad ambiental y difusa 
		0.0f, 0.0f, 0.0f, // Posici n inicial
		1.0f, 0.15f, 0.05f  // ATENUACION
	);
	pointLightCount++;

	pointLights[1] = PointLight( //Cambiar solo el 0 por la que sea
		1.0f, 0.0f, 0.0f, // COLOR
		1.5f, 1.2f,       // Intensidad ambiental y difusa 
		0.0f, 0.0f, 0.0f, // Posici n inicial
		1.0f, 0.15f, 0.05f  // ATENUACION
	);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//Escenario de rings
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		25.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		2.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	spotLights[2] = SpotLight(1.0f, 0.0f, 1.0f,
		2.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;




	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	//Matrices y vectores auxiliares para jerarquias y animaciones
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 cafelamp(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


	// Taz animation state (persistent across frames)
	static float tazOrbitAngle = 0.0f; // degrees
	static float tazSpinAngle = 0.0f; // degrees (self rotation)
	static float tornadoY = -1.5f; // current Y of tornado (starts at -1.5f as before)
	const float tazOrbitRadius = 10.5f; // radius of circular movement
	const float tazOrbitSpeed = 10.0f; // Tiempo de translado
	const float tazSpinSpeed = 1.0f; // degrees per second for self spin (fast)
	const float tornadoRiseSpeed = 1.5f; // units per second to approach Y=0

	// Configurar luz del ring desde Window (no romper jerarquia)
	mainWindow.SetRingLightCenter(glm::vec3(-42.0f, 10.0f, -17.0f));
	mainWindow.SetRingLightRadius(20.0f);
	// Desactivar orbita estableciendo offset local fijo
	mainWindow.SetRingLightLocalOffset(glm::vec3(0.0f, 5.0f, 0.0f)); // posicion local fija relativa al ring
	mainWindow.SetRingLightSpinSpeed(10.0f); // velocidad de giro propio (deg/s)
	mainWindow.SetRingLightTiltDeg(30.0f); // inclinacion de la luz hacia abajo (grados)

	// Configurar parametros del cinturon (altura, velocidad y flotacion)
	mainWindow.SetCinturonRiseSpeed(1.5f);
	mainWindow.SetCinturonTargetY(4.0f);
	mainWindow.SetCinturonFloatAmplitude(0.20f);
	mainWindow.SetCinturonFloatFreq(0.008f);

	// Configure 5 custom cameras (used by key 4 cycling). You can change these positions/targets here.
	mainWindow.SetCustomCameraPosition(0, glm::vec3(0.0f, 12.0f, -30.0f));
	mainWindow.SetCustomCameraTarget(0, glm::vec3(0.0f, 0.0f, -115.0f));
	mainWindow.SetCustomCameraPosition(1, glm::vec3(30.0f, 10.0f, -60.0f));
	mainWindow.SetCustomCameraTarget(1, glm::vec3(10.0f, 0.0f, -80.0f));
	mainWindow.SetCustomCameraPosition(2, glm::vec3(-30.0f, 15.0f, -40.0f));
	mainWindow.SetCustomCameraTarget(2, glm::vec3(-10.0f, 0.0f, -100.0f));
	mainWindow.SetCustomCameraPosition(3, glm::vec3(0.0f, 40.0f, 20.0f));
	mainWindow.SetCustomCameraTarget(3, glm::vec3(0.0f, 0.0f, -20.0f));
	mainWindow.SetCustomCameraPosition(4, glm::vec3(50.0f, 8.0f, -10.0f));
	mainWindow.SetCustomCameraTarget(4, glm::vec3(20.0f, 0.0f, -30.0f));

	// Reusable per-frame variables (avoid reallocations inside the loop)
	glm::mat4 viewMatrix;
	glm::vec3 camWorldPos(0.0f);
	glm::vec3 camDir(0.0f, 0.0f, -1.0f);
	glm::vec3 patoPos(0.0f);
	glm::vec3 dirVec(0.0f);
	glm::vec3 camPos(0.0f);
	glm::vec3 targetPos(0.0f);
	glm::vec3 ringPos(1.0);
	glm::vec3 ringDir(1.0);

	//Matrices auxiliares 
	glm::mat4 modelauxRing(1.0);
	glm::mat4 modelauxSillaB(1.0);
	glm::mat4 modelauxSillaR(1.0);
	glm::mat4 modelauxBote(1.0);
	glm::mat4 modelauxCampana(1.0);
	glm::vec3 modelauxfuegoPos(0.0f, 0.0f, 0.0f);
	glm::mat4 modelauxMojo(1.0);

	float patoRotY = 0.0f;
	float yawDeg = 0.0f;
	float pitchDeg = 0.0f;
	float yawRad = 0.0f;
	float pitchRad = 0.0f;
	float distanceBehind = 0.0f;
	float dx = 0.0f, dy = 0.0f;
	int camID = mainWindow.GetActiveCameraID();
	float tazRad = 0.0f, tazOx = 0.0f, tazOz = 0.0f;

	// Variables para ciclo dia y noche
	float dayNightPeriod = 40.0f; // Ajustar para velocidad de dia a noche (por ejemplo 80.0f para el doble de lento)
	const float TWO_PI = 6.28318530717958647692f;
	const float AMBIENT_DAY = 0.8f;   // valor en dia
	const float AMBIENT_NIGHT = 0.08f; // valor en noche
	const float AMBIENT_MID = (AMBIENT_DAY + AMBIENT_NIGHT) * 0.5f; // 0.3
	const float AMBIENT_AMP = (AMBIENT_DAY - AMBIENT_NIGHT) * 0.5f; // 0.2

	float intensidaddia = 0.4;

	float cambiodia = 0.0f;
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Actualizar animaciones del window (tapa del bote)
		mainWindow.ActualizaAnimaciones(deltaTime);

		// Ciclo dia y noche
		cambiodia += 0.01 * deltaTime; // avanzamos en segundos
		if (cambiodia >= dayNightPeriod) cambiodia = fmod(cambiodia, dayNightPeriod);
		float theta = TWO_PI * cambiodia / dayNightPeriod;
		float sineValue = sinf(theta); // positivo = dia, negativo = noche


		//Recibir eventos del usuario
		glfwPollEvents();
		// get mouse deltas once per frame (reusing variables)
		dx = mainWindow.getXChange();
		dy = mainWindow.getYChange();
		camID = mainWindow.GetActiveCameraID();
		if (camID == 1)
		{
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(dx, dy);
		}
		else
		{
			// still process keyboard for movement (optional)
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			// route mouse to window camera controls
			if (camID == 2)
			{
				mainWindow.AdjustThirdPersonRotation(dx, dy);
			}
			else if (camID == 3)
			{
				mainWindow.AdjustAerialRotation(dx, dy);
			}
		}

		//------- CAMINATA PATO LUCAS ---------
		mainWindow.UpdateMovement(deltaTime);
		// actualizar animacion del cinturon
		mainWindow.UpdateCinturon(deltaTime);
		// Taz animation update (hold Z to animate)
		if (mainWindow.GetTazActive())
		{
			tazOrbitAngle += tazOrbitSpeed * deltaTime;
			tazSpinAngle += tazSpinSpeed * deltaTime;
			if (tazOrbitAngle >= 360.0f) tazOrbitAngle -= 360.0f;
			tazSpinAngle = fmod(tazSpinAngle, 360.0f);
			tornadoY += tornadoRiseSpeed * deltaTime;
			if (tornadoY > 0.0f) tornadoY = 0.0f;
		}
		else
		{
			// when not active, lower tornado back down
			tornadoY -= tornadoRiseSpeed * deltaTime;
			if (tornadoY < -1.5f) tornadoY = -1.5f;
		}
		//---------------------------------- Hasta Aqui--------------------------------


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Cambio de dia y noche: usar el signo de la senoide (>=0 dia, <0 noche)
		if (sineValue >= 0.0f)
		{
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		}
		else
		{
			skyboxNight.DrawSkybox(camera.calculateViewMatrix(), projection);

		}
		// Choose active camera and compute view matrix + camera position/dir
		if (camID == 1)
		{
			// Free camera
			viewMatrix = camera.calculateViewMatrix();
			camWorldPos = camera.getCameraPosition();
			camDir = camera.getCameraDirection();
		}
		else if (camID == 2)
		{
			// Third-person camera following pato Lucas — orbit around Lucas using thirdYawDeg/thirdPitchDeg
			patoPos = glm::vec3(0.0f, 1.0f, -115.0f + mainWindow.getmuevex());
			patoRotY = mainWindow.getRotationY(); // degrees (character facing)
			yawDeg = patoRotY + mainWindow.GetThirdPersonYawDeg()*4;
			pitchDeg = mainWindow.GetThirdPersonPitchDeg();
			yawRad = glm::radians(yawDeg);
			pitchRad = glm::radians(pitchDeg);
			distanceBehind = mainWindow.GetThirdPersonDistance()*-1;
			// direction from target to camera in spherical coordinates
			dirVec.x = sinf(yawRad) * cosf(pitchRad);
			dirVec.y = sinf(pitchRad)*-1;
			dirVec.z = cosf(yawRad) * cosf(pitchRad);
			camPos = patoPos - dirVec * distanceBehind;
			targetPos = patoPos + glm::vec3(0.0f, 1.5f, 0.0f);
			viewMatrix = glm::lookAt(camPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));
			camWorldPos = camPos;
			camDir = glm::normalize(targetPos - camPos);
		}
		else if (camID == 3)
		{
			// Aerial camera
			camWorldPos = mainWindow.GetAerialPosition();
			targetPos = mainWindow.GetAerialTarget();
			viewMatrix = glm::lookAt(camWorldPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));
			camDir = glm::normalize(targetPos - camWorldPos);
		}
		else if (camID == 4)
		{
			// Custom camera group: use selected customCamIndex
			int idx = mainWindow.GetCurrentCustomCameraIndex();
			camWorldPos = mainWindow.GetCustomCameraPosition(idx);
			targetPos = mainWindow.GetCustomCameraTarget(idx);
			viewMatrix = glm::lookAt(camWorldPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));
			camDir = glm::normalize(targetPos - camWorldPos);
		}
		else
		{
			viewMatrix = camera.calculateViewMatrix();
			camWorldPos = camera.getCameraPosition();
			camDir = camera.getCameraDirection();
		}



		// Use main shader
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//informacion en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();


		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniform3f(uniformEyePosition, camWorldPos.x, camWorldPos.y, camWorldPos.z);

		//// luz ligada a la camara de tipo flash
		//lowerLight = camWorldPos;
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camDir);

		// Actualizar animacion de la luz del ring dentro de Window (mantiene jerarquia)
		mainWindow.UpdateRingLight(deltaTime);
		ringPos = mainWindow.GetRingLightWorldPos();
		ringDir = mainWindow.GetRingLightWorldDir();
		spotLights[0].SetFlash(ringPos, ringDir);


		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);

		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		////Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -3.7f, -75.5f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Astra_M.RenderModel();

		float ambient = AMBIENT_MID + AMBIENT_AMP * sineValue;
		if (ambient < AMBIENT_NIGHT) ambient = AMBIENT_NIGHT;
		if (ambient > AMBIENT_DAY) ambient = AMBIENT_DAY;

		mainLight.SetAmbientIntensity(ambient);
		shaderList[0].SetDirectionalLight(&mainLight);




		// Control simple de point lights: dia -> apagadas, noche -> encendidas
		if (sineValue >= 0.0f)
		{
			if (mainWindow.getAntorchaEncendida())
			{
				SpotLight active[2] = { spotLights[1], spotLights[2] };
				shaderList[0].SetSpotLights(active, 2);
				//shaderList[0].SetSpotLights(&spotLights[1], 0);
			}
			else
			{
				shaderList[0].SetSpotLights(&spotLights[1], 0);
			}
			shaderList[0].SetPointLights(pointLights, 0);

		}
		else
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);

			if (mainWindow.getAntorchaEncendida())
			{
				/*SpotLight active[2] = { spotLights[1], spotLights[2] };
				shaderList[0].SetSpotLights(active, 2);*/
				shaderList[0].SetSpotLights(spotLights, 3);
			}
			else
			{
				shaderList[0].SetSpotLights(&spotLights[0], 1);
			}
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//------------------------------
		//----------RENDERIZADO---------
		//------------------------------

		//-------ESTRUCTURAS-----------

		//PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Piso_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PisoCamino_M.RenderModel();

		//---Tenochtitlan

		//TemploMayor
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 4.2f, -17.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.8f, 2.2f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TemploMayor_M.RenderModel();

		//Tezcatlipoca (esquina izq)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 6.5f, -97.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.5f, 4.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TemploTezcatlipoca_M.RenderModel();

		//Tezcatlipoca (esquina der)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 6.0f, 97.0f));
		//model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.3f, 4.3f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TemploTezcatlipoca_M.RenderModel();

		//Calmecac
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.0f, -1.5f, 97.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Calmecac_M.RenderModel();

		//Tlachtli
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.5f, 63.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tlachtli_M.RenderModel();

		//Tonatiuht
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-38.5f, -0.8f, -75.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.55, 1.6f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tonatiuht_M.RenderModel();

		//Coacalco 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.0f, -1.8f, -56.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coacalco_M.RenderModel();

		//Cihuacoatl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.0f, -0.65f, -43.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cihuacoatl_M.RenderModel();

		//Chicomecoatl 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(19.0f, -1.73f, 33.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Chicomecoatl_M.RenderModel();

		//Xochiquetzal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(19.0f, -0.63f, 43.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Xochiquetzal_M.RenderModel();

		//Ehecatl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -1.2f, -100.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ehecatl_M.RenderModel();


		//Ring y Sillas


		//Ring y Sillas

		//Ring 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, -2.0f, -31.0f));
		modelauxRing = model;
		modelauxSillaB = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ring_M.RenderModel();

		//Ring 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-61.0f, -2.0f, -17.0f));
		modelauxSillaR = model;
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ring_M.RenderModel();
		//-----Animaciones complejas---
		//Cinturon
		model = modelaux;
		float yc = mainWindow.GetCinturonY();
		model = glm::translate(model, glm::vec3(0.0f, yc, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cinturon_M.RenderModel();
		//----- Fin Animacion complejas----

		//Ring 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, -2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ring_M.RenderModel();


		//Sillas blancas
		model = modelauxSillaB;
		model = glm::translate(model, glm::vec3(-10.0f, 1.0f, -10.0f));
		model = glm::rotate(model, 250.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sillas_B_M.RenderModel();

		//Sillas rojos
		model = modelauxSillaR;
		model = glm::translate(model, glm::vec3(-6.5f, 1.0f, 12.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sillas_R_M.RenderModel();


		//Sillas verdes
		model = modelauxSillaR;
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -10.0f));
		model = glm::rotate(model, 200.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sillas_V_M.RenderModel();


		//-Extras-

		//Banca 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -1.0f, 53.7f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -1.0f, -73.7f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote CSP
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.05f, -66.7f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.35f, 1.35f, 1.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_CSP_M.RenderModel();

		//Bote ZZZ
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.0f, 0.05f, 53.7f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_E_M.RenderModel();

		//Bote Marvin
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.05f, 25.7f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M_M.RenderModel();

		//---Mundos---

		//-Casa Chicas SuperPoderosas
		//Casa y arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(51.0f, -2.2f, -51.0f));

		modelaux = model;
		modelaux2 = model;

		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.75f, 1.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaCSP_M.RenderModel();

		//Garaje
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.85f, 3.0f, 1.7f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 1.6f, 1.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaCSP_PyG_M.RenderModel();

		//Ventanas
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.02f, 4.0f, -2.2f));
		model = glm::rotate(model, 88.35f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.4f, 1.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaCSP_V_M.RenderModel();

		//-Casa Marvin
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(58.0f, -1.6f, 17.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.75f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaMarvin_M.RenderModel();

		//Cafeter a 
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-42.0f, 2.5f, 18.0f));
		cafelamp = model;
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cafe_ZZZ.RenderModel();



		model = cafelamp;
		model = glm::translate(model, glm::vec3(-9.5f, 6.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		spotLights[1].SetPos(glm::vec3(model[3].x, model[3].y - 2.1f, model[3].z));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		foco.RenderModel();

		model = cafelamp;
		model = glm::translate(model, glm::vec3(-9.5f, 6.0f, 4.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		spotLights[2].SetPos(glm::vec3(model[3].x, model[3].y - 2.1f, model[3].z));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		foco.RenderModel();

		//-------PERSONAJES-----------

		//-----AVATAR----
		//CAMINATA PATO LUCAS
		//Pato lucas cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 01.0f, -115.0f + mainWindow.getmuevex()));
		model = glm::rotate(model, mainWindow.getRotationY() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		modelaux = model;
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //para que lucas mire hacia donde gustes xd
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patoLucasCuerpo_M.RenderModel();

		//Pato lucas brazo derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.04f, -0.009f, 0.0f));
		model = glm::rotate(model, mainWindow.getarticulacion1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patoLucas_ManoDer_M.RenderModel();

		//pato lucas brazo izquierdo 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.004f, 0.009f, 0.0f));
		model = glm::rotate(model, -mainWindow.getarticulacion1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patoLucas_ManoIzq_M.RenderModel();

		//pato lucas pierna derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.05f, -0.36f, 0.0f));
		model = glm::rotate(model, -mainWindow.getarticulacion1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patoLucas_PieDer_M.RenderModel();

		//pato lucas pierna izquierda 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.05f, -0.36f, 0.047f));
		model = glm::rotate(model, mainWindow.getarticulacion1() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patoLucas_PieIzq_M.RenderModel();
		//---------------------------------- Hasta Aqui--------------------------------
		//Bombon
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, -1.0f, 60.7f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bombon_M.RenderModel();

		//Burbuja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 0.15f, 25.7f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Burbuja_M.RenderModel();

		//Bellota
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-47.0f, 1.15f, -35.85f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bellota_M.RenderModel();

		//Mojo Jojo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.3f, -2.6f, -39.0f));
		model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MojoJojo_M.RenderModel();
		modelauxMojo = model;

		//El
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -3.7f, -73.7f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		El_M.RenderModel();

		//-Looney Tunes

		//Bugs Bunny
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, 15.2f, -17.0f));
		parentModelMatrix = model;
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BugsBunny_M.RenderModel();
		// store parent's world matrix for hierarchical child rendering
		if (keyframeParent == &BugsBunny_M) parentModelMatrix2 = model;

		

		//Marvin
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -2.0f, 53.7f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Marvin_M.RenderModel();

		//Coyote
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -2.2f, -17.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coyote_M.RenderModel();

		//Taz Demonio de Tasmania (animated when Z is held)
		{
			float rad = glm::radians(tazOrbitAngle);
			float ox = cos(rad) * tazOrbitRadius;
			float oz = sin(rad) * tazOrbitRadius;
			model = glm::mat4(1.0f);
			// orbit position
			model = glm::translate(model, glm::vec3(ox, -0.7f, oz));
			// self spin
			model = glm::rotate(model, glm::radians(tazSpinAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			taz_M.RenderModel();
		}

		//Taz tornado (hierarchical)
		{
			model = modelaux;
			// tornado rises from below to tornadoY (animated)
			model = glm::translate(model, glm::vec3(0.0f, tornadoY, 0.0f));
			// spin tornado for visual effect
			model = glm::rotate(model, glm::radians(tazOrbitAngle * 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			taz_Tornado_M.RenderModel();
		}

		//-Zenles Zone Zero 
		//Belle
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -2.2f, -19.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		belle.RenderModel();

		// --- Render del modelo animado usando keyframes (Animacion1_M)
		// Ahora jerarquizado respecto al modelo padre seleccionado (por defecto BugsBunny_M).
		{
			glm::vec3 posAnim = mainWindow.GetAnimModelPos();
			float rotYAnim = mainWindow.GetAnimModelRotY();
			float rotZAnim = mainWindow.GetAnimModelRotZ();
			// Start from parent's world matrix (mirror Pato Lucas hierarchy style)
			parentModelMatrix = translate(parentModelMatrix, glm::vec3(-4.0f, 2.0f, -1.0f));
			model = parentModelMatrix;
			// apply local keyframe transform relative to parent
			model = glm::translate(model, posAnim);
			model = glm::rotate(model, rotYAnim * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotZAnim * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Animacion1_M.RenderModel();
		}
		// --- Render del modelo animado usando keyframes (Animacion2_M) ZANAHORIA
		{
			glm::vec3 posAnim = mainWindow.GetAnimModelPos();
			float rotYAnim = mainWindow.GetAnimModelRotY();
			float rotZAnim = mainWindow.GetAnimModelRotZ();
			// Start from parent's world matrix (mirror Pato Lucas hierarchy style)
			model = parentModelMatrix2;
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
			// apply local keyframe transform relative to parent
			model = glm::translate(model, posAnim);
			model = glm::rotate(model, rotYAnim * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotZAnim * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Animacion2_M.RenderModel();
		}

		{
			glm::vec3 posAnim = mainWindow.GetAnimModelPos();
			float rotYAnim = mainWindow.GetAnimModelRotY();
			float rotZAnim = mainWindow.GetAnimModelRotZ();
			model = modelauxMojo;
			model = glm::translate(model, posAnim);
			model = glm::rotate(model, rotYAnim * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotZAnim * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Animacion3_M.RenderModel();
		}

		//

		//Vivian
		model = modelauxRing;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -10.85f));
		model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		//model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Vivian_M.RenderModel();

		//Astra

		//Eus
		model = modelauxRing;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -20.85f));
		model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Eus_M.RenderModel();

		//Ellen
		model = modelauxRing;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, -10.85f));
		model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ellen.RenderModel();

		//Animaciones sencillas
		//-Bote de basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -1.5f, -8.0f));

		modelauxBote = model;

		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BoteLuchaLibre_M.RenderModel();

		//Tapa
		model = modelauxBote;
		model = glm::translate(model, glm::vec3(0.7f, 1.3f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));

		model = glm::rotate(model, glm::radians(mainWindow.getTapaBasuraRotX()), glm::vec3(1.0f, 0.0f, .0f));

		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_LL_Tapa_M.RenderModel();


		//-Lata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.5f + mainWindow.getLataPosY(), -8.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lata_M.RenderModel();

		//-Campana
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-54.0f, 0.0f, -17.0f));

		modelauxCampana = model;

		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		//model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Campana_M.RenderModel();

		//Martillo de la campana
		model = modelauxCampana;
		model = glm::translate(model, glm::vec3(-0.8f, 3.8f, 0.7f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, .0f));

		// Rotaci n en X del martillo seg n la animaci n del Window
		model = glm::rotate(model, glm::radians(mainWindow.getMartilloRotX()), glm::vec3(1.0f, 0.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Campana_M_M.RenderModel();

		//iluminacion de las antorchas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -2.2f, 0.0f)); // posici n base
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		// Calcular posici n del fuego (luz)
		modelauxfuegoPos = glm::vec3(model * glm::vec4(0.0f, 5.5f, 0.0f, 1.0f));
		pointLights[0].SetPos(modelauxfuegoPos);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		antorcha_piso_M.RenderModel();


		//Antorcha de piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -2.2f, -35.0f)); // posici n base
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		// Calcular posici n del fuego (luz)
		modelauxfuegoPos = glm::vec3(model * glm::vec4(0.0f, 5.5f, 0.0f, 1.0f));
		pointLights[1].SetPos(modelauxfuegoPos);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		antorcha_piso_M.RenderModel();

		


		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;

}