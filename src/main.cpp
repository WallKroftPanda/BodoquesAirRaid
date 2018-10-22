#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "gl_utils.h"
#include "tools.h"
#include "airplane.h"
#include "zeppelin.h"
#include "malla.h"
#include "camera.h"
//Definiciones

#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "shaders/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/test_fs.glsl"

//VARIABLES GLOBALES

int g_gl_width = 1280;
int g_gl_height = 720;
GLFWwindow* g_window = NULL;
GLuint shader_programme;
//Posiciones de camara
camera *camara;
/*glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void Init();

bool firstMouse = true;
float yaw   = -90.0f;
float deltaTime;
float timef;
float lastFrame;
float pitch =  0.0f;
float lastX =  g_gl_width / 2.0;
float lastY =  g_gl_height / 2.0;
float fov   =  45.0f;
airplane* avion;

int model_mat_location;
airplane *bodoque;
zeppelin *e1;
malla *suelo;
malla *ElMono;

int main()
{
    Init();
    
    camara= new camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    camara->setProjection(fov, g_gl_width, g_gl_height);
    
    camara->setView();

	camara->setViewMatLocation(shader_programme);
	
	camara->setProjMatLocation(shader_programme);
    int model_mat_location = glGetUniformLocation(shader_programme, "model");
	glm::vec3 cam;
	
    while (!glfwWindowShouldClose(g_window)){

        //time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timef+=deltaTime;

        processInput(g_window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam = camara->getCameraPos();
        cout<<cam.x<<","<<cam.y<<","<<cam.z<<endl;

        // activate shader
		glUseProgram (shader_programme);

	    camara->setProjection(fov, g_gl_width, g_gl_height);

        camara->setView();
        //Dibujar suelo
        suelo->setpos(glm::vec3(0.0f,-1.0f,0.0f),model_mat_location);
        glBindVertexArray(suelo->getvao());
        glDrawArrays(GL_TRIANGLES,0,suelo->getnumvertices());
        //Dibujar avión
        bodoque->setPosition(glm::vec3(cam.x,cam.y-1,cam.z-10), model_mat_location);
        bodoque->setRotation(1.55f, glm::vec3(0,1,0),model_mat_location);
        glBindVertexArray(bodoque->getVao());
        glDrawArrays(GL_TRIANGLES,0,bodoque->getNumVertices());
        //Dibujar zeppelin
        e1->setPosition(glm::vec3(-21.0f,10.0f,-50.0f), model_mat_location);
        glBindVertexArray(e1->getVao());
        glDrawArrays(GL_TRIANGLES,0,e1->getNumVertices());

        //Dibujar ElMono
        ElMono->setpos(glm::vec3(-30.0f,2.0f,5.0f), model_mat_location);
        glBindVertexArray(ElMono->getvao());
        glDrawArrays(GL_TRIANGLES,0,ElMono->getnumvertices());

		/*e1->setPosition(glm::vec3(4.0f,-1.0f,0.0f));
        glBindVertexArray(e1->getVao());
        e1->draw(model_mat_location);
*/        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void Init(){
	//Funciones contenidas en gl_utils.cpp
	restart_gl_log ();
	start_gl();
	//Funciones propias de GL
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glClearColor (0.2, 0.2, 0.2, 1.0);
	glViewport (0, 0, g_gl_width, g_gl_height);
	
	glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
	
	//ESTO SOLO DEBE INCLUIRSE POR AHORA, AL MOMENTO DE CONSTRUIR EL JUEGO, YA QUE NUESTRO JUEGO NO OCUPA MOUSE
	glfwSetCursorPosCallback(g_window, mouse_callback);
	glfwSetScrollCallback(g_window, scroll_callback);
    
	// Le decimos a GLFW que capture nuestro mouse
	glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
		/*-------------------------------Creamos Shaders-------------------------------*/
	shader_programme = create_programme_from_files (
	VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    model_mat_location=  glGetUniformLocation (shader_programme, "model");
    
    bodoque = new airplane((char*)"mallas/Hurricane.obj");
    e1 = new zeppelin((char*)"mallas/dirigible.obj");
   	suelo = new malla((char*)"mallas/sueloRef.obj");
    ElMono = new malla((char*)"mallas/suzanne.obj");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 25 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camara->setCameraPos(8,cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camara->setCameraPos(2,cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camara->setCameraPos(4,cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camara->setCameraPos(6,cameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camara->setCameraFront(front);
    std::cout<<front.x<<std::endl;
    std::cout<<front.y<<std::endl;
    std::cout<<front.z<<std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
       
}

void Logica(){
    //  DISPARAR
    //  MOVERSE
    //  COLISIONAR
    //  OBSTACULOS
    //  MAPA
}