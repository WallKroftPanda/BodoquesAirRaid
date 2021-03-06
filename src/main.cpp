#include "addons.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLDebugDrawer.hpp"
#include "irrKlang.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace irrklang;


#define SKYBOX_VERTEX_SHADER_FILE "shaders/sky_vert.glsl"
#define SKYBOX_FRAGMENT_SHADER_FILE "shaders/sky_frag.glsl"
//VARIABLES GLOBALES


int g_gl_width = 1280;
int g_gl_height = 720;
int bullet_Count = 0;
GLFWwindow* g_window = NULL;
GLuint shader_programme;
//Posiciones de camara
camera *camara;
glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 aux;
btDiscreteDynamicsWorld* dynamicsWorld;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
btRigidBody* crearCuerpoRigido(float posX,float posY,float posZ,float masa,float alfa, float dirX,float dirY,float dirZ, float colX, float colY, float colZ);
void Init();
void activarMouse();
void GUILayout();
void checkCollision();
void drawBala();
bool load_cube_map_side( GLuint texture, GLenum side_target, const char *file_name ) {
	
	glBindTexture( GL_TEXTURE_CUBE_MAP, texture );

	int x, y, n;
	int force_channels = 4;
	unsigned char *image_data = stbi_load( file_name, &x, &y, &n, force_channels );
	if ( !image_data ) {
		fprintf( stderr, "ERROR: could not load %s\n", file_name );
		return false;
	}
	// non-power-of-2 dimensions check
	if ( ( x & ( x - 1 ) ) != 0 || ( y & ( y - 1 ) ) != 0 ) {
		fprintf( stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name );
	}

	// copy image data into 'target' side of cube map
	glTexImage2D( side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );
	free( image_data );
	return true;
}
// MOUSE
bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  g_gl_width / 2.0;
float lastY =  g_gl_height / 2.0;
float fov   =  45.0f;
/////

float deltaTime;
float timef;
float lastFrame;

//sonido
ISoundEngine *SoundEngine;

airplane* avion;

// Variables condicionales del transcurso del juego
bool inGame=false;
bool alive = true;
bool Win = false;
bool lose = false;
int count_balas = 0;


int model_mat_location;

airplane *bodoque;
zeppelin *e1;
//sky skycube;
suelo *elsuelo;
malla *ElMono;
malla *pickUp;
malla *ball;
malla *caja;
malla *bala;
malla *menu;

malla *aletasT;
malla *aletasL;
malla *aletaT;
helice *heli;
btVector3 f;

glm::vec4 auxhUp;
glm::vec4 auxhFront;
glm::vec4 auxCam;

btVector3 hUp;
btVector3 hFront;

bool firstBodoque;
float bPitch;
float bYaw;
float bRoll;
float bLastX;
float bLastY;

glm::mat4 projection;
glm::mat4 view;
int view_mat_location;
int proj_mat_location;

// pantalla de bienvenida
int menuInicio = 1;

btTransform trans;
btRigidBody* bodyBall;
btRigidBody* bodySuelo;
btRigidBody* bodyCaja;
btRigidBody* bodyHurri;
btRigidBody* bodyZep;
btQuaternion bHQT;

float pHx,pHy,pHz;//Posición del hurricane
float cHx,cHy,cHz;//Tamaño caja de colision de hurricane
float pZx,pZy,pZz,cZx,cZy,cZz;//Tamaño caja y posicion de zepelin

int main(){
    float points[] = {
		-1500.0f, 1500.0f,	-1500.0f, -1500.0f, -1500.0f, -1500.0f, 1500.0f,	-1500.0f, -1500.0f,
		1500.0f,	-1500.0f, -1500.0f, 1500.0f,	1500.0f,	-1500.0f, -1500.0f, 1500.0f,	-1500.0f,

		-1500.0f, -1500.0f, 1500.0f,	-1500.0f, -1500.0f, -1500.0f, -1500.0f, 1500.0f,	-1500.0f,
		-1500.0f, 1500.0f,	-1500.0f, -1500.0f, 1500.0f,	1500.0f,	-1500.0f, -1500.0f, 1500.0f,

		1500.0f,	-1500.0f, -1500.0f, 1500.0f,	-1500.0f, 1500.0f,	1500.0f,	1500.0f,	1500.0f,
		1500.0f,	1500.0f,	1500.0f,	1500.0f,	1500.0f,	-1500.0f, 1500.0f,	-1500.0f, -1500.0f,

		-1500.0f, -1500.0f, 1500.0f,	-1500.0f, 1500.0f,	1500.0f,	1500.0f,	1500.0f,	1500.0f,
		1500.0f,	1500.0f,	1500.0f,	1500.0f,	-1500.0f, 1500.0f,	-1500.0f, -1500.0f, 1500.0f,

		-1500.0f, 1500.0f,	-1500.0f, 1500.0f,	1500.0f,	-1500.0f, 1500.0f,	1500.0f,	1500.0f,
		1500.0f,	1500.0f,	1500.0f,	-1500.0f, 1500.0f,	1500.0f,	-1500.0f, 1500.0f,	-1500.0f,

		-1500.0f, -1500.0f, -1500.0f, -1500.0f, -1500.0f, 1500.0f,	1500.0f,	-1500.0f, -1500.0f,
		1500.0f,	-1500.0f, -1500.0f, -1500.0f, -1500.0f, 1500.0f,	1500.0f,	-1500.0f, 1500.0f
	};

    Init();
    GLuint vbosky;
	glGenBuffers( 1, &vbosky );
	glBindBuffer( GL_ARRAY_BUFFER, vbosky );
	glBufferData( GL_ARRAY_BUFFER, 3 * 36 * sizeof( GLfloat ), &points,
								GL_STATIC_DRAW );

	GLuint vaosky;
	glGenVertexArrays( 1, &vaosky );
	glBindVertexArray( vaosky );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vbosky );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    int model_mat_location = glGetUniformLocation(shader_programme, "model");
    //crea un contexto ,usado por imgui , y setea el inicio de la libreria
      
    const char* glsl_version = "#version 130";//version del shader usada para unir imgui con opengl3*/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(g_window, true);//une la libreria con glfw
    ImGui_ImplOpenGL3_Init(glsl_version);//une la libreria con opengl*/
    



    // Configuracion inical del mundo fisico Bullet
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	// Creacion del mundo fisico - Uno por aplicacion
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	// Vector de gravedad
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
    //btRigidBody* crearCuerpoRigido(float posX,float posY,float posZ,float masa,float alfa, float dirX,float dirY,float dirZ, btDiscreteDynamicsWorld* mundoFisico){
    //bodyBall = crearCuerpoRigido( 0.0 , 10.0, -10.0, 5.0,0.0,1.0,0.0,0.0,1.0,1.0,1.0);
    bodySuelo = crearCuerpoRigido(0.0f,0.0f,  0.0f, 0.0,0.0,0.0,0.0,1.0,500.0,1.0,500.0);
    bodySuelo->setUserPointer(&bodySuelo);
    dynamicsWorld->addRigidBody(bodySuelo);
    pZx=35.f;pZy=40.f;pZz=-100.f;cZx=25.0;cZy=3.0;cZz=3.0;
    e1->setRBody(crearCuerpoRigido(pZx,pZy,pZz,0.0f,0,0,0,1.0f,cZx,cZy,cZz));
    e1->getRBody()->setUserPointer(&e1);
    dynamicsWorld->addRigidBody(e1->getRBody());
    cHx=3.0;cHy=1.0;cHz=3.0;
    bodoque->setABody(crearCuerpoRigido(50.f,40.f,10.f,0.5f,0.0f,1.0f,0.0f,1.0f,cHx,cHy,cHz));
    bodoque->getABody()->setUserPointer(&bodoque);
    bodoque->getABody()->setLinearVelocity(btVector3(0.0f,0.f,-0.5f));
    dynamicsWorld->addRigidBody(bodoque->getABody());
    alive = true;
    
//    btRigidBody* crearCuerpoRigido(float posX,float posY,float posZ,float masa,float alfa, float dirX,float dirY,float dirZ, float colX, float colY, float colZ, btDiscreteDynamicsWorld* mundoFisico);

    // Setup style
    ImGui::StyleColorsDark();//setea el estilo de la ventana , igual puede ser clara
    
	GLDebugDrawer* debug = new GLDebugDrawer();
	debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe );
	debug->setView(&view);
	debug->setProj(&projection);
	dynamicsWorld->setDebugDrawer(debug);
	
    
	glm::mat4 modelMatrixUp;

    GLuint skybox_shader = create_programme_from_files ( SKYBOX_VERTEX_SHADER_FILE, SKYBOX_FRAGMENT_SHADER_FILE);
	glUseProgram (skybox_shader);

	int view_skybox = glGetUniformLocation (skybox_shader, "view");
	int proj_skybox = glGetUniformLocation (skybox_shader, "proj");
	glUniformMatrix4fv (view_skybox, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv (proj_skybox, 1, GL_FALSE, &projection[0][0]);
		


	GLuint cube_map_texture;
	glActiveTexture( GL_TEXTURE0 );
	glGenTextures( 1, &cube_map_texture );

	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "textures/back.jpg" );
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "textures/front.jpg"  );
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "textures/top.jpg" ) ;
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "textures/bottom.jpg"  );
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "textures/right.jpg" ) ;
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, "textures/left.jpg" ) ;

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	//Esto es para controlar front y visualizar vectores del avión
	//centro de masa del avión:
	btVector3 f = bodoque->getABody()->getCenterOfMassPosition();
	
	//Vector up inicializado como btVector3:
	//btVector3 prueba =btVector3(0.f,21.f,15.0f);
	
	//Si se quiere utilizar glm::vec3 para trabajar con los vectores del avión se debe descomentar esto:
	
	glm::vec4 temp;
	btTransform rot;
	//Por que se necesita una resta, para que dibuje la linea?
	float linea[] = {f.getX(), f.getY(), f.getZ(),temp.x, temp.y, temp.z};
	
	GLuint vboLin;
	glGenBuffers( 1, &vboLin );
	glBindBuffer( GL_ARRAY_BUFFER, vboLin );
	glBufferData( GL_ARRAY_BUFFER, 3 * 6 * sizeof( GLfloat ), &linea,GL_STATIC_DRAW );
	
	GLuint vaoLin;
	glGenVertexArrays( 1, &vaoLin );
	glBindVertexArray( vaoLin );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vboLin );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	activarMouse();
    while (!glfwWindowShouldClose(g_window)){
        
        if(menuInicio){
            debug->setView(&view);
            debug->setProj(&projection);
            dynamicsWorld->debugDrawWorld();
            debug->drawLines();
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram (shader_programme);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
            
            ImGui:: CreateContext ();
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            GUILayout();
            //funcion propia de imgui 
            ImGui::Render();
            //hace lo que creen que hace
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(g_window);
            glfwPollEvents();
            //time
        }
        if (inGame) {
            bodoque->setPosition(glm::vec3(0,40,10),model_mat_location);
            debug->setView(&view);
            debug->setProj(&projection);
            dynamicsWorld->debugDrawWorld();
            debug->drawLines();

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            if (deltaTime < 0.016f){
                continue;
            }
            lastFrame = currentFrame;
            /* inicializa los frames*/

            dynamicsWorld->stepSimulation(1.f / 60.f, 10);

                processInput(g_window);

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glUseProgram (skybox_shader);

            glDepthMask( GL_FALSE );
            glUseProgram( skybox_shader );
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_CUBE_MAP, cube_map_texture );
            glBindVertexArray( vaosky );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
            glDepthMask( GL_TRUE );
            
            glUseProgram (skybox_shader);
            glUniformMatrix4fv(view_skybox, 1, GL_FALSE, &view[0][0]);

            /*
                --date=relative
            */
            
            glUseProgram (shader_programme);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
            
            
            // ------
        
            //glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
                

            // activate shader
            // render

            // CREANDO CUERPOS RIGIDOS
            ////////////////////////////////
            
            bodySuelo->getMotionState()->getWorldTransform(trans);
            trans.getOpenGLMatrix(&aux[0][0]);
            elsuelo->setModelMatrix(aux);
            elsuelo->draw(model_mat_location);
            printf("\nHERE\n");
            /*
            bodyBall->getMotionState()->getWorldTransform(trans); // Se guarda la informacion de transformaciones de bodyBall en trans
            // Y se obtiene la matrix model directamente desde bullet
            trans.getOpenGLMatrix(&aux[0][0]);
            ball->setModelMatrix(aux);
            ball->draw(model_mat_location);
            */
            e1->getRBody()->getMotionState()->getWorldTransform(trans);
            trans.getOpenGLMatrix(&aux[0][0]);
            e1->setModelMatrix(aux);
            e1->draw(model_mat_location);
            
            //mapa
            //bodySuelo->getMotionState()->getWorldTransform(trans);
            //trans.getOpenGLMatrix(&aux[0][0]);
            //elsuelo->setModelMatrix(aux);
            //elsuelo->draw(model_mat_location);
            menu->draw(model_mat_location);
            menu->setPos(glm::vec3(10.0f,15.0f,-25.0f));
            
            //Hurricane
            if(alive)
            {
                //Hurricane
                bodoque->getABody()->getMotionState()->getWorldTransform(trans);
                trans.getOpenGLMatrix(&aux[0][0]);
                bodoque->setModelMatrix(aux);
                bodoque->draw(model_mat_location);
                aletasT->setModelMatrix(aux);
                aletasL->setModelMatrix(aux);
                aletaT->setModelMatrix(aux);
                aletasT->draw(model_mat_location);
                aletasL->draw(model_mat_location);
                aletaT->draw(model_mat_location);
                heli->setModelMatrix(aux);
                heli->setRotation(1,glm::vec3(1,0,0));
                heli->draw(model_mat_location);
                ///w///////////////////////
                if(bodoque->getABody()->getCenterOfMassPosition().getY()<=150)
                {
                    bodoque->getABody()->applyForce(btVector3(0,5,0),btVector3(0,1,0));
                }
                //bodoque->getABody()->setAngularVelocity(bodoque->getABody()->getAngularVelocity()*-1);
                btVector3 bpos = bodoque->getABody()->getCenterOfMassPosition();
                cameraPos = glm::vec3(bpos.getX(),bpos.getY()+1.f,bpos.getZ()+6.f);
                pHx=bpos.getX();
                pHy=bodoque->getABody()->getCenterOfMassPosition().getY();
                pHz=bodoque->getABody()->getCenterOfMassPosition().getZ();
                f = bodoque->getABody()->getCenterOfMassPosition();
                
                auxCam = aux*glm::vec4(-5.f,1.f,0.f,1.f);
                
                cameraPos.x = auxCam.x;
                cameraPos.y = auxCam.y;
                cameraPos.z = auxCam.z;
                
                cameraUp.x = hUp.getX();
                cameraUp.y = hUp.getY();
                cameraUp.z = hUp.getZ();

                f = bodoque->getABody()->getCenterOfMassPosition();
            
                hUp.setX(auxhUp.x-f.getX());
                hUp.setY(auxhUp.y-f.getY());
                hUp.setZ(auxhUp.z-f.getZ());
                
                hFront.setX(auxhFront.x-f.getX());
                hFront.setY(auxhFront.y-f.getY());
                hFront.setZ(auxhFront.z-f.getZ());
            
                cameraFront.x = hFront.getX();
                cameraFront.y = hFront.getY();
                cameraFront.z = hFront.getZ();
            }

            
            //Por si se quiere usar btVector3
            
            //Por si se quiere usar glm::vec3
            auxhUp = aux*glm::vec4(0.f,1.f,0.f,1.f);
            auxhFront = aux*glm::vec4(1.f,0.f,0.f,1.f);
            
            //Por si se quiere usar btQuaternion
            //bFront=bodyHurri->getOrientation();
            
            
            //En este print se imprime la posicíón    	Y la posición del punto que representa el "UP" 
            // del centro de masa a la derecha			del avión a la izquierda
            
            //ELija uno de los print dependiendo de con que tipo de dato está usando:
            //btVector3 o btQuaternion:
            printf("X: %f Y: %f Z: %f --- X: %f Y: %f Z: %f \n", f.getX(), f.getY(), f.getZ(), auxCam.x, auxCam.y, auxCam.z);
            
            //glm::vec3
            //printf("X: %f Y: %f Z: %f --- X: %f Y: %f Z: %f \n", f.getX(), f.getY(), f.getZ(), prueba.x, prueba.y, prueba.z);
            glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, &aux[0][0]);
            glBindVertexArray( vaoLin );
            glDrawArrays( GL_LINES, 0, 2 );
            
            /*linea[0]=c.getX();
            linea[1]=c.getY();
            linea[2]=c.getZ();
            linea[3]=axis.getX();
            linea[4]=axis.getY();
            linea[5]=axis.getZ();*/
            //Se dibuja la linea de orientacion
            ///w///////////////////////

            //camara->setProjection(fov);

            //camara->setView();
            //Dibujar suelo
            
            //Dibujar zeppelin
            

            // DIBUJAR CAJA DE MUNICION
            //pickUp->setPos(glm::vec3(-10.0f,15.0f,-25.0f));
            // glBindVertexArray(pickUp->getvao());
            // glDrawArrays(GL_TRIANGLES,0,pickUp->getnumvertices());
            //pickUp->setModelMatrix(aux);
            //pickUp->draw(model_mat_location);
            //Dibujar ElMono
            /*
            ElMono->setPos(glm::vec3(-30.0f,2.0f,5.0f));
            glBindVertexArray(ElMono->getVao());
            glDrawArrays(GL_TRIANGLES,0,ElMono->getNumVertices());
            */
            /*e1->setPosition(glm::vec3(4.0f,-1.0f,0.0f));
            glBindVertexArray(e1->getVao());
            e1->draw(model_mat_location);

            /*funcion donde se define lo que se dibuja de la GUI*/
            debug->setView(&view);
            debug->setProj(&projection);
            dynamicsWorld->debugDrawWorld();
            debug->drawLines();
            
            checkCollision();
            glfwSwapBuffers(g_window);
            glfwPollEvents();
        }
        
        if (!alive) {
            debug->setView(&view);
            debug->setProj(&projection);
            dynamicsWorld->debugDrawWorld();
            debug->drawLines();
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram (shader_programme);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
            ImGui:: CreateContext ();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            bool open = true;

            ImGui::Begin("Lo lamentamos pero has perdido ",&open);
            ImGui::SetWindowPos("Lo lamentamos pero has perdido ",ImVec2(10,10));
            ImGui::SetWindowSize(ImVec2(g_gl_width-20, g_gl_height-20));
            ImGui::StyleColorsLight(NULL);
            ImGui::Text("Intentalo Otra vez");
            ImGui::Text("Presiona ESCAPE para finalizar");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            if (ImGui::Button("CERRAR")) {
                exit(1);
            }
            ImGui::End();
            ImGui::Render();

            processInput(g_window);
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(g_window);
            glfwPollEvents();
        }
        
        if (Win) {
            debug->setView(&view);
            debug->setProj(&projection);
            dynamicsWorld->debugDrawWorld();
            debug->drawLines();
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram (shader_programme);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
            ImGui:: CreateContext ();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("¡HAS GANADO FELICIDADES!!!!!! ");
            ImGui::SetWindowPos("¡HAS GANADO FELICIDADES!!!!!! ",ImVec2(10,10));
            ImGui::SetWindowSize(ImVec2(g_gl_width-20, g_gl_height-20));
            ImGui::StyleColorsLight(NULL);
            ImGui::Text("ENHORABUENA LO HAS LOGRADO!!!!!!!!!!!!!!!!");
            ImGui::Text("Presiona ESCAPE para finalizar");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            if (ImGui::Button("CERRAR")) {
                exit(1);
            }
            ImGui::End();       
            ImGui::Render();

            processInput(g_window);
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(g_window);
            glfwPollEvents();
        }
        

        
        
    }

        /*limpia todo*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void GUILayout(){
            const ImVec2 size(1400, 1200);
            bool open = true;
            float alpha = 0.90f;

              
            ImGui::Begin("Hello, world!",&open, alpha);                          // Create a window called "Hello, world!" and append into it.
            ImGui::SetWindowPos("Hello, world!",ImVec2(10,10));
            ImGui::SetWindowSize(ImVec2(g_gl_width-20, g_gl_height-20));
            ImGui::Text("Bienvenidos a el proyecto Bodoque.....para un final!!!");               // Display some text (you can use a format strings too) // Edit 3 floats representing a color
            ImGui::Text("Para Seleccionar una opcion debe presionar la tecla ESPACIO");
            if (ImGui::Button("Jugar")) {                           // Buttons return true when clicked (most widgets return true when edited/activated)
                
                inGame=true;
                menuInicio = false;
            }else if(ImGui::Button("Salir")){
                glfwSetWindowShouldClose(g_window, true);
            }
            
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Instrucciones:\nFlechas direccionales: UP-arriba, DOWN-abajo, LEFT-izquierda, RIGHT-derecha\nLEFT-CTRL: Frenar\nLEFT-ALT: Acelerar\nX: Estabilizar\nZ: Disparar\n");
            ImGui::End();
        
}


void activarMouse(){
    glfwSetCursorPosCallback(g_window, mouse_callback);
	glfwSetScrollCallback(g_window, scroll_callback);
    
	// Le decimos a GLFW que capture nuestro mouse
	glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Init(){
	//Funciones contenidas en gl_utils.cpp
	
	restart_gl_log ();
	start_gl();
	//Funciones propias de GL
	glEnable (GL_DEPTH_TEST);
	glDepthFunc ( GL_LESS);
	/*lEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);*/
	glClearColor (0.2, 0.2, 0.2, 1.0);
	glViewport (0, 0, g_gl_width, g_gl_height);
	
	glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
	
	//ESTO SOLO DEBE INCLUIRSE POR AHORA, AL MOMENTO DE CONSTRUIR EL JUEGO, YA QUE NUESTRO JUEGO NO OCUPA MOUSE

    glfwSetCursorPosCallback(g_window, mouse_callback);
	glfwSetScrollCallback(g_window, scroll_callback);
    
	// Le decimos a GLFW que capture nuestro mouse
	glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
		/*-------------------------------Creamos Shaders-------------------------------*/
	shader_programme = create_programme_from_files (
	VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 1000000.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
	proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, &projection[0][0]);

    model_mat_location=  glGetUniformLocation (shader_programme, "model");
    
    bodoque = new airplane((char*)"mallas/Hurricane.obj",shader_programme);
    bodoque->load_surface("textures/Hurricane_model.jpg");
    bodoque->load_specular("textures/earth-specular-1k.jpg");
    e1 = new zeppelin((char*)"mallas/dirigible.obj");
   	elsuelo = new suelo((char*)"mallas/sueloRef.obj");
    ElMono = new malla((char*)"mallas/suzanne.obj",shader_programme);
    pickUp = new malla((char*) "mallas/caja.obj",shader_programme);
    ball = new malla((char*)"mallas/ball.obj",shader_programme);
	aletasT = new malla((char*)"mallas/aletas_traseras.obj",shader_programme);
	aletasL = new malla((char*)"mallas/aletas_laterales.obj",shader_programme);
	aletaT = new malla((char*)"mallas/aleta_trasera_vert.obj",shader_programme);
	heli = new helice((char*)"mallas/hélice.obj");
    menu = new malla((char*)"mallas/menu_principal.obj",shader_programme);
    menu->load_surface("textures/fondoxd.png");
	SoundEngine = createIrrKlangDevice();
	if (!SoundEngine)
	{
		printf("Could not startup engine\n");
	}
	SoundEngine->play2D("src/bgsound1.ogg", true);
    SoundEngine->play2D("src/plane.ogg", true);
    
    printf("sadada");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 25 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        //bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,0.f));
 		bodoque->getABody()->setAngularVelocity(/*bodoque->getABody()->getAngularVelocity()  -*/ btVector3(-0.1f,0.f,0.f));
        bodoque->getABody()->setLinearVelocity(bodoque->getABody()->getLinearVelocity() + btVector3(0.0f,-0.001f,0.f));
        /*
        cameraFront.x = hFront.getX();
        cameraFront.y = hFront.getY();
        cameraFront.z = hFront.getZ();
        */
	}       
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		//bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,0.f));
 		bodoque->getABody()->setAngularVelocity(/*bodoque->getABody()->getAngularVelocity()  +*/ btVector3(0.1f,0.f,0.f));
        bodoque->getABody()->setLinearVelocity(bodoque->getABody()->getLinearVelocity() + btVector3(0.0f,0.001f,0.f));
        /*
        cameraFront.x = hFront.getX();
        cameraFront.y = hFront.getY();
        cameraFront.z = hFront.getZ();
        */
	}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        //bodoque->getABody()->setAngularVelocity(btVector3(0.0f,bodoque->getABody()->getAngularVelocity().getY(),bodoque->getABody()->getAngularVelocity().getX()));
        bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,0.0f));
        bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,0.65f));
        cameraFront.x = hFront.getX();
        cameraFront.y = hFront.getY();
        cameraFront.z = hFront.getZ(); 
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,0.0f));
        bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,-0.65f));
        cameraFront.x = hFront.getX();
        cameraFront.y = hFront.getY();
        cameraFront.z = hFront.getZ();
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        bodoque->getABody()->setAngularVelocity(btVector3(0.f,0.f,0.0f));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        if(bodoque->getABody()->getLinearVelocity().getZ()<-0.5f)bodoque->getABody()->applyCentralForce(btVector3(0.f,0.f,1.f));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS){
        bodoque->getABody()->applyCentralForce(hFront);
    }
    if(alive && glfwGetKey(window, GLFW_KEY_Z)== GLFW_PRESS){
        drawBala();
    }
     
}
//Camara casi completamente migrada, esta función se me hace un problema, ya que se llama a esta función en INIT(), al quererla reemplazar dire3ctamente con la funcion camara->actualizar(), me daba error, por lo que decidí dejarla tal cual está, y llamar a "actualizar dentro de esta.
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

btRigidBody* crearCuerpoRigido(float posX,float posY,float posZ,float masa,float alfa, float dirX,float dirY,float dirZ, float colX, float colY, float colZ){

    btCollisionShape* Shape = new btBoxShape(btVector3(colX,colY,colZ));

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(posX,posY,posZ)); // Posicion incial
    btScalar Mass(masa); // Masa

    bool isDynamic = (Mass != 0.f);
    Transform.setRotation(btQuaternion(alfa,dirX,dirY,dirZ));

    btVector3 localInertia(1, 0, 0);
    if (isDynamic)
        Shape->calculateLocalInertia(Mass, localInertia);

    btDefaultMotionState* MotionState = new btDefaultMotionState(Transform);
    btRigidBody::btRigidBodyConstructionInfo RbInfo(Mass, MotionState, Shape, localInertia);
    btRigidBody* body = new btRigidBody(RbInfo);
    body->setActivationState(DISABLE_DEACTIVATION);  
    return body;
}

/*void figuraColisionadora(int convex){
    btCollisionShape* newShape;
    if (convex)
    {
        //2
        newShape = new btConvexHullShape();
        for (int i = 0; i < vertexCount; i++)
        {
            Vertex v = vertices[i];
            btVector3 btv = btVector3(v.Position[0], v.Position[1], v.Position[2]);
            ((btConvexHullShape*)newShape)->addPoint(btv);
        }
    }
    else
    {
        //3
        btTriangleMesh* mesh = new btTriangleMesh();
        for (int i=0; i < vertexCount; i += 3)
        {
            Vertex v1 = vertices[i];
            Vertex v2 = vertices[i+1];
            Vertex v3 = vertices[i+2];
            
            btVector3 bv1 = btVector3(v1.Position[0], v1.Position[1], v1.Position[2]);
            btVector3 bv2 = btVector3(v2.Position[0], v2.Position[1], v2.Position[2]);
            btVector3 bv3 = btVector3(v3.Position[0], v3.Position[1], v3.Position[2]);
            
            mesh->addTriangle(bv1, bv2, bv3);
        }        
           newShape = new btBvhTriangleMeshShape(mesh, true);
    }
}*/

void checkCollision()
{
    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    #pragma omp parallel for schedule(dynamic,4)      // PARALELISMO
    for(int i = 0; i < numManifolds; ++i)
    {
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
        
        if((&bodoque==obA->getUserPointer() and &e1==obB->getUserPointer()) or (&bodoque==obB->getUserPointer() and &e1==obA->getUserPointer()))
        {
            alive = false;
            inGame = false;
            printf("Choco zepelin con avion\n");
        }   
        else if((&bodySuelo==obA->getUserPointer() and &bodoque==obB->getUserPointer()) or (&bodoque==obA->getUserPointer() and &bodySuelo==obB->getUserPointer()))
        {
            alive = false;
            inGame = false;
            printf("Choco avion con el suelo\n");
        }
        else if((&bala==obB->getUserPointer() && &e1==obA->getUserPointer())||(&bala==obA->getUserPointer() && &e1==obB->getUserPointer()))
        {
            
            count_balas++;
            if(count_balas >= 500)
            {
                printf("Zepelin derribado\n");
                Win = true;
                inGame = false;
            }
        }
    }
}

void drawBala()
{
    
    bala = new malla((char*)"mallas/ball.obj",shader_programme);
    btVector3 inst_pos = bodoque->getABody()->getCenterOfMassPosition();
    bala->setMBody(crearCuerpoRigido(inst_pos.getX(),inst_pos.getY(),inst_pos.getZ()-2.f,0.001f,0.0f,1.0f,0.0f,1.0f,0.1f,0.1f,0.1f));
    
    bala->getMBody()->setLinearVelocity(bodoque->getABody()->getLinearVelocity() + btVector3(0.f,0.f,-2.5f));
    
    bala->getMBody()->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(&aux[0][0]);
    bala->setModelMatrix(aux);
    bala->draw(model_mat_location);
    bala->getMBody()->applyCentralForce(btVector3(6*hFront.getX(),3*hFront.getY(),3*hFront.getZ()));
    bala->getMBody()->setUserPointer(&bala);
    dynamicsWorld->addRigidBody(bala->getMBody());
    
    printf("Ok\n");
    
}
