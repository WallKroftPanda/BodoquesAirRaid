#ifndef MALLA_H
#define MALLA_H
#include "addons.h"
using namespace std;

class Malla{
    protected:
        GLuint vao, vbo;
        int numVertices;
        glm::vec3 pos;
        glm::vec3 rotation;
        char* filename;
        btVector3 posicion;
        btQuaternion rotacion;

        btCollisionShape* colShape;
        btRigidBody* rigidBody;
        btTransform trfBody;

        float masa;
        float angle;

        btVector3 scale;
        glm::mat4 T;
        glm::mat4 modelMatrix;

    public:
        // gets
        GLuint getVao();
        int getNumVertices();
        glm::vec3 getPosition();
        char* getFilename();
        btRigidBody* getRigidBody();

        // sets
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setPos(glm::vec3 p,int model);
        void setFilename(char *f);
        void setModelMatrix(glm::mat4 model);
        void setRigidBody(btCollisionShape *objetoShape,btDiscreteDynamicsWorld* &dynamicsWorld);

        bool load_mesh (const char* filename, GLuint *vao, int *vert_no, btCollisionShape** col);
        bool load_mesh (const char* filename, GLuint *vao, int *vert_no);
        
};

#endif
