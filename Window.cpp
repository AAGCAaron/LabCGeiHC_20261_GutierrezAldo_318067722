#include "Window.h"
#include <cmath>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdarg>

static const float PI_F = 3.14159265358979323846f;

// simple print wrapper to replace std::cout usage
static void print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}


Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	antorchaEncendida = false;

	//Animaciones basicas
	// inicializar animaciones
	tapaBasuraRotX = 0.0f;
	lataPosY = 0.0f;
	tapaAnimando = false;
	tapaObjetivoAbierta = false;

	lataAnimando = false;
	lataObjetivoY = 1.0f; // valor por defecto (posicion inicial)

	// martillo
	martilloRotX = 0.0f;
	martilloAnimando = false;
	martilloDireccion = -1.0f; // empieza movindose hacia negativo


	// initialize movement state
	muevex = 0.0f;
	movementState = WALKING;
	moveDir = 1; // forward
	walkTarget = 100.0f;
	walkSpeed = 10.0f; // units per second (adjustable)
	rotationY = 0.0f;
	rotationDuration = 5.0f; // seconds to rotate 180 deg
	rotationElapsed = 0.0f;
	rotationStart = 0.0f;
	rotationTarget = 0.0f;

	// Taz
	tazActive = false;

	// Initialize ring light state (defaults)
	ringLightAngle = 0.0f;
	ringLightOrbitSpeed = 60.0f; // deg/s
	ringLightSpinAngle = 0.0f;
	ringLightSpinSpeed = 90.0f; // deg/s
	ringLightRadius = 6.0f;
	ringLightTiltDeg = 30.0f; // tilt downwards from horizontal? we'll apply below
	ring2Center = glm::vec3(-61.0f, -2.0f, -17.0f);
	ring2Model = glm::mat4(1.0f);
	ring2Model = glm::translate(ring2Model, ring2Center);
	ring2Model = glm::scale(ring2Model, glm::vec3(1.5f, 1.5f, 1.5f));
	ringLightLocalOffset = glm::vec3(0.0f);

	// Inicializacion cinturon
	cinturonActive = false;
	cinturonY = 0.0f;
	cinturonTargetY = 0.5f;
	cinturonRiseSpeed = 1.0f; // unidades por segundo
	cinturonAtTarget = false;
	cinturonFloatAmplitude = 0.08f; // amplitud de flotacion
	cinturonFloatFreq = 1.0f; // Hz
	cinturonTime = 0.0f;

	// camera default
	activeCameraID = 1; // free camera

	// third-person / aerial defaults
	thirdPersonDistance = 8.0f;
	thirdPersonHeight = 3.0f;
	thirdYawDeg = 0.0f;
	thirdPitchDeg = 10.0f; // look slightly down
	mouseSensitivityDeg = 0.15f; // degrees per pixel

	aerialPosition = glm::vec3(0.0f, 40.0f, 60.0f);
	aerialTarget = glm::vec3(0.0f, 0.0f, -100.0f);
	aerialYawDeg = 0.0f;
	aerialPitchDeg = -30.0f;
	aerialRadius = glm::length(aerialPosition - aerialTarget);

	mouseFirstMoved = true;
	lastX = 0.0f; lastY = 0.0f;
	xChange = 0.0f; yChange = 0.0f;

	// Keyframe animation defaults
	canSaveKeyframe = false;
	keyframesLocked = false;
	playbackActive = false;
	playbackOnce = false; // new: default false
	playbackTimer = 0.0f;
	playbackIndex = 0;
	segmentDuration = 1.0f; // 1 second per segment
	animModelPos = glm::vec3(0.0f);
	animModelRotY = 0.0f;
	animModelRotZ = 0.0f; // initialize Z rotation

	// Initialize custom cameras to safe defaults to avoid uninitialized reads
	customCamIndex = 0;
	customCamPos[0] = glm::vec3(0.0f, 12.0f, -30.0f); customCamTarget[0] = glm::vec3(0.0f, 0.0f, -115.0f);
	customCamPos[1] = glm::vec3(30.0f, 10.0f, -60.0f); customCamTarget[1] = glm::vec3(10.0f, 0.0f, -80.0f);
	customCamPos[2] = glm::vec3(-30.0f, 15.0f, -40.0f); customCamTarget[2] = glm::vec3(-10.0f, 0.0f, -100.0f);
	customCamPos[3] = glm::vec3(0.0f, 40.0f, 20.0f); customCamTarget[3] = glm::vec3(0.0f, 0.0f, -20.0f);
	customCamPos[4] = glm::vec3(50.0f, 8.0f, -10.0f); customCamTarget[4] = glm::vec3(20.0f, 0.0f, -30.0f);
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 0.0f; // start at 0

	//animacion basica
	tapaBasuraRotX = 0.0f;
	lataPosY = 0.0f;
	tapaAnimando = false;
	tapaObjetivoAbierta = false;

	lataAnimando = false;
	lataObjetivoY = 1.0f;

	// martillo
	martilloRotX = 0.0f;
	martilloAnimando = false;
	martilloDireccion = -1.0f;


	movementState = WALKING;
	moveDir = -1; // forward
	walkTarget = 200.0f;
	walkSpeed = 0.06f; // entre mas pequeno mas lento
	rotationY = 0.0f;
	rotationDuration = 50.0f;
	rotationElapsed = 0.0f;
	rotationStart = 0.0f;
	rotationTarget = 0.0f;

	// Taz
	tazActive = false;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	

	// Initialize ring light state (defaults)
	ringLightAngle = 0.0f;
	ringLightOrbitSpeed = 60.0f;
	ringLightSpinAngle = 0.0f;
	ringLightSpinSpeed = 90.0f;
	ringLightRadius = 6.0f;
	ringLightTiltDeg = 30.0f;
	ring2Center = glm::vec3(-61.0f, -2.0f, -17.0f);
	ring2Model = glm::mat4(1.0f);
	ring2Model = glm::translate(ring2Model, ring2Center);
	ring2Model = glm::scale(ring2Model, glm::vec3(1.5f, 1.5f, 1.5f));
	ringLightLocalOffset = glm::vec3(0.0f);

	// Inicializacion cinturon
	cinturonActive = false;
	cinturonY = 0.0f;
	cinturonTargetY = 0.5f;
	cinturonRiseSpeed = 1.0f; // unidades por segundo
	cinturonAtTarget = false;
	cinturonFloatAmplitude = 0.08f; // amplitud de flotacion
	cinturonFloatFreq = 1.0f; // Hz
	cinturonTime = 0.0f;

	// camera default
	activeCameraID = 1; // free camera

	// third-person / aerial defaults
	thirdPersonDistance = 8.0f;
	thirdPersonHeight = 5.0f;
	thirdYawDeg = 0.0f;
	thirdPitchDeg = 10.0f;
	mouseSensitivityDeg = 0.15f;

	aerialPosition = glm::vec3(0.0f, 100.0f, 60.0f);
	aerialTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	aerialYawDeg = 0.0f;
	aerialPitchDeg = -30.0f;
	aerialRadius = glm::length(aerialPosition - aerialTarget);

	mouseFirstMoved = true;
	lastX = 0.0f; lastY = 0.0f;
	xChange = 0.0f; yChange = 0.0f;

	// Keyframe animation defaults
	canSaveKeyframe = false;
	keyframesLocked = false;
	playbackActive = false;
	playbackOnce = false;
	playbackTimer = 0.0f;
	playbackIndex = 0;
	segmentDuration = 1.0f; // 1 second per segment
	animModelPos = glm::vec3(0.0f);
	animModelRotY = 0.0f;
	animModelRotZ = 0.0f;

	// Initialize custom cameras to safe defaults to avoid uninitialized reads
	customCamIndex = 0;
	customCamPos[0] = glm::vec3(0.0f, 12.0f, -30.0f); customCamTarget[0] = glm::vec3(0.0f, 0.0f, -115.0f);
	customCamPos[1] = glm::vec3(30.0f, 10.0f, -60.0f); customCamTarget[1] = glm::vec3(10.0f, 0.0f, -80.0f);
	customCamPos[2] = glm::vec3(-30.0f, 15.0f, -40.0f); customCamTarget[2] = glm::vec3(-10.0f, 0.0f, -100.0f);
	customCamPos[3] = glm::vec3(0.0f, 40.0f, 20.0f); customCamTarget[3] = glm::vec3(0.0f, 0.0f, -20.0f);
	customCamPos[4] = glm::vec3(50.0f, 8.0f, -10.0f); customCamTarget[4] = glm::vec3(20.0f, 0.0f, -30.0f);
}

