#ifndef BALA_H
#define BALA_H

using namespace std;

class bala{
    private:
        GLuint vao,vbo;
        int numVertices;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 vectorVelocity;
        glm::mat4 T;
        glm::mat4 modelMatrix;
        char* filename;
        float angle;
        float velocity;
    public:
        bala(char* filename, glm::vec3 pos, glm::vec3 orientation);
        //getters
        GLuint getVao();
        int getNumVertices();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getVectorVelocity();
        char* getFilename();

        //setters
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setPosition(glm::vec3 pos, int model);
        void setRotation(float ang,glm::vec3 rot,int model);
        void setVectorVelocity(float velocity, glm::vec3 dir);
        void setFileName(char *f);
        void setModelMatrix(glm::mat4 model);

        void draw(int matloc);

        //Implementar
        void vuelo(glm::vec3 pos, float velocity);
        void colition();
};
#endif
