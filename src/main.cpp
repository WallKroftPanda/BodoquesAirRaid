#include "addons.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "airplane.h"
#include "zeppelin.h"
#include "camera.h"
#include "suelo.h"
//VARIABLES GLOBALES

int g_gl_width = 1280;
int g_gl_height = 720;
GLFWwindow* g_window = NULL;
GLuint shader_programme;
//Posiciones de camara
camera *camara;
/*glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void Init();
btRigidBody* crear_Objeto(btVector3 posicion, btCollisionShape *objetoShape,btQuaternion rotacion, float masa);
void activarMouse();
void GUILayout();
float deltaTime;
float timef;
float lastFrame;
float fov   =  16.0f;
airplane* avion;
bool inGame=false;
float xbd,ybd,zbd; //x,y,z de bodoque
int model_mat_location;
airplane *bodoque;
zeppelin *e1;
Suelo *elsuelo;
Suelo *pickUp;

btDiscreteDynamicsWorld* dynamicsWorld;

int main()
{

    Init();
    camara= new camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),g_gl_width,g_gl_height);
    
    camara->setProjection(fov);
    
    camara->setView();

	camara->setViewMatLocation(shader_programme);
	
	camara->setProjMatLocation(shader_programme);
    int model_mat_location = glGetUniformLocation(shader_programme, "model");
	glm::vec3 cam;
    /* crea un contexto ,usado por imgui , y setea el inicio de la libreria
      */
    const char* glsl_version = "#version 130";//version del shader usada para unir imgui con opengl3
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(g_window, true);//une la libreria con glfw
    ImGui_ImplOpenGL3_Init(glsl_version);//une la libreria con opengl

    // Setup style
    ImGui::StyleColorsDark();//setea el estilo de la ventana , igual puede ser clara
	
    glm::mat4 aux;
    while (!glfwWindowShouldClose(g_window)){

        //time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (deltaTime < 0.016f){
            continue;
        }
        lastFrame = currentFrame;
        timef+=deltaTime;
        /*inicializa los frames*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(inGame){
            processInput(g_window);
        }
        //
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam = camara->getCameraPos();
        //cout<<xbd<<","<<ybd<<","<<zbd<<endl;

        // activate shader
		glUseProgram (shader_programme);

	    camara->setProjection(fov);

        camara->setView();
        //Dibujar suelo
        elsuelo->setPos(glm::vec3(0.0f,-1.0f,0.0f),model_mat_location);
        glBindVertexArray(elsuelo->Malla::getVao());
        glDrawArrays(GL_TRIANGLES,0,elsuelo->Malla::getNumVertices());
        
        //Dibujar avión
        /*
        bodoque->setPosition(glm::vec3(1,8,1), model_mat_location);
        bodoque->setRotation(1.57f, glm::vec3(0,1,0),model_mat_location);
        glBindVertexArray(bodoque->getVao());
        glDrawArrays(GL_TRIANGLES,0,bodoque->getNumVertices());
        */
        btTransform trans;
        /*test fisico avion y mapa
        
        
        //avion
        bodoqueBody->getMotionState()->getWorldTransform(trans); // Se guarda la informacion de transformaciones de bodyBall en trans
        // Y se obtiene la matrix model directamente desde bullet
        trans.getOpenGLMatrix(&aux[0][0]);
        bodoque->setModelMatrix(aux);
        bodoque->draw(model_mat_location);
        //fin avion
        mapaBody->getMotionState()->getWorldTransform(trans);
        trans.getOpenGLMatrix(&aux[0][0]);
        elsuelo->Malla::setModelMatrix(aux);
        elsuelo->Suelo::draw(model_mat_location);
        */
        elsuelo->Malla::getRigidBody()->getMotionState()->getWorldTransform(trans);
        elsuelo->Malla::setModelMatrix(aux);
        elsuelo->Suelo::draw(model_mat_location);
        
        //mapa

        //fin mapa
        //fin test fisico*/
        
        //Dibujar zeppelin
        e1->setPos(glm::vec3(-21.0f,10.0f,-50.0f), model_mat_location);
        glBindVertexArray(e1->Malla::getVao());
        glDrawArrays(GL_TRIANGLES,0,e1->Malla::getNumVertices());

        // DIBUJAR CAJA DE MUNICION
        pickUp->setPos(glm::vec3(-10.0f,15.0f,-25.0f),model_mat_location);
        glBindVertexArray(pickUp->Malla::getVao());
        glDrawArrays(GL_TRIANGLES,0,pickUp->Malla::getNumVertices());

		/*e1->setPosition(glm::vec3(4.0f,-1.0f,0.0f));
        glBindVertexArray(e1->getVao());
        e1->draw(model_mat_location);

        //funcion donde se define lo que se dibuja de la GUI
*/      GUILayout();

        //funcion propia de imgui 
        ImGui::Render();
        //hace lo que creen que hace
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(g_window);
        glfwPollEvents();
        xbd = 0;
        ybd = 10;
        zbd = 0;
        camara->setCameraPos(glm::vec3(xbd,ybd,zbd));
    }

    //limpia todo
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void GUILayout(){
    
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too) // Edit 3 floats representing a color

            if (ImGui::Button("Jugar")) {                           // Buttons return true when clicked (most widgets return true when edited/activated)
                activarMouse();
                inGame=true;
            }else if(ImGui::Button("Salir")){
                glfwSetWindowShouldClose(g_window, true);
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glClearColor (0.2, 0.2, 0.2, 1.0);
	glViewport (0, 0, g_gl_width, g_gl_height);
	
	glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
	
	//ESTO SOLO DEBE INCLUIRSE POR AHORA, AL MOMENTO DE CONSTRUIR EL JUEGO, YA QUE NUESTRO JUEGO NO OCUPA MOUSE

	//glfwSetCursorPosCallback(g_window, mouse_callback);
	//glfwSetScrollCallback(g_window, scroll_callback);
    
	// Le decimos a GLFW que capture nuestro mouse
	//glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
		/*-------------------------------Creamos Shaders-------------------------------*/
	shader_programme = create_programme_from_files (
	VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    model_mat_location=  glGetUniformLocation (shader_programme, "model");
    
    bodoque = new airplane((char*)"mallas/Hurricane.obj");
    e1 = new zeppelin((char*)"mallas/dirigible.obj");
   	elsuelo = new Suelo((char*)"mallas/sueloRef.obj",0);
    pickUp = new Suelo((char*) "mallas/caja.obj",0);
    // Configuracion inical del mundo fisico Bullet
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // Creacion del mundo fisico - Uno por aplicacion
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    // Vector de gravedad
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 25 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camara->setCameraPos(8,cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camara->setCameraPos(2,cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camara->setCameraPos(4,cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camara->setCameraPos(6,cameraSpeed);
}
//Camara casi completamente migrada, esta función se me hace un problema, ya que se llama a esta función en INIT(), al quererla reemplazar dire3ctamente con la funcion camara->actualizar(), me daba error, por lo que decidí dejarla tal cual está, y llamar a "actualizar dentro de esta.
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
		camara->actualizar(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
       
}

void Logica(){
    /*
    //  DISPARAR
    //  MOVERSE
    Movimiento en desarrollo, idealmente se rota con A (izquierda) y D (derecha),
    S para subir el morro y W para bajarlo, se intentara implementar comandos para
    acelerar o desacelerar el avion (el avion no poseera velocidad constante).
    //  COLISIONAR
    Dirigibles y el suelo seran objetos en los cuales si el jugador choca, pierde
    //  MAPA
    El mapa se modificara acorde al desarrollo del juego, de manera preliminar
    se puede decir que al salir del mapa el jugador pierde automaticamente
    */
}