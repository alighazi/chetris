#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/util/ImageInfo.h"
#include "core/sigil_board.h"
#include "core/boilerplate.h"
#include <experimental/vector>
using glm::vec3;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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

void Boilerplate::update(float deltaTime){

    // direction = Direction::NONE;
    // if(glfwGetKey(window, GLFW_KEY_LEFT)== GLFW_PRESS)
    //     direction = (Direction) (direction | Direction::LEFT);
    // if(glfwGetKey(window, GLFW_KEY_RIGHT)== GLFW_PRESS)
    //      direction = (Direction) (direction | Direction::RIGHT);
    // if(glfwGetKey(window, GLFW_KEY_DOWN)== GLFW_PRESS)
    //      direction = (Direction) (direction | Direction::DOWN);
    // if(glfwGetKey(window, GLFW_KEY_UP)== GLFW_PRESS)
    //      direction = (Direction) (direction | Direction::UP);
    //camera.move(deltaTime, dir& Direction::LEFT,  dir& Direction::RIGHT, dir& Direction::UP, dir&Direction::DOWN);    
}

Boilerplate::Boilerplate()
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    shader = new Shader("shader/default.vs", "shader/lamp.frag");

    // load and create a texture 
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
    shader->Use();

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader->setInt("texture1", 0);

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
    shader->setInt("texture2", 1);
}

Boilerplate::~Boilerplate()
{
    if(shader != nullptr){
        delete shader;
        shader = nullptr;
    }
}

void Boilerplate::addInputHandler(InputHandler* ic)
{ 
    input_handlers_.push_back(ic);
}

void Boilerplate::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static int depth=6, color_seed=15;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(action == GLFW_RELEASE && Boilerplate::instance().input_handlers_.size()){
        for(auto&& ic : Boilerplate::instance().input_handlers_)
        {
            ic->onKeyRelease(key);
        }
    }
        
}

void Boilerplate::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //camera.lookAt(xpos, ypos);
} 

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Boilerplate::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
