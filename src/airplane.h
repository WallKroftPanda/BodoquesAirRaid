#ifndef AIRPLANE_H
#define AIRPLANE_H


using namespace std;
class airplane{
    private:
        GLuint vao,vbo;
        int numVertices;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale ;
        glm::mat4 T;
        glm::mat4 modelMatrix;
        char* filename;
        float angle;
    public:
        airplane(char* filename);
        //getters
        GLuint getVao();
        int getNumVertices();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        char* getFilename();

        //setters
        void setVao(GLuint vao);
        void setNumVertices(int num);
        void setPosition(glm::vec3 pos, int model);
        void setRotation(float ang,glm::vec3 rot);
        void setRotation(float ang,glm::vec3 rot,int model);
        void setFileName(char *f);
        void setModelMatrix(glm::mat4 model);

        void draw(int matloc);
        
        //Funcionalidades del avión deben ser implementadas más adelante junto con bullet... 
        void explotar();
        void girar_IZQ(); //movimiento de palas para girar a la izquierda 
        void girar_DER();//movimiento de palas para girar a la derecha
        void inc_VEL(); //Incrementar velocidad
        void dis_VEL(); //Reducir velocidad
        
};
#endif
