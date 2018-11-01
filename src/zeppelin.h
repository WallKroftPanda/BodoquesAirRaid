#ifndef ZEPPELIN_H
#define ZEPPELIN_H
#include "malla.h"

using namespace std;
class zeppelin : public Malla{
    private:
        float life;
    public:
        zeppelin(char *filename);
        
        //getters
        GLuint getVao();
        int getNumVertices();
        float getLife();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        char* getFilename();
        
        //setters
        void setRotation(float ang,glm::vec3 rot,int model);
        void setModelMatrix(glm::mat4 model);

        void draw(int matloc);
        void explotar();
        
};
#endif
