#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tools.h"
#include "airplane.h"
#include <assert.h>
#include "stb_image.h"
using namespace std;

airplane::airplane(char* filename,GLuint shader_programme){
    this->filename = filename;
    this->position = glm::vec3(0,0,0);
	this->shader_programme = shader_programme;
	this->matloc = glGetUniformLocation (this->shader_programme, "model");
    this->angle = 0.0f;
    this->life = 100.0f;
    assert(load_mesh(filename,&vao,&numVertices));
}

GLuint airplane::getVao(){
    return this->vao;
}
int airplane::getNumVertices(){
    return this->numVertices;
}
float airplane::getLife(){
    return this->life;
}
int airplane::getBullets(){
    return this->bullets;
}
glm::vec3 airplane::getFront(){
	return this->front;
}
glm::vec3 airplane::getPosition(){
    return this->position;
}
glm::vec3 airplane::getRotation(){
    return this->rotation;
}
char* airplane::getFilename(){
    return this->filename;
}
void airplane::setPosition(glm::vec3 pos,int model){
    T = glm::translate(glm::mat4(1.0f),pos);
    glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void airplane::setRotation(float ang, glm::vec3 rot, int model){
        T = glm::rotate(glm::mat4(T),ang,rot);
        glUniformMatrix4fv(model, 1,GL_FALSE, glm::value_ptr(T));
}

void airplane::setModelMatrix(glm::mat4 model){
    this->modelMatrix = model;
}
void airplane::setBullets(int bullet){
    this->bullets = bullet;
}

void airplane::draw(int matloc){
		//matloc = glGetUniformLocation(shaderprog,"model");
		glActiveTexture(GL_TEXTURE0);
    	glBindVertexArray(getVao());
    	glBindTexture(GL_TEXTURE_2D, tex);
	    glUniformMatrix4fv(this->matloc, 1, GL_FALSE, &this->modelMatrix[0][0]);
        glBindVertexArray(this->getVao());
        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
}

bool airplane::load_texture (const char* file_name, GLuint *tex, GLenum texslot) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load (file_name, &x, &y, &n, force_channels);
    //printf("x = %i    y = %i\n", x, y);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	glGenTextures (1, tex);
	glActiveTexture(texslot);
	glBindTexture (GL_TEXTURE_2D, *tex);
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap (GL_TEXTURE_2D);
    // probar cambiar GL_CLAMP_TO_EDGE por GL_REPEAT
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 16.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	return true;
}

bool airplane::load_surface(const char *filename){
    this->load_texture(filename, &this->tex, GL_TEXTURE0);
    // load texture
    int tex_location = glGetUniformLocation( this->shader_programme, "rgb_sampler" );
    //assert( tex_location > -1 );
    glUniform1i( tex_location, 0 );
    return true;
}

bool airplane::load_specular(const char *filename){
    this->load_texture(filename, &this->stex, GL_TEXTURE1);
    int tex_location = glGetUniformLocation(this-> shader_programme, "spec_sampler" );
    //assert( tex_location > -1 );
    glUniform1i( tex_location, 1 );
    return true;
}