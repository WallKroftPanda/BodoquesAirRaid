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
#include "malla.h"

using namespace std;
malla::malla(char* filename){
    this->filename = filename;
    this->pos = glm::vec3(0, 0, 0);
    this->rotation = glm::vec3(0, 1, 0);
    this->angle = 0.0f;
	assert(load_mesh(filename, &vao, &numvertices));
}

GLuint malla::getvao(){
    return this->vao;
}

int malla::getnumvertices(){
    return this->numvertices;
}

glm::vec3 malla::getpos(){
    return this->pos;
}

glm::vec3 malla::getRotation(){
    return this->rotation;
}

char* malla::getfilename(){
    return this->filename;
}

void malla::setpos(glm::vec3 p){
        this->pos = p;
}

void malla::setRotation(float ang, glm::vec3 rot){
        this->rotation = rot;
        this->angle = ang;
}

void malla::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}

void malla::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getvao());
        glDrawArrays(GL_TRIANGLES, 0, this->getnumvertices());
}
