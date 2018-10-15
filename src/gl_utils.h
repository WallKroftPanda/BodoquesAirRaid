#ifndef _GL_UTILS_H_
#define _GL_UTILS_H_

#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;

bool restart_gl_log ();
bool start_gl ();
bool gl_log (const char* message, ...);
bool gl_log_err (const char* message, ...);

void glfw_error_callback (int error, const char* description);
void glfw_window_size_callback (GLFWwindow* window, int width, int height);

//FUNCIONES PARA CREAR SHADERS
bool is_programme_valid (GLuint sp);
bool parse_file_into_str (const char* file_name, char* shader_str, int max_len);
bool create_shader (const char* file_name, GLuint* shader, GLenum type);
bool create_shader (const char* file_name, GLuint* shader, GLenum type);
bool create_programme (GLuint vert, GLuint frag, GLuint* programme);
GLuint create_programme_from_files (const char* vert_file_name, const char* frag_file_name);

#endif
