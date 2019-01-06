#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/util/Shader.h"
#include "core/Camera.h"
#include "core/util/ImageInfo.h"
#include "core/util/to_string.hpp"
#include "core/sigil.h"
using glm::vec3;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	printf("\n*** ");
	if(fif != FIF_UNKNOWN) {
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
#pragma GCC diagnostic ignored "-Wformat-security"
	printf(message);
#pragma GCC diagnostic pop
	printf(" ***\n");
}

Camera camera(SCR_WIDTH, SCR_HEIGHT);
enum Direction{
    NONE = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 4,
    LEFT = 8
};
void update(GLFWwindow* window, float deltaTime){

    Direction dir= Direction::NONE;
    if(glfwGetKey(window, GLFW_KEY_LEFT)== GLFW_PRESS)
        dir = (Direction) (dir | Direction::LEFT);
    if(glfwGetKey(window, GLFW_KEY_RIGHT)== GLFW_PRESS)
         dir = (Direction) (dir | Direction::RIGHT);
    if(glfwGetKey(window, GLFW_KEY_DOWN)== GLFW_PRESS)
         dir = (Direction) (dir | Direction::DOWN);
    if(glfwGetKey(window, GLFW_KEY_UP)== GLFW_PRESS)
         dir = (Direction) (dir | Direction::UP);
    camera.move(deltaTime, dir& Direction::LEFT,  dir& Direction::RIGHT, dir& Direction::UP, dir&Direction::DOWN);    
}
int main()
{ 
	FreeImage_SetOutputMessage(FreeImageErrorHandler);
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader shader2("shader/default.vs", "shader/lamp.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<Sigil*> sigils;
    sigils.push_back(new Sigil(Sigil::blocks_S, vec3(-2.f,0.f,-3.f)));
    sigils.push_back(new Sigil(Sigil::blocks_SR, vec3(-1.f,0.f,-3.f)));
    sigils.push_back(new Sigil(Sigil::blocks_L, vec3(0.f,0.f,-3.f)));
    sigils.push_back(new Sigil(Sigil::blocks_LR, vec3(1.f,0.f,-3.f)));
    sigils.push_back(new Sigil(Sigil::blocks_Box, vec3(2.f,0.f,-3.f)));
    sigils.push_back(new Sigil(Sigil::blocks_Bar, vec3(-3.f,0.f,-3.f)));
    sigils.push_back(new Sigil(Sigil::blocks_S, vec3(3.f,0.f,-3.f)));

    // load and create a texture 
    unsigned int texture1, texture2;
    // texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
	ImageInfo img = ImageInfo("data/img/face.png");
	if(img.isOk()){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		img.free();
	}else {
		std::cout<<"Failed to load: "<<img.getFileName();
	}
    shader2.Use();

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader2.setInt("texture1", 0);

    // texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
	img = ImageInfo("data/img/wud.jpg");
	if(img.isOk()){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		img.free();
	}else {
		std::cout<<"Failed to load: "<<img.getFileName();
	}
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader2.setInt("texture2", 1);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        static float currentSecond;
        static int fps;
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        currentSecond += deltaTime;
        fps++;
        if(currentSecond >= 1){
            std::cout<<"FPS: "<<fps<<std::endl;
            currentSecond = 0;
            fps = 0;
        }

        update(window, deltaTime);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        shader2.Use();
        shader2.setMat4("view", *camera.getView());
        shader2.setMat4("projection", *camera.getProjection());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        for(int i=0;i<sigils.size();i++){
            sigils[i]->update(deltaTime, currentFrame);
            sigils[i]->render(&shader2);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    for(int i=0;i<sigils.size();i++){
        delete sigils[i];
    }
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static int depth=6, color_seed=15;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float pitch=0.f, yaw=-90.f;
double lastX, lastY;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.lookAt(xpos, ypos);
} 

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

