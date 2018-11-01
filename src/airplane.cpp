#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tools.h"
#include "airplane.h"
#include <assert.h>
using namespace std;

airplane::airplane(char* filename){
    this->filename = filename;
    this->pos = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,1,0);
    this->angle = 0.0f;
    this->life = 100.0f;
    this->posicion = btVector3(0,10,0);
    this->rotacion = btQuaternion(0.f,1.f,0.f,0.f);
    assert(Malla::load_mesh(filename,&vao,&numVertices));
}

float airplane::getLife(){
    return this->life;
}
int airplane::getBullets(){
    return this->bullets;
}

glm::vec3 airplane::getRotation(){
    return this->rotation;
}

void airplane::setRotation(float ang, glm::vec3 rot, int model){
        T = glm::rotate(glm::mat4(T),ang,rot);
        glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void airplane::setBullets(int bullet){
    this->bullets = bullet;
}

void airplane::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
}
