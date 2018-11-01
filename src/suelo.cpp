#include "suelo.h"
using namespace std;
Suelo::Suelo(char* filename,int n){
    this->filename = filename;
    this->pos = glm::vec3(0,0,0);
    this->rotation = glm::vec3(0,1,0);
    this->masa = 0.0f;
    this->angle = 0.0f;
    this->scale=btVector3(100,1,100);
    this->rotacion=btQuaternion(0,0,0,0);
    assert(Malla::load_mesh(filename,&vao,&numVertices));
}

void Suelo::draw(int matloc){
	    glUniformMatrix4fv(matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->Malla::getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->Malla::getNumVertices());
}