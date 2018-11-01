#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
//#include <iostream>
camera::camera(glm::vec3 cameraPos,glm::vec3 cameraFront,glm::vec3 cameraUp, int W, int H){
	this->cPos = cameraPos;
	this->cFront = cameraFront;
	this->cUp = cameraUp;
	this->gl_W = W;
	this->gl_H = H;
	this->lastX =  W / 2.0;
	this->lastY =  H / 2.0;
	this->firstMouse = true;
	this->sensitivity = 0.1f;
	this->yaw   = -90.0f;
	this->pitch =  0.0f;
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

void camera::setProjection(float fov){
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

void camera::setCameraPos(glm::vec3 pos)
{
	this->cPos.x = pos.x;
	this->cPos.y = pos.y;
	this->cPos.z = pos.z;
}

void camera::setCameraFront(glm::vec3 front){
	this->cFront = glm::normalize(front);
}

void camera::actualizar(double xpos, double ypos){
	if (this->firstMouse){
        this->lastX = xpos;
        this->lastY = ypos;
        this->firstMouse = false;
    }

    float xoffset = xpos - this->lastX;
    float yoffset = this->lastY - ypos; // reversed since y-coordinates go from bottom to top
    this->lastX = xpos;
    this->lastY = ypos;
    
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    //actualizar la camara con los nuevos datos:    
    this->cFront = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
    						sin(glm::radians(pitch)),
    						sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    /*Si se quiere habilitar em imprimir fron, se debe exportar <iostream>
    std::cout<<front.x<<std::endl;
    std::cout<<front.y<<std::endl;
    std::cout<<front.z<<std::endl;
    */
}


