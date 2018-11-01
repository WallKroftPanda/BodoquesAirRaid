#ifndef BALA_H
#define BALA_H
#include "malla.h"

using namespace std;

class bala : public Malla{
    private:
        glm::vec3 vectorVelocity;
        float velocity;
    public:
        bala(char* filename, glm::vec3 pos, glm::vec3 orientation);
        //getters
        glm::vec3 getRotation();
        glm::vec3 getVectorVelocity();

        //setters
        void setRotation(float ang,glm::vec3 rot,int model);
        void setVectorVelocity(float velocity, glm::vec3 dir);

        void draw(int matloc);

        //Implementar
        void vuelo(glm::vec3 pos, float velocity);
        void colition();
};
#endif
