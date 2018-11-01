#ifndef AIRPLANE_H
#define AIRPLANE_H
#include "addons.h"
#include "malla.h"

using namespace std;
class airplane: public Malla{
    private:
        float life;
        int bullets;
    public:
        airplane(char* filename);
        //getters
        int getBullets();
        float getLife();
        glm::vec3 getRotation();

        //setters
        void setVao(GLuint vao);
        void setBullets(int bullet);
        void setRotation(float ang,glm::vec3 rot,int model);

        void draw(int matloc);
        
        //Funcionalidades del avión deben ser implementadas más adelante junto con bullet... 
        void explotar();
        void girar_IZQ(); //movimiento de palas para girar a la izquierda 
        void girar_DER();//movimiento de palas para girar a la derecha
        void inc_VEL(); //Incrementar velocidad
        void dis_VEL(); //Reducir velocidad
        
};
#endif
