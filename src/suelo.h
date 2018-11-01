#ifndef SUELO_H
#define SUELO_H

#include "addons.h"
using namespace std;

class Suelo : public Malla
{
    public:
        Suelo(char *filename,int n);

        // gets
        GLuint getVao();
        int getNumVertices();
        char* getfilename();
        btRigidBody* getRigidBody();

        //sets
        void setModelMatrix(glm::mat4 model);
        void draw(int matloc);
};

#endif