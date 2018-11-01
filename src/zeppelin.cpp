#include "zeppelin.h"
using namespace std;

zeppelin::zeppelin(char* filename){
	this->filename = filename;
	this->pos = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,1,0);
    this->angle = 0.0f;
    this->life = 300.0f;
    this->posicion = btVector3(5,10,5);
    this->rotacion = btQuaternion(1.57f,0,1,0);
    assert(Malla::load_mesh(filename,&vao,&numVertices));
}

float zeppelin::getLife(){
    return this->life;
}

glm::vec3 zeppelin::getRotation(){
    return this->rotation;
}


void zeppelin::setRotation(float ang, glm::vec3 rot, int model){
        T = glm::rotate(glm::mat4(T),ang,rot);
        glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void zeppelin::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}

void zeppelin::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->Malla::getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->Malla::getNumVertices());
}