int Window::Initialise()
{
	//Inicializacion de GLFW
	if (!glfwInit())
	{
		printf("Fallo inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamano de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo inicializacion de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se esta usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


//------------- Animaciones basicas -----------------

void Window::ActualizaAnimaciones(GLfloat deltaTime)
{
	// Animar la tapa del bote.  ngulo objetivo: 0 (cerrada) - 90 (abierta)
	if (tapaAnimando)
	{
		const GLfloat velocidadGradosPorSeg = 120.0f; // grados por segundo (ajustable)
		if (tapaObjetivoAbierta)
		{
			tapaBasuraRotX += velocidadGradosPorSeg * deltaTime;
			if (tapaBasuraRotX >= 90.0f)
			{
				tapaBasuraRotX = 90.0f;
				tapaAnimando = false;
			}
		}
		else
		{
			tapaBasuraRotX -= velocidadGradosPorSeg * deltaTime;
			if (tapaBasuraRotX <= 0.0f)
			{
				tapaBasuraRotX = 0.0f;
				tapaAnimando = false;
			}
		}
	}

	// Animar la lata en Y. La lata se mover  entre lataObjetivoY y una posici n lmite (por ejemplo -2.0f)
	if (lataAnimando)
	{
		const GLfloat velocidadYPorSeg = 2.0f; // unidades Y por segundo (ajustable)
		// si el objetivo es menor que la posici n actual, caer ; si es mayor, subir 
		if (lataPosY > lataObjetivoY)
		{
			lataPosY -= velocidadYPorSeg * deltaTime;
			if (lataPosY <= lataObjetivoY)
			{
				lataPosY = lataObjetivoY;
				lataAnimando = false;
			}
		}
		else if (lataPosY < lataObjetivoY)
		{
			lataPosY += velocidadYPorSeg * deltaTime;
			if (lataPosY >= lataObjetivoY)
			{
				lataPosY = lataObjetivoY;
				lataAnimando = false;
			}
		}
	}

	// Animaci n del martillo: rotaci n en X para simular golpe
	if (martilloAnimando)
	{
		const GLfloat velocidadMartilloGradosPorSeg = 180.0f; // velocidad de oscilaci n
		// Oscilar entre -45 y 0 grados (golpe hacia delante y retorno)
		martilloRotX += martilloDireccion * velocidadMartilloGradosPorSeg * deltaTime;
		if (martilloRotX <= -45.0f)
		{
			martilloRotX = -45.0f;
			martilloDireccion = 1.0f; // invertir direcci n para regresar
		}
		else if (martilloRotX >= 0.0f)
		{
			martilloRotX = 0.0f;
			martilloDireccion = -1.0f; // invertir para volver a golpear
		}
	}

	// Update keyframe playback if active
	if (playbackActive && keyframes.size() > 0)
	{
		// ensure playbackIndex in range
		if (playbackIndex < 0) playbackIndex = 0;
		if (playbackIndex >= (int)keyframes.size()) playbackIndex = 0;

		int nextIndex = (playbackIndex + 1) % keyframes.size();
		const Keyframe &kf0 = keyframes[playbackIndex];
		const Keyframe &kf1 = keyframes[nextIndex];
		// advance timer
		playbackTimer += deltaTime;
		float t = playbackTimer / segmentDuration;
		if (t >= 1.0f)
		{
			// advance to next segment
			playbackIndex = nextIndex;
			playbackTimer = fmod(playbackTimer, segmentDuration);
			t = playbackTimer / segmentDuration;
			// If we reached the last frame and playbackOnce is true, stop
			if (playbackOnce && playbackIndex == 0)
			{
				// we've looped back to start -> stop playback and set final frame
				playbackActive = false;
				// set to last keyframe transform
				const Keyframe &lastKf = keyframes.back();
				animModelPos = lastKf.pos;
				animModelRotY = lastKf.rotY;
				print("[Playback] Reproduccion one-shot finalizada.\n");
				return;
			}
		}
		// interpolate position and rotation (LERP and shortest angle)
		animModelPos = glm::mix(kf0.pos, kf1.pos, t);
		// rotation: interpolate properly across wrap-around
		float a0 = glm::radians(kf0.rotY);
		float a1 = glm::radians(kf1.rotY);
		// compute shortest angular difference
		float diff = a1 - a0;
		while (diff > PI_F) diff -= 2.0f * PI_F;
		while (diff < -PI_F) diff += 2.0f * PI_F;
		float a = a0 + diff * t;
		animModelRotY = glm::degrees(a);
		// preserve animModelRotZ (controlled by Q/E)
	}
}

//----------- Fin animaciones basicas ---------------------

void Window::SaveKeyframesToFile(const char* filename)
{
	std::ofstream ofs(filename);
	if (!ofs) return;
	for (size_t i = 0; i < keyframes.size(); ++i)
	{
		ofs << keyframes[i].pos.x << " " << keyframes[i].pos.y << " " << keyframes[i].pos.z << " " << keyframes[i].rotY << "\n";
	}
	ofs.close();
	keyframesLocked = true;
}

void Window::LoadKeyframesFromFile(const char* filename)
{
	std::ifstream ifs(filename);
	if (!ifs) return;
	keyframes.clear();
	std::string line;
	while (std::getline(ifs, line))
	{
		std::istringstream iss(line);
		Keyframe kf;
		if (!(iss >> kf.pos.x >> kf.pos.y >> kf.pos.z >> kf.rotY)) break;
		keyframes.push_back(kf);
	}
	ifs.close();
	// reset current animated transform to first keyframe if exists
	if (!keyframes.empty()){
		animModelPos = keyframes[0].pos;
		animModelRotY = keyframes[0].rotY;
	}
}

void Window::AppendCurrentKeyframe()
{
	if (keyframesLocked) return; // cannot append when locked
	Keyframe kf;
	kf.pos = animModelPos;
	kf.rotY = animModelRotY;
	keyframes.push_back(kf);
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	// --------- Animaciones basicas con teclas -----------------

	//---------Animacion basica - Bote (toggle con B)
	// Al presionar B se inicia la animaci n para abrir o cerrar seg n estado actual.
	
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		theWindow->antorchaEncendida = !theWindow->antorchaEncendida;
		//print("[Tecla 6] Antorcha: %s\n", (theWindow->antorchaEncendida ? "encendida" : "apagada"));
	}
	
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		// Si est  en movimiento, ignorar nueva pulsaci n
		if (!theWindow->tapaAnimando)
		{
			if (theWindow->tapaBasuraRotX <= 0.01f)
				theWindow->tapaObjetivoAbierta = true; // abrir
			else if (theWindow->tapaBasuraRotX >= 89.99f)
				theWindow->tapaObjetivoAbierta = false; // cerrar
			else
				theWindow->tapaObjetivoAbierta = !theWindow->tapaObjetivoAbierta; // invertir si estaba a la mitad

			theWindow->tapaAnimando = true;
			print("[Tecla B] Animacion tapa: iniciado toggle.\n");
		}
	}

	//-Lata: al presionar N inicia ca da hasta -2.0f, si ya est  abajo la regresa a 1.0f
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (!theWindow->lataAnimando)
		{
			// si la lata est  en su posici n inicial o por encima, cae; si est  abajo, sube
			if (theWindow->lataPosY > -1.9f)
			{
				theWindow->lataObjetivoY = -2.0f; // caer
				print("[Tecla N] Lata: caer activado.\n");
			}
			else
			{
				theWindow->lataObjetivoY = 1.0f; // volver a la posici n inicial
				print("[Tecla N] Lata: subir activado.\n");
			}
			theWindow->lataAnimando = true;
		}
	}

	// Martillo: tecla M activa/desactiva la animaci n del martillo
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		theWindow->martilloAnimando = !theWindow->martilloAnimando;
		// si se activa, asegurarse de empezar desde 0 y con direcci n hacia golpe
		if (theWindow->martilloAnimando)
		{
			theWindow->martilloRotX = 0.0f;
			theWindow->martilloDireccion = -1.0f;
			print("[Tecla M] Martillo: animacion activada.\n");
		}
		else
		{
			print("[Tecla M] Martillo: animacion desactivada.\n");
		}
	}

	// ------ Fin animciones basicas -----------------


	//---------- Animaciones complejas con teclas  -----------------
	// Taz toggle (mantener con tecla)
	if (key == GLFW_KEY_Z)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			theWindow->tazActive = true;
			print("[Tecla Z] Taz: activo (mantener para animar).\n");
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->tazActive = false;
			print("[Tecla Z] Taz: desactivado.\n");
		}
	}

	// Cinturon toggle: presionar X para alternar animacion
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		theWindow->SetCinturonActive(!theWindow->GetCinturonActive());
		print("[Tecla X] Cinturon: %s\n", (theWindow->GetCinturonActive() ? "activado" : "desactivado"));
	}

	//---------- Fin Animaciones complejas -----------------


	//--------- Control camaras -----------------

	// Camera switching: keys 1,2,3, and 4 for custom cameras
	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) && action == GLFW_PRESS)
	{
		int id = 1;
		if (key == GLFW_KEY_1) id = 1;
		if (key == GLFW_KEY_2) id = 2;
		if (key == GLFW_KEY_3) id = 3;
		theWindow->SetActiveCamera(id);
		print("[Teclas 1/2/3] Camara activa: %d\n", id);
	}
	// Custom camera cycle: press 4 to cycle through 5 custom cameras
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		// advance index using CycleCustomCamera() (ensures proper wrapping)
		theWindow->CycleCustomCamera();
		print("[Tecla 4] Camara personalizada seleccionada: %d (0-based)\n", theWindow->GetCurrentCustomCameraIndex());
	}

// Aerial height adjust: 9 up, 0 down
	if ((key == GLFW_KEY_9 || key == GLFW_KEY_8) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		float delta = 2.0f;
		if (key == GLFW_KEY_9) theWindow->ModifyAerialHeight(delta);
		else theWindow->ModifyAerialHeight(-delta);
		print("[Teclas 8/9] Ajuste altura aerial.\n");
	}

	// --------- Keyframe controls (additional keys) -----------------
	// Space: one-shot playback (toggle was previously loop)
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!theWindow->playbackActive)
		{
			// start playback once
			if (!theWindow->keyframes.empty())
			{
				theWindow->playbackActive = true;
				theWindow->playbackOnce = true;
				theWindow->playbackTimer = 0.0f;
				theWindow->playbackIndex = 0;
				print("[Barra espacio] Reproduccion one-shot iniciada.\n");
			}
		}
		else
		{
			// if already playing, ignore or allow manual stop
			print("[Barra espacio] Reproduccion ya en curso.\n");
		}
	}
	// 0: reset to initial keyframe and pause playback until SPACE is pressed
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		theWindow->playbackActive = false; // pause playback
		theWindow->playbackTimer = 0.0f;
		theWindow->playbackIndex = 0;
		// move model to first keyframe if available
		if (!theWindow->keyframes.empty())
		{
			theWindow->animModelPos = theWindow->keyframes[0].pos;
			theWindow->animModelRotY = theWindow->keyframes[0].rotY;
		}
		// reset Z rotation user control
		theWindow->animModelRotZ = 0.0f;
		print("[Tecla 0] Reposicionada al frame inicial y playback en pausa. Presione SPACE para reproducir.\n");
	}
	// P toggles ability to save a new frame (kept on key 5 earlier)
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		theWindow->canSaveKeyframe = !theWindow->canSaveKeyframe;
		print("[Tecla 5] Guardado de frames: %s\n", (theWindow->canSaveKeyframe ? "HABILITADO" : "DESHABILITADO"));
	}
	// L: save current frame (only if canSaveKeyframe true)
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (theWindow->canSaveKeyframe && !theWindow->keyframesLocked)
		{
			theWindow->AppendCurrentKeyframe();
			print("[Tecla L] Keyframe guardado. Total frames: %zu\n", theWindow->keyframes.size());
		}
		else
		{
			print("[Tecla L] No se puede guardar: %s\n", (theWindow->keyframesLocked ? "Keyframes bloqueados" : "Guardado deshabilitado (presione 5)"));
		}
	}
	// C: save all keyframes to file (example filename passed below)
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		// example default file - user requested keyframesXX.txt naming
		theWindow->SaveKeyframesToFile("keyframe01.txt");
		print("[Tecla C] Keyframes guardados en 'keyframe01.txt' y ahora bloqueados para append.\n");
	}
	// K: unlock saving after C
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		theWindow->keyframesLocked = false;
		print("[Tecla K] Keyframes desbloqueados para agregar nuevos frames.\n");
	}

	// Q/E: rotate animModel around Z (Q negative, E positive)
	if ((key == GLFW_KEY_Q || key == GLFW_KEY_E) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		float rotDelta = 5.0f; // degrees per press
		if (key == GLFW_KEY_Q) theWindow->animModelRotZ -= rotDelta;
		if (key == GLFW_KEY_E) theWindow->animModelRotZ += rotDelta;
		// clamp or wrap if desired
		if (theWindow->animModelRotZ > 360.0f) theWindow->animModelRotZ = fmod(theWindow->animModelRotZ, 360.0f);
		if (theWindow->animModelRotZ < -360.0f) theWindow->animModelRotZ = fmod(theWindow->animModelRotZ, 360.0f);
		print("[Rot Z keyframe] RotZ: %f\n", theWindow->animModelRotZ);
	}

	// U/I/V: load different keyframe files
	if ((key == GLFW_KEY_U || key == GLFW_KEY_I || key == GLFW_KEY_V) && action == GLFW_PRESS)
	{
		const char* fname = nullptr;
		if (key == GLFW_KEY_U) fname = "keyframe01.txt";
		if (key == GLFW_KEY_I) fname = "keyframe02.txt";
		if (key == GLFW_KEY_V) fname = "keyframe03.txt";
		if (fname)
		{
			// Load and reset playback state so the animation can be played later
			theWindow->LoadKeyframesFromFile(fname);
			// ensure playback is stopped and reset to first frame
			theWindow->playbackActive = false;
			theWindow->playbackOnce = false;
			theWindow->playbackTimer = 0.0f;
			theWindow->playbackIndex = 0;
			if (!theWindow->keyframes.empty())
			{
				// set current animated transform to first keyframe so it appears in scene immediately
				theWindow->animModelPos = theWindow->keyframes[0].pos;
				theWindow->animModelRotY = theWindow->keyframes[0].rotY;
				print("[Tecla %c] Cargado archivo: %s (frames: %zu). Animacion lista para reproducir.\n",
					(key==GLFW_KEY_U?'U':(key==GLFW_KEY_I?'I':'V')), fname, theWindow->keyframes.size());
			}
			else
			{
				print("[Tecla %c] Error: no se cargaron frames desde: %s\n",
					(key==GLFW_KEY_U?'U':(key==GLFW_KEY_I?'I':'V')), fname);
			}
		}
	}

	// Movement adjustments: J/G Z axis, Y/H Y axis, P/O X axis
	// Reuse toggles: T enables reuse for Z (not implemented as toggle state here), U for Y, I for X, E for rotation
	if ((key == GLFW_KEY_J || key == GLFW_KEY_G || key == GLFW_KEY_Y || key == GLFW_KEY_H || key == GLFW_KEY_P || key == GLFW_KEY_O || key == GLFW_KEY_R || key == GLFW_KEY_F) && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// small delta per press; hold will repeat
		float delta = 0.1f; // translation per press (can be adjusted)
		float rotDelta = 5.0f; // degrees per press
		// Choose which axis to modify
		if (key == GLFW_KEY_J) theWindow->animModelPos.z += delta; // move Z positive
		if (key == GLFW_KEY_G) theWindow->animModelPos.z -= delta; // move Z negative
		if (key == GLFW_KEY_Y) theWindow->animModelPos.y += delta; // move Y positive
		if (key == GLFW_KEY_H) theWindow->animModelPos.y -= delta; // move Y negative
		if (key == GLFW_KEY_P) theWindow->animModelPos.x += delta; // move X positive
		if (key == GLFW_KEY_O) theWindow->animModelPos.x -= delta; // move X negative
		if (key == GLFW_KEY_R) theWindow->animModelRotY += rotDelta; // rotate Y positive
		if (key == GLFW_KEY_F) theWindow->animModelRotY -= rotDelta; // rotate Y negative
		// normalize rotation
		if (theWindow->animModelRotY > 360.0f) theWindow->animModelRotY = fmod(theWindow->animModelRotY, 360.0f);
		print("[Movimiento keyframe] Pos: (%f, %f, %f) RotY: %f RotZ: %f\n", theWindow->animModelPos.x, theWindow->animModelPos.y, theWindow->animModelPos.z, theWindow->animModelRotY, theWindow->animModelRotZ);
	}

	// movimiento control y demas (sin cambios)
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			theWindow->keys[key] = true;
		else if (action == GLFW_RELEASE)
			theWindow->keys[key] = false;
	}

	// --------- Fin control camaras  -----------------
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}



