#include <stdio.h>
#include <stdlib.h> //random, srand
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <time.h>  

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader; //variables de tipos de datos de GL, la u es de unsigned, sin signo 


float red = 0.0f, green = 0.1f, blue = 0.3f;              // 0=Rojo, 1=Verde, 2=Azul
clock_t lastTick = 0;									// tiempo del último cambio de color
const double intervalo = 3.0;						  // segundos por color



//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

/*vec3 recibe x, y, z/*/ 
/*El gl position, recibe 3 vectores y convierte a pixeles /*/

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,1.0f,1.0f);	 			\n\
}";

//para cambiar el color en vec4 esta en rojo, es por eso que se ve el triangulo rojo


/* Nuestro Hola mundo */
void CrearTriangulo()
{
	//arreglo dinamico, le vamos a pasar vertices
	// Un vertice no solo es xyz, se puede tener datos de color, datos de normales, y texturizado 
		GLfloat vertices[] = {
				// ====== Letra A ========
				// Triangulo grande izquierdo
				-0.6f, 1.0f, 0.0f,
				-0.9f, 0.2f, 0.0f,
				-0.6f, 0.2f, 0.0f,

				// Triangulo grande derecha
				-0.4f, 1.0f, 0.0f,
				-0.4f, 0.2f, 0.0f,
				-0.1f, 0.2f, 0.0f,

				// Triangulo medio superior derecha 
				-0.4f, 1.0f, 0.0f,
				-0.5f, 0.8f, 0.0f,
				-0.4f, 0.6f, 0.0f,

				// Triangulo medio superior izq
				-0.6f, 1.0f, 0.0f,
				-0.6f, 0.6f, 0.0f,
				-0.4f, 1.0f, 0.0f,

				// Triángulo medio inferior izq
				-0.6f, 0.6f, 0.0f,
				-0.6f, 0.5f, 0.0f,
				-0.4f, 0.5f, 0.0f,

				// Triángulo medio inferior der
				-0.4f, 0.6f, 0.0f,
				-0.6f, 0.6f, 0.0f,
				-0.4f, 0.5f, 0.0f,
			
				// ====== Letra G ========

				//Triangulo arriba der
				0.8f, 1.0f, 0.0f,
				0.4f, 1.0f, 0.0f, 
				0.8f, 0.8f, 0.0f, 


				//arriba izq
				0.4f, 1.0f, 0.0f,  
				0.2f, 0.8f, 0.0f,
				0.8f, 0.8f, 0.0f,

				//medio izq
				0.4f, 0.8f, 0.0f,  
				0.2f, 0.8f, 0.0f,
				0.2f, 0.4f, 0.0f,


				//medio der
				0.4f, 0.8f, 0.0f,
				0.2f, 0.4f, 0.0f,
				0.4f, 0.2f, 0.0f,

				//abajo izq
				0.4f, 0.4f, 0.0f,
				0.4f, 0.2f, 0.0f,
				0.8f, 0.2f, 0.0f,

				//abajo der
				0.8f, 0.4f, 0.0f,
				0.4f, 0.4f, 0.0f,
				0.8f, 0.2f, 0.0f,

				//boca der
				0.8f, 0.6f, 0.0f,
				0.7f, 0.4f, 0.0f,
				0.8f, 0.4f, 0.0f,

				//boca izq
				0.7f, 0.6f, 0.0f,
				0.7f, 0.4f, 0.0f,
				0.8f, 0.6f, 0.0f,

				//diente arriba
				0.5f, 0.6f, 0.0f,
				0.7f, 0.5f, 0.0f,
				0.7f, 0.6f, 0.0f,

				//diente abajo
				0.5f, 0.6f, 0.0f,
				0.55f, 0.5f, 0.0f,
				0.7f, 0.5f, 0.0f,


				//curva medio abajo	
				0.4f, 0.5f, 0.0f,
				0.4f, 0.4f, 0.0f,
				0.5f, 0.4f, 0.0f,

				//curva medio arriba
				0.4f, 0.8f, 0.0f,
				0.4f, 0.7f, 0.0f,
				0.5f, 0.8f, 0.0f,


				// ====== Letra C ========

			//arriba der

				-0.2f, -0.2f, 0.0f,
				-0.6f, -0.2f, 0.0f,
				-0.2f, -0.4f, 0.0f,

			//arriba izq
				-0.6f, -0.2f, 0.0f,
				-0.6f, -0.4f, 0.0f,
				-0.2f, -0.4f, 0.0f,


			//Medio izq
				-0.8f, -0.4f, 0.0f,
				-0.8f, -0.8f, 0.0f,
				-0.6f, -0.8f, 0.0f,

			//medio der
				-0.6f, -0.2f, 0.0f,
				-0.8f, -0.4f, 0.0f,
				-0.6f, -0.8f, 0.0f,


			//abajo izq
				-0.8f, -0.8f, 0.0f,
				-0.6f, -1.0f, 0.0f,
				-0.2f, -0.8f, 0.0f,


			//abajo der
				-0.2f, -0.8f, 0.0f,
				-0.6f, -1.0f, 0.0f,
				-0.2f, -1.0f, 0.0f,

			//curva arriba
				-0.6f, -0.4f, 0.0f,
				-0.6f, -0.5f, 0.0f,
				-0.5f, -0.4f, 0.0f,

			//curva abajo
				-0.6f, -0.7f, 0.0f,
				-0.6f, -0.8f, 0.0f,
				-0.5f, -0.8f, 0.0f,


	};
	//siempre unir a los vertices en sentido antihorario
	//arriba dibujamos 3 puntos (-1,-1,0) abajo izq,(xyz)abajo der,(xyz)arriba en medio
	//la ventana siempre va a ser de dos unidades por dos unidades de -1 a 1 horizontal y vertical

	// Para generar un VAO necesitamos un VBO /
		glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
	//Me sirve para organizar, los datos en conjuntos de 3 en 3 
	//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//GLvoid* Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment, solo estos dos para esta materia
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se compila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

// solo se le van a agregar para futuras practicas tanto en addShader como en CompileShaders
void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	//envia informacion a a
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}//validacion del shader y si se pudo linkear
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}

//para usar una funcion en main declarar antes
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica01", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();

	//Para poder cambiar colores de manera random, iniciamos la semilla
	srand((unsigned)time(NULL));   // semilla distinta en cada ejecución
	lastTick = clock();            // arranca el cronómetro ahora


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		


		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		


		glUseProgram(shader); //una vez que comprobe, valide. Lo voy a usar

		glBindVertexArray(VAO);
		//(FORMA (points, triangles, Lines,))
		glDrawArrays(GL_TRIANGLES, 0, 78); //son los puntos que el glDrawArray me regreso
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

		
		clock_t now = clock();   // tomar el tiempo actual en ticks

		double elapsed = (double)(now - lastTick) / (double)CLOCKS_PER_SEC; // calcular segundos transcurridos
		if (elapsed >= intervalo) {   // Verifica el tiempo y si ya paso dichos segundos(intervalo)
			red = (float)rand() / (float)RAND_MAX;   // generar valor aleatorio entre 0 y 1
			green = (float)rand() / (float)RAND_MAX;   
			blue = (float)rand() / (float)RAND_MAX;   
			lastTick = now;   // reiniciar el cronómetro
		}





	}


	return 0;
}