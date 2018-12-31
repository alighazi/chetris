#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/util/Shader.h"
#include "core/util/ImageInfo.h"
#include "core/util/to_string.hpp"

using glm::vec3;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

std::vector<glm::vec3> inner_triangle(glm::vec3 &a,glm::vec3 &b,glm::vec3 &c, int depth){
    float z= -depth/100.f;
    auto in_a = (c-b)/2.f+b;
    auto in_b = (a-c)/2.f+c;
    auto in_c = (b-a)/2.f+a;
    return  {
                glm::vec3(in_a.x, in_a.y, z),
                glm::vec3(in_b.x, in_b.y, z),
                glm::vec3(in_c.x, in_c.y, z)
            };
}

std::vector<glm::vec3> fractal(glm::vec3 &a,glm::vec3 &b,glm::vec3 &c, int depth){
    std::cout<<"depth: "<<depth<<std::endl;
    if(depth==0) return std::vector<vec3>();

    auto inner = inner_triangle(a,b,c, depth);
    if(depth==1)
        return inner;
    
    depth--;
    auto vertices = std::vector<vec3>();
    vec3& inner_a=inner[0];
    vec3& inner_b=inner[1];
    vec3& inner_c=inner[2];
    auto top_triangle = fractal(inner_b, inner_a , c, depth);
    auto left_triangle = fractal(a, inner_c , inner_b, depth);
    auto right_triangle = fractal(inner_c, b , inner_a, depth);

    vertices.insert(std::end(vertices), std::begin(top_triangle), std::end(top_triangle));
    vertices.insert(std::end(vertices), std::begin(right_triangle), std::end(right_triangle));
    vertices.insert(std::end(vertices), std::begin(left_triangle), std::end(left_triangle));
    vertices.insert(std::end(vertices), std::begin(inner), std::end(inner));

    return vertices;
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
    
    std::vector<float> vertices_origin = {
        // positions        // colors           // texture coords
        -1.f, -1.f, 0.0f, 0.0f, 0.0f, 1.0f,   -0.5f, -0.3f,   // bottom left
        1.f, -1.f, 0.0f,  0.0f, 1.0f, 0.0f,   1.5f, -0.3f,   // bottom right
        0.f,  1.f, 0.0f, 1.0f, 1.0f, 0.0f,   0.5f, 1.7f    // top mid  
    };
    auto indices = std::vector<unsigned int>();

    auto vertices = std::vector<float>();
    int stride=8;
    vec3 a = vec3(-1.f, -1.f, 0.0f);
    vec3 b = vec3(1.f, -1.f, 0.0f);
    vec3 c = vec3(0.f,  1.f, 0.0f);
    std::cout<<"a b c: "<<std::to_string(a)<<std::to_string(b)<<std::to_string(c)<<std::endl;
    auto inner=fractal(a,b,c,6);
    std::cout<<"inner has "<<inner.size()<<" vertices:\n"<<std::to_string(inner)<<std::endl;
    for(int i=0;i<inner.size();i++){
        vertices.push_back(inner[i].x);
        vertices.push_back(inner[i].y);
        vertices.push_back(inner[i].z);
        //color
        float rad1=glm::radians((i%6) * 60.f);
        float rad2=glm::radians((i%18) * 20.f);
        float rad3=glm::radians((i%36) * 10.f);
        vertices.push_back(0.5*sin(rad1)+0.5f);
        vertices.push_back(0.5*sin(rad2)+0.5f);
        vertices.push_back(0.5*sin(rad3)+0.5f);
        //u,v
        if(i%3 == 0){
            //v= a = top right
            vertices.push_back(-0.5f);
            vertices.push_back(1.2f);
        }else if (i%3 == 1){
            //v= b = top left
            vertices.push_back(1.5f);
            vertices.push_back(1.2f);
        }else{
            //v = c = bottom mid
            vertices.push_back(0.5f);
            vertices.push_back(-0.7f);
        }

        indices.push_back(i);
    }
    std::cout<<"indices:\n"<<std::to_string(indices)<<std::endl;
    std::cout<<"vertices:\n"<<std::to_string(vertices,3)<<std::endl;

 

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    //initializing VBO, VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    //Element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    // texture cordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

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
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glm::mat4 trans(1.0f);
        // float rads=glm::radians(fmodf(currentFrame*160,360));
        // float rad2=glm::radians(fmodf(currentFrame*40,360));
        // trans = glm::translate(trans, glm::vec3(fmodf(currentFrame/10.f,2.0f)-1.f,sin(rad2),0.0f));
        shader2.Use();
        shader2.setMat4("transform", trans);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()*3/8);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

