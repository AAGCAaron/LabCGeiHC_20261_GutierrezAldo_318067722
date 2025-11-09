#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
#include <glm.hpp>
#include <vector>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();

	//metodos para obtener el movimiento de las luces
	GLfloat getmuevex() { return muevex; }
	GLfloat getarticulacion1();

	//Antorcha
	bool getAntorchaEncendida() { return antorchaEncendida; }
	bool antorchaEncendida;

	//CAMINATA PATO LUCAS
	void UpdateMovement(GLfloat deltaTime);
	GLfloat getRotationY() { return rotationY; }

	// Taz / Tornado control
	void SetTazActive(bool v) { tazActive = v; }
	bool GetTazActive() { return tazActive; }

	// Ring 2 spotlight control (animation state updated in Window)
	void UpdateRingLight(GLfloat deltaTime);
	glm::vec3 GetRingLightWorldPos() const;
	glm::vec3 GetRingLightWorldDir() const;

	// setters para controlar posicion/velocidades/angulo sin romper jerarquia
	void SetRingLightCenter(const glm::vec3 &center);
	void SetRingLightOrbitSpeed(float degPerSec);
	void SetRingLightSpinSpeed(float degPerSec);
	void SetRingLightRadius(float radius);
	void SetRingLightTiltDeg(float deg);
	// nuevo: establecer offset local fijo para que la luz no se mueva en orbita
	void SetRingLightLocalOffset(const glm::vec3 &offset);

	//------------------------------Hasta aqui------------------------------

	// CINTURON (animacion)
	void SetCinturonActive(bool active);
	bool GetCinturonActive() const;
	void UpdateCinturon(GLfloat deltaTime);
	float GetCinturonY() const;
	// setters para controlar velocidad, altura y flotacion
	void SetCinturonRiseSpeed(float v);
	void SetCinturonTargetY(float y);
	void SetCinturonFloatAmplitude(float a);
	void SetCinturonFloatFreq(float f);

	// Active camera control (1=free,2=third-person,3=aerial,4=custom)
	void SetActiveCamera(int id);
	int GetActiveCameraID() const;

	// API para cámaras personalizadas (5 cámaras indexadas 0..4)
	void SetCustomCameraPosition(int index, const glm::vec3 &pos);
	void SetCustomCameraTarget(int index, const glm::vec3 &target);
	glm::vec3 GetCustomCameraPosition(int index) const;
	glm::vec3 GetCustomCameraTarget(int index) const;
	void CycleCustomCamera();
	int GetCurrentCustomCameraIndex() const;

	// Third-person camera parameters
	void SetThirdPersonDistance(float d);
	void SetThirdPersonHeight(float h);
	float GetThirdPersonDistance() const;
	float GetThirdPersonHeight() const;
	// rotate third-person with mouse
	void AdjustThirdPersonRotation(float dx, float dy);
	float GetThirdPersonYawDeg() const;
	float GetThirdPersonPitchDeg() const;

	// Aerial camera parameters
	void SetAerialPosition(const glm::vec3 &pos);
	void SetAerialTarget(const glm::vec3 &target);
	glm::vec3 GetAerialPosition() const;
	glm::vec3 GetAerialTarget() const;
	// rotate aerial with mouse
	void AdjustAerialRotation(float dx, float dy);
	// change aerial height (keys 9/0)
	void ModifyAerialHeight(float delta);

	GLFWwindow* getWindow() { return mainWindow; }

	//Animaciones basicas
	GLfloat getTapaBasuraRotX() { return tapaBasuraRotX; }
	GLfloat getLataPosY() { return lataPosY; }
	GLfloat getMartilloRotX() { return martilloRotX; } // getter para martillo
	void ActualizaAnimaciones(GLfloat deltaTime); // actualizar animaciones por frame


	///----------------------------------------------------------
	double getTime() { return glfwGetTime(); }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);} 
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();

	// ----------------- Keyframe animation API (public) -----------------
	// Save/load keyframes for the single animatable model (Animacion1_M).
	void SaveKeyframesToFile(const char* filename);
	void LoadKeyframesFromFile(const char* filename);
	void AppendCurrentKeyframe();
	glm::vec3 GetAnimModelPos() const { return animModelPos; }
	float GetAnimModelRotY() const { return animModelRotY; }
	float GetAnimModelRotZ() const { return animModelRotZ; } // getter para rot Z

	// Muestra en consola las opciones del menú de keyframes (en español)
	void PrintKeyframeMenu() const;

private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	// ------------------------------------- Animaciones basicas
	GLfloat tapaBasuraRotX, lataPosY;
	// control de animacion de la tapa 
	bool tapaAnimando;
	bool tapaObjetivoAbierta;
	// control de animacion de la lata 
	bool lataAnimando;
	GLfloat lataObjetivoY;
	// animacion del martillo de la campana
	GLfloat martilloRotX; // Angulo actual en grados
	bool martilloAnimando; // si esta animando
	GLfloat martilloDireccion; // direccion de movimiento (-1 o 1)
	
	///----------------------------------------------------------
	GLfloat muevex;
	//----------------------------------------------------------
	GLboolean mouseFirstMoved;
	// CAMINATA PATO LUCAS
	enum MovementState { WALKING = 0, ROTATING = 1 };
	MovementState movementState;
	int moveDir; // +1 forward, -1 backward
	GLfloat walkTarget;
	GLfloat walkSpeed; // units per second
	GLfloat rotationY; // current Y rotation in degrees
	GLfloat rotationDuration; // seconds to rotate 180 degrees
	GLfloat rotationElapsed;
	GLfloat rotationStart;
	GLfloat rotationTarget;
	//---------------------------- Hasta Aqui ------------------------------

	// Taz / Tornado
	bool tazActive;

	// Ring 2 spotlight state (hierarchy-managed)
	float ringLightAngle; // orbit angle deg (kept but orbit can be disabled)
	float ringLightOrbitSpeed; // deg/s
	float ringLightSpinAngle; // self spin deg
	float ringLightSpinSpeed; // deg/s
	float ringLightRadius; // orbit radius
	float ringLightTiltDeg; // tilt of light cone from vertical
	glm::vec3 ring2Center;
	glm::mat4 ring2Model; // precomputed model for ring transform
	glm::vec3 ringLightLocalOffset; // local offset; use to make light stationary

	// Cinturon (animacion)
	bool cinturonActive;
	float cinturonY; // desplazamiento local Y actual
	float cinturonTargetY; // objetivo (0.5f)
	float cinturonRiseSpeed; // velocidad al subir
	bool cinturonAtTarget; // si ya alcanzo target y ahora flota
	float cinturonFloatAmplitude; // amplitud del sinusoidal
	float cinturonFloatFreq; // frecuencia en Hz del sinusoidal
	float cinturonTime; // tiempo desde que alcanzo target

	// camera selection
	int activeCameraID;

	// custom cameras (indexes 0..4) and cycling state
	glm::vec3 customCamPos[5];
	glm::vec3 customCamTarget[5];
	int customCamIndex; // 0..4, when activeCameraID == 4 means custom group active

	// third-person / aerial parameters
	float thirdPersonDistance;
	float thirdPersonHeight;
	float thirdYawDeg;
	float thirdPitchDeg;
	float mouseSensitivityDeg;

	glm::vec3 aerialPosition;
	glm::vec3 aerialTarget;
	float aerialYawDeg;
	float aerialPitchDeg;
	float aerialRadius;

	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	struct Keyframe { glm::vec3 pos; float rotY; };
	std::vector<Keyframe> keyframes;
	bool canSaveKeyframe; // P toggles this
	bool keyframesLocked; // after saving with C, lock until K
	bool playbackActive; // space toggles playback
	bool playbackOnce; // reproducir una sola vez cuando se invoque
	float playbackTimer; // local timer within current segment
	int playbackIndex; // current keyframe index (start of segment)
	float segmentDuration; // seconds per segment when playing
	glm::vec3 animModelPos; // current transform for the animated model
	float animModelRotY;
	float animModelRotZ; // rotacion Z del modelo animado (controlada por Q/E)
	
};

