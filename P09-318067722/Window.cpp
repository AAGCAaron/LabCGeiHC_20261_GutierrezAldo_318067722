#include "Window.h"
#include <cstdio>

// --- Parámetros de animación de puertas ---
static const float RIGHT_MAX = 5.20f;   // recorrido corredera IZQ
static const float ANGLE_MAX = 60.0f;   // apertura DER en grados

// Velocidades separadas: dejamos la corredera (RIGHT) como ya te gusta
// y hacemos mucho más lenta solo la bisagra (ANGLE).
static const float RIGHT_SPEED = 0.1f ;               // unidades/seg (corredera)
static const float ANGLE_SLOW_FACTOR = 100.0f;         // cuanto mayor, más lenta la bisagra
static const float ANGLE_SPEED = 180.0f / ANGLE_SLOW_FACTOR;  // grados/seg (bisagra)
static const float HOLD_TIME = 50.5f;    // s abiertas antes de autocierre

static inline void moveTowards(float target, float speed, float dt, float& v) {
    if (v < target) { v += speed * dt; if (v > target) v = target; return; }
    if (v > target) { v -= speed * dt; if (v < target) v = target; }
}

Window::Window() {
    width = 800;
    height = 600;

    lastX = 0.0f; lastY = 0.0f;
    xChange = 0.0f; yChange = 0.0f;
    mouseFirstMoved = true;

    muevex = 2.0f;
    llantaMov = 0.0f;

    // Puertas
    puertaIzq = 0.0f; puertaDer = 0.0f;
    targetIzqOpen = false; targetDerOpen = false;
    izqHold = 0.0f; derHold = 0.0f;

    for (size_t i = 0; i < 1024; i++) keys[i] = false;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;

    lastX = 0.0f; lastY = 0.0f;
    xChange = 0.0f; yChange = 0.0f;
    mouseFirstMoved = true;

    muevex = 2.0f;
    llantaMov = 0.0f;

    // Puertas
    puertaIzq = 0.0f; puertaDer = 0.0f;
    targetIzqOpen = false; targetDerOpen = false;
    izqHold = 0.0f; derHold = 0.0f;

    for (size_t i = 0; i < 1024; i++) keys[i] = false;
}

int Window::Initialise() {
    if (!glfwInit()) {
        std::printf("Falló inicializar GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);
    if (!mainWindow) {
        std::printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    createCallbacks();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::printf("Falló inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, ManejaTeclado);
    glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange() {
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange() {
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_Y) {
        theWindow->muevex += 0.30f;
        theWindow->llantaMov -= 10.0f;
    }
    if (key == GLFW_KEY_U) {
        theWindow->muevex -= 0.30f;
        theWindow->llantaMov += 10.0f;
    }

    // Puertas: una pulsación para toggle animación completa
    if (key == GLFW_KEY_H && action == GLFW_PRESS) theWindow->TogglePuertaIzq(); // IZQ corredera
    if (key == GLFW_KEY_J && action == GLFW_PRESS) theWindow->TogglePuertaDer(); // DER bisagra
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        theWindow->TogglePuertaDer();
        theWindow->TogglePuertaIzq();
    }// Ambas

    // Matriz de teclas
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)   theWindow->keys[key] = true;
        else if (action == GLFW_RELEASE) theWindow->keys[key] = false;
    }
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = static_cast<GLfloat>(xPos);
        theWindow->lastY = static_cast<GLfloat>(yPos);
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = static_cast<GLfloat>(xPos) - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - static_cast<GLfloat>(yPos);

    theWindow->lastX = static_cast<GLfloat>(xPos);
    theWindow->lastY = static_cast<GLfloat>(yPos);
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

// --------- Puertas ---------

void Window::TogglePuertaIzq() {
    targetIzqOpen = !targetIzqOpen;
    izqHold = 0.0f; // cancelar hold si estaba activo
}

void Window::TogglePuertaDer() {
    targetDerOpen = !targetDerOpen;
    derHold = 0.0f;
}

void Window::StepPuertas(float dt) {
    // Izquierda: translate
    float prevIzq = puertaIzq;
    float tIzq = targetIzqOpen ? RIGHT_MAX : 0.0f;
    moveTowards(tIzq, RIGHT_SPEED, dt, puertaIzq);

    // Al terminar apertura, iniciar temporizador
    if (targetIzqOpen && puertaIzq == RIGHT_MAX && prevIzq != RIGHT_MAX) {
        izqHold = HOLD_TIME;
    }
    // Cuenta atrás y autocierre
    if (izqHold > 0.0f) {
        izqHold -= dt;
        if (izqHold <= 0.0f) targetIzqOpen = false;
    }

    // Derecha: rotate
    float prevDer = puertaDer;
    float tDer = targetDerOpen ? ANGLE_MAX : 0.0f;
    moveTowards(tDer, ANGLE_SPEED, dt, puertaDer);

    if (targetDerOpen && puertaDer == ANGLE_MAX && prevDer != ANGLE_MAX) {
        derHold = HOLD_TIME;
    }
    if (derHold > 0.0f) {
        derHold -= dt;
        if (derHold <= 0.0f) targetDerOpen = false;
    }
}
