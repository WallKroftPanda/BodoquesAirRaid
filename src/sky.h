#ifndef SKY_H
#define SKY_H

#include "malla.h"

using namespace std;
class sky/*: public malla*/{
    private:
        GLuint vao,vbo;
        int numVertices;
        char* filename;
        glm::vec3 position;
        glm::vec3 front;
        float angle;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 T;
        glm::mat4 modelMatrix;
    public:
        sky(char* filename);
        //getters
        GLuint getVao();
        int getNumVertices();
        glm::vec3 getFront();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        char* getFilename();

        //setters
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setPosition(glm::vec3 pos, int model);
        void setRotation(float ang,glm::vec3 rot,int model);
        void setFileName(char *f);
        void setModelMatrix(glm::mat4 model);

        void draw(int matloc);

        
};
#endif
