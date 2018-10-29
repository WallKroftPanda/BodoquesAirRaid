#ifndef _CAMERA_H
#define _CAMERA_H
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera{
	private:
		//matrices de transformación y pocisión para la camara
		glm::vec3 cPos;
		glm::vec3 cFront;
		glm::vec3 cUp;
		glm::mat4 projection;
		glm::mat4 view;
		int view_mat_location;
		int proj_mat_location;
		int gl_W;
		int gl_H;
		//Variables necesarias para actualizar la camara, y vista de objetos
		bool firstMouse;
		float sensitivity;
		float yaw;
		float pitch;
		float lastX;
		float lastY;
	public:
		
		//Constructor
		camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp,int W,int H);
		//getters
		glm::vec3 getCameraPos();
		glm::vec3 getCameraFront();
		glm::vec3 getCameraUp();
		int getViewMatLocation();
		int getProjMatLocation();
		
		//Setters
		void setCameraPos(int dir,float cameraSpeed);
		void setCameraFront(glm::vec3 front);
		void setCameraUp();
		void setView();
		void setCameraPos();
		void setProjection(float fov);
		void setViewMatLocation(GLuint shader_programme);
		void setProjMatLocation(GLuint shader_programme);
		void actualizar(double xpos, double ypos);
};

#endif