// ---------- Camaras -----------------


void Window::AdjustThirdPersonRotation(float dx, float dy)
{
	thirdYawDeg += dx * mouseSensitivityDeg;
	thirdPitchDeg += dy * mouseSensitivityDeg;
	// clamp pitch
	if (thirdPitchDeg > 45.0f) thirdPitchDeg = 45.0f;
	if (thirdPitchDeg < -45.0f) thirdPitchDeg = -45.0f;
}

void Window::AdjustAerialRotation(float dx, float dy)
{
	aerialYawDeg += dx * mouseSensitivityDeg;
	aerialPitchDeg += dy * mouseSensitivityDeg;
	if (aerialPitchDeg > 89.0f) aerialPitchDeg = 89.0f;
	if (aerialPitchDeg < -89.0f) aerialPitchDeg = -89.0f;
	// recompute aerialPosition using spherical coordinates around aerialTarget
	float yawRad = glm::radians(aerialYawDeg);
	float pitchRad = glm::radians(aerialPitchDeg);
	glm::vec3 dir;
	dir.x = cosf(pitchRad) * sinf(yawRad);
	dir.y = sinf(pitchRad);
	dir.z = cosf(pitchRad) * cosf(yawRad);
	aerialPosition = aerialTarget - dir * aerialRadius;
}

void Window::ModifyAerialHeight(float delta)
{
	aerialPosition.y += delta;
	// update radius to keep correct spherical distance
	aerialRadius = glm::length(aerialPosition - aerialTarget);
}

	//metodos gett y sett de camaras


void Window::SetActiveCamera(int id)
{
	if (id < 1 || id > 4) return;
	activeCameraID = id;
}

int Window::GetActiveCameraID() const
{
	return activeCameraID;
}

void Window::SetThirdPersonDistance(float d)
{
	if (d < 0.0f) d = 0.0f;
	thirdPersonDistance = d;
}

void Window::SetThirdPersonHeight(float h)
{
	thirdPersonHeight = h;
}

float Window::GetThirdPersonDistance() const
{
	return thirdPersonDistance;
}

float Window::GetThirdPersonHeight() const
{
	return thirdPersonHeight;
}

float Window::GetThirdPersonYawDeg() const
{
	return thirdYawDeg;
}

float Window::GetThirdPersonPitchDeg() const
{
	return thirdPitchDeg;
}

void Window::SetAerialPosition(const glm::vec3& pos)
{
	aerialPosition = pos;
}

void Window::SetAerialTarget(const glm::vec3& target)
{
	aerialTarget = target;
}

glm::vec3 Window::GetAerialPosition() const
{
	return aerialPosition;
}

glm::vec3 Window::GetAerialTarget() const
{
	return aerialTarget;
}

//----------  Camaras fin -----------------

// --------- Animaciones complejas  -----------------

// -------------------- CINTURON: implementacion --------------------
void Window::SetCinturonActive(bool active)
{
	cinturonActive = active;
	if (cinturonActive)
	{
		// reiniciar secuencia de subida/flotacion
		cinturonAtTarget = false;
		cinturonTime = 0.0f;
		// si estaba por debajo, empezara a subir desde su valor actual
	}
	else
	{
		// al desactivar, dejaremos que UpdateCinturon lo baje suavemente
	}
}

bool Window::GetCinturonActive() const
{
	return cinturonActive;
}

float Window::GetCinturonY() const
{
	return cinturonY;
}

// Nuevos setters implementados
void Window::SetCinturonTargetY(float targetY)
{
	cinturonTargetY = targetY;
}

void Window::SetCinturonRiseSpeed(float speed)
{
	cinturonRiseSpeed = speed;
}

void Window::SetCinturonFloatAmplitude(float amplitude)
{
	cinturonFloatAmplitude = amplitude;
}

void Window::SetCinturonFloatFreq(float freq)
{
	cinturonFloatFreq = freq;
}

void Window::UpdateCinturon(GLfloat deltaTime)
{
	// Si no esta activo, devolvemos el cinturon hacia Y=0 suavemente
	if (!cinturonActive)
	{
		if (cinturonY > 0.0f)
		{
			cinturonY -= cinturonRiseSpeed * deltaTime;
			if (cinturonY <= 0.0f)
			{
				cinturonY = 0.0f;
				cinturonAtTarget = false;
				cinturonTime = 0.0f;
			}
		}
		return;
	}

	// Si esta activo:
	if (!cinturonAtTarget)
	{
		// sube hasta el target
		cinturonY += cinturonRiseSpeed * deltaTime;
		if (cinturonY >= cinturonTargetY)
		{
			cinturonY = cinturonTargetY;
			cinturonAtTarget = true;
			cinturonTime = 0.0f;
		}
	}
	else
	{
		// ya alcanzo la altura: flotacion sinusoidal
		cinturonTime += deltaTime;
		const float twoPi = 6.283185307179586f;
		float offset = sinf(twoPi * cinturonFloatFreq * cinturonTime) * cinturonFloatAmplitude;
		cinturonY = cinturonTargetY + offset;
	}
}
		// --------------------- Fin animacion cinturon  ----------------------

GLfloat Window::getarticulacion1()
{
	const float velocidad = 0.40f; // cycles per second
	const float amplitud = 60.0f; // degrees
	float t = static_cast<float>(glfwGetTime());
	float angle = std::sinf(t * velocidad * 3.14159265f * 2.0f) * amplitud;
	return angle;
}

void Window::UpdateMovement(GLfloat deltaTime)
{
	if (movementState == WALKING)
	{
		muevex += moveDir * walkSpeed * deltaTime;
		if (moveDir > 0 && muevex >= walkTarget)
		{
			muevex = walkTarget;
			movementState = ROTATING;
			rotationElapsed = 0.0f;
			rotationStart = rotationY;
			rotationTarget = rotationStart + 180.0f;
		}
		else if (moveDir < 0 && muevex <= 0.0f)
		{
			muevex = 0.0f;
			movementState = ROTATING;
			rotationElapsed = 0.0f;
			rotationStart = rotationY;
			rotationTarget = rotationStart + 180.0f;
		}
	}
	else if (movementState == ROTATING)
	{
		rotationElapsed += deltaTime;
		float t = rotationElapsed / rotationDuration;
		if (t >= 1.0f)
		{
			rotationY = std::fmod(rotationTarget, 360.0f);
			movementState = WALKING;
			moveDir = -moveDir;
		}
		else
		{
			rotationY = rotationStart + (rotationTarget - rotationStart) * t;
		}
	}
}

// Ring control API
void Window::SetRingLightCenter(const glm::vec3 &center)
{
	ring2Center = center;
	ring2Model = glm::mat4(1.0f);
	ring2Model = glm::translate(ring2Model, ring2Center);
	ring2Model = glm::scale(ring2Model, glm::vec3(1.5f, 1.5f, 1.5f));
}

void Window::SetRingLightOrbitSpeed(float degPerSec)
{
	ringLightOrbitSpeed = degPerSec;
}

void Window::SetRingLightSpinSpeed(float degPerSec)
{
	ringLightSpinSpeed = degPerSec;
}

void Window::SetRingLightRadius(float radius)
{
	ringLightRadius = radius;
}

void Window::SetRingLightTiltDeg(float deg)
{
	ringLightTiltDeg = deg;
}

void Window::SetRingLightLocalOffset(const glm::vec3 &offset)
{
	ringLightLocalOffset = offset;
}

void Window::UpdateRingLight(GLfloat deltaTime)
{
	// advance self spin only; do not change orbit angle if local offset set
	if (ringLightLocalOffset == glm::vec3(0.0f))
	{
		// orbit enabled
		ringLightAngle += ringLightOrbitSpeed * deltaTime;
		if (ringLightAngle >= 360.0f) ringLightAngle = fmod(ringLightAngle, 360.0f);
	}
	// advance self spin
	ringLightSpinAngle += ringLightSpinSpeed * deltaTime;
	if (ringLightSpinAngle >= 360.0f) ringLightSpinAngle = fmod(ringLightSpinAngle, 360.0f);
}

glm::vec3 Window::GetRingLightWorldPos() const
{
	if (ringLightLocalOffset != glm::vec3(0.0f))
	{
		return glm::vec3(ring2Model * glm::vec4(ringLightLocalOffset, 1.0f));
	}
	float rad = glm::radians(ringLightAngle);
	glm::vec3 local = glm::vec3(cos(rad) * ringLightRadius, 2.0f, sin(rad) * ringLightRadius);
	return glm::vec3(ring2Model * glm::vec4(local, 1.0f));
}

glm::vec3 Window::GetRingLightWorldDir() const
{
	// Direction pointing downwards with tilt and rotated by spin angle around local Y
	glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
	float tiltRad = glm::radians(ringLightTiltDeg);
	glm::mat4 tiltM = glm::rotate(glm::mat4(1.0f), tiltRad, glm::vec3(1.0f, 0.0f, 0.0f));
	float spinRad = glm::radians(ringLightSpinAngle);
	glm::mat4 spinM = glm::rotate(glm::mat4(1.0f), spinRad, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 dir = spinM * tiltM * glm::vec4(down, 0.0f);
	return glm::normalize(glm::vec3(dir));
}


// Implementaciones para cámaras personalizadas (5 cámaras)
int Window::GetCurrentCustomCameraIndex() const
{
    return customCamIndex;
}

glm::vec3 Window::GetCustomCameraPosition(int index) const
{
    if (index < 0 || index >= 5) return glm::vec3(0.0f);
    return customCamPos[index];
}

glm::vec3 Window::GetCustomCameraTarget(int index) const
{
    if (index < 0 || index >= 5) return glm::vec3(0.0f);
    return customCamTarget[index];
}

void Window::SetCustomCameraPosition(int index, const glm::vec3 &pos)
{
    if (index < 0 || index >= 5) return;
    customCamPos[index] = pos;
}

void Window::SetCustomCameraTarget(int index, const glm::vec3 &target)
{
    if (index < 0 || index >= 5) return;
    customCamTarget[index] = target;
}

void Window::CycleCustomCamera()
{
    customCamIndex = (customCamIndex + 1) % 5;
    activeCameraID = 4; // activar grupo custom
}

// Nota: La lógica para detectar la tecla 4 y avanzar el índice ya está en ManejaTeclado.

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
