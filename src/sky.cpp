#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tools.h"
#include "sky.h"
#include <assert.h>
using namespace std;

sky::sky(char* filename){
    this->filename = filename;
    this->position = glm::vec3(0,0,0);
    this->angle = 0.0f;
    assert(load_mesh(filename,&vao,&numVertices));
}

GLuint sky::getVao(){
    return this->vao;
}
int sky::getNumVertices(){
    return this->numVertices;
}
glm::vec3 sky::getFront(){
	return this->front;
}
glm::vec3 sky::getPosition(){
    return this->position;
}
glm::vec3 sky::getRotation(){
    return this->rotation;
}
char* sky::getFilename(){
    return this->filename;
}
void sky::setPosition(glm::vec3 pos,int model){
    T = glm::translate(glm::mat4(1.0f),pos);
    glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void sky::setRotation(float ang, glm::vec3 rot, int model){
        T = glm::rotate(glm::mat4(T),ang,rot);
        glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void sky::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}

void sky::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
}
