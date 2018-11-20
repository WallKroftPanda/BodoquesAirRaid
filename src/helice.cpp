#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tools.h"
#include "helice.h"

using namespace std;
helice::helice(char* filename){
    this->filename = filename;
    this->pos = glm::vec3(0, 0, 0);
    this->rotation = glm::vec3(0, 1, 0);
    this->angle = 0.0f;
	assert(load_mesh(filename, &vao, &numvertices));
}

GLuint helice::getvao(){
    return this->vao;
}

int helice::getnumvertices(){
    return this->numvertices;
}

glm::vec3 helice::getpos(){
    return this->pos;
}

glm::vec3 helice::getRotation(){
    return this->rotation;
}

char* helice::getfilename(){
    return this->filename;
}

void helice::setpos(glm::vec3 p){
        this->pos = p;
}

void helice::setRotation(float ang, glm::vec3 rot){
        this->rotation = rot;
        this->angle = angle + ang;
        this->modelMatrix = glm::rotate(modelMatrix,angle,rot);	
}

void helice::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}

void helice::rotate(){
	this->modelMatrix = glm::rotate(modelMatrix,1.0f,glm::vec3(1,0,0));	
}

void helice::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getvao());
        glDrawArrays(GL_TRIANGLES, 0, this->getnumvertices());
}
