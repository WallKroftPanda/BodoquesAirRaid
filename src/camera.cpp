#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
camera::camera(glm::vec3 cameraPos,glm::vec3 cameraFront,glm::vec3 cameraUp){
	this->cPos = cameraPos;
	this->cFront = cameraFront;
	this->cUp = cameraUp;
}
//GETTERS

glm::vec3 camera::getCameraPos(){
	return this->cPos;
}
glm::vec3 camera::getCameraFront(){
	return this->cFront;
}
glm::vec3 camera::getCameraUp(){
	return this->cUp;
}
int camera::getViewMatLocation(){
	return this->view_mat_location;
}
int camera::getProjMatLocation(){
	return this->proj_mat_location;
}


//SETTERS
		
void camera::setView(){
	this->view = glm::lookAt(cPos, cPos + cFront, cUp);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera::setProjection(float fov, int gl_W, int gl_H){
	this->projection = glm::perspective(glm::radians(fov), (float)gl_W / (float)gl_H, 0.1f, 100.0f);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void camera::setViewMatLocation(GLuint shader_programme){
	this->view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv(this->view_mat_location, 1, GL_FALSE, &view[0][0]);	
}

void camera::setProjMatLocation(GLuint shader_programme){
	this->proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (this->proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void camera::setCameraPos(int dir,float cameraSpeed){
	switch (dir){
		case 8:
			this->cPos += cameraSpeed*this->cFront;
			break;
		case 2:
			this->cPos -= cameraSpeed*this->cFront;
			break;
		case 4:
			this->cPos -= glm::normalize(glm::cross(this->cFront,this->cUp)) * cameraSpeed;
			break;
		case 6:
			this->cPos += glm::normalize(glm::cross(this->cFront, this->cUp)) * cameraSpeed;
			break;
	}
}

void camera::setCameraFront(glm::vec3 front){
	this->cFront = glm::normalize(front);
}


