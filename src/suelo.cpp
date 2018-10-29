#include "addons.h"

using namespace std;
suelo::suelo(char* filename){
    this->filename = filename;
    this->position = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,1,0);
    this->angle = 0.0f;
    assert(load_mesh(filename,&vao,&numVertices));
}

GLuint suelo::getVao(){
    return this->vao;
}

int suelo::getNumVertices(){
    return this->numVertices;
}

glm::vec3 suelo::getPosition(){
    return this->position;
}

char* suelo::getfilename(){
    return this->filename;
}

void suelo::setPosition(glm::vec3 pos,int model){
    T = glm::translate(glm::mat4(1.0f),pos);
    glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void suelo::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}

void suelo::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
}