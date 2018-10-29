#ifndef SUELO_H
#define SUELO_H

#include "addons.h"
using namespace std;

class suelo{
    private:
        GLuint vao, vbo;
        int numVertices;
        glm::vec3 p;
        char* filename;
        const float mass = 0.0f;
        glm::vec3 position;
        float angle;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 T;
        glm::mat4 modelMatrix;

    public:
        suelo(char *filename);

        // gets
        GLuint getVao();
        int getNumVertices();
        glm::vec3 getPosition();
        char* getfilename();

        //sets
        
        void setvao(GLuint vao);
        void setnumvertices(int num);
        void setPosition(glm::vec3 p,int model);
        void setfilename(char *f);
        void setModelMatrix(glm::mat4 model);
        void draw(int matloc);
};

#endif