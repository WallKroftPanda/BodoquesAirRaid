#include "bala.h"

using namespace std;

bala::bala(char* filename,glm::vec3 pos, glm::vec3 orientation){
    this->filename = filename;
    this->pos = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,1,0);
    this->velocity = 50;
    this->vectorVelocity = orientation * velocity;
    this->angle = 0.0f;
    assert(Malla::load_mesh(filename,&vao,&numVertices));
}

glm::vec3 bala::getRotation(){
    return this->rotation;
}

void bala::setRotation(float ang, glm::vec3 rot, int model){
        T = glm::rotate(glm::mat4(T),ang,rot);
        glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void bala::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->Malla::getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->Malla::getNumVertices());
}
