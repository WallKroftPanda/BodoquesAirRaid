#ifndef ZEPPELIN_H
#define ZEPPELIN_H

using namespace std;
class zeppelin{
    private:
        GLuint vao,vbo;
        int numVertices;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale ;
        glm::mat4 modelMatrix;
        char* filename;
        float angle;
    public:
        zeppelin(char *filename);
        //getters
        GLuint getVao();
        int getNumVertices();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        char* getFilename();
        //setters
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setPosition(glm::vec3 pos);
        void setRotation(float ang,glm::vec3 rot);
        void setFileName(char *f);
        void setModelMatrix(glm::mat4 model);

        void draw(int matloc);
};
#endif