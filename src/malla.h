#ifndef MALLA_H
#define MALLA_H
using namespace std;

class malla{
    private:
        GLuint vao, vbo;
        int numvertices;
        float angle;
        glm::vec3 pos;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 modelMatrix;
        char* filename;




    public:
        malla(char *filename);

        // gets
        GLuint getvao();
        int getnumvertices();
        glm::vec3 getpos();
        glm::vec3 getRotation();
        char* getfilename();

        // sets
        void setvao(GLuint vao);
        void setnumvertices(int num);
        void setpos(glm::vec3 p);
        void setRotation(float ang, glm::vec3 rot);
        void setModelMatrix(glm::mat4 model);
        void setfilename(char *f);

        void draw(int matloc);
};

#endif
