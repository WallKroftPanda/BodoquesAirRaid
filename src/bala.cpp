#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tools.h"
#include "bala.h"
#include <assert.h>

using namespace std;

bala::bala(char* filename,glm::vec3 pos, glm::vec3 orientation){
    this->filename = filename;
    this->position = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,1,0);
    this->velocity = 50;
    this->vectorVelocity = orientation * velocity;
    this->angle = 0.0f;
    assert(load_mesh(filename,&vao,&numVertices));
}

GLuint bala::getVao(){
    return this->vao;
}
int bala::getNumVertices(){
    return this->numVertices;
}
glm::vec3 bala::getPosition(){
    return this->position;
}
glm::vec3 bala::getRotation(){
    return this->rotation;
}
char* bala::getFilename(){
    return this->filename;
}
void bala::setPosition(glm::vec3 pos,int model){
    T = glm::translate(glm::mat4(1.0f),pos);
    glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void bala::setRotation(float ang, glm::vec3 rot, int model){
        T = glm::rotate(glm::mat4(T),ang,rot);
        glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void bala::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}

void bala::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
}
