#ifndef AIRPLANE_H
#define AIRPLANE_H
#include "addons.h"
#include "malla.h"

using namespace std;
class airplane/*: public malla*/{
    private:
        GLuint vao,vbo;
        int numVertices;
        float life;
        int bullets;
        char* filename;
        glm::vec3 position;
        glm::vec3 front;
        float angle;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 T;
        glm::mat4 modelMatrix;
        btRigidBody* aBody;
        
    protected:
        GLuint shader_programme;
        GLuint tex;
        GLuint stex;
        GLuint matloc;
    public:
        airplane(char* filename,GLuint);
        //getters
        bool load_texture (const char* file_name, GLuint *tex, GLenum texslot);
        GLuint getVao();
        int getNumVertices();
        int getBullets();
        glm::vec3 getFront();
        float getLife();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        char* getFilename();
        btRigidBody* getABody();
        

        //setters
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setBullets(int bullet);
        void setPosition(glm::vec3 pos, int model);
        void setRotation(float ang,glm::vec3 rot,int model);
        void setFileName(char *f);
        void setModelMatrix(glm::mat4 model);

        void draw(int matloc);
        
        /*Funcionalidades del avión deben ser implementadas más adelante junto con bullet... 
        void explotar();
        void girar_IZQ(); //movimiento de palas para girar a la izquierda 
        void girar_DER();//movimiento de palas para girar a la derecha
        void inc_VEL(); //Incrementar velocidad
        void dis_VEL(); //Reducir velocidad*/
        void setABody(btRigidBody* aBody);
        bool load_specular(const char *filename);
        bool load_surface(const char *filename);
};
#endif
