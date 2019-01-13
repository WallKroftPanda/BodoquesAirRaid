#ifndef MALLA_H
#define MALLA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <bullet/btBulletDynamicsCommon.h>
using namespace std;
class malla/*: public malla*/{
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
        btRigidBody* mBody;

    protected:
        GLuint shader_programme;
        GLuint tex;
        GLuint stex;
        GLuint matloc;
    public:
        malla(char* filename,GLuint);
        void setPos(glm::vec3 p);
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
        btRigidBody* getMBody();
        

        //setters
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setBullets(int bullet);
        void setPosition(glm::vec3 pos, int model);
        void setRotation(float ang,glm::vec3 rot,int model);
        void setFileName(char *f);
        void setModelMatrix(glm::mat4 model);
        void setMBody(btRigidBody* mBody);
    
        void draw(int matloc);
        bool load_specular(const char *filename);
        bool load_surface(const char *filename);
};
#endif
