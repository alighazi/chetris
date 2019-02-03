#include "core/boilerplate.h"
#include "core/sigil_board.h"

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{ 
    Boilerplate &boilerplate = Boilerplate::instance();
    SigilBoard board(10,10);
    auto projectionMatrix = glm::ortho(0.f, board.getDimennsions().x, 0.f, board.getDimennsions().y, 0.1f, 10.f);
    auto viewMatrix = glm::mat4(1.f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    while (!glfwWindowShouldClose(boilerplate.window))
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

        boilerplate.update(deltaTime);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        boilerplate.shader->Use();

        boilerplate.shader->setMat4("view", viewMatrix);
        boilerplate.shader->setMat4("projection", projectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boilerplate.texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, boilerplate.texture2);

        board.update(deltaTime, currentFrame);
        board.render(boilerplate.shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(boilerplate.window);
        glfwPollEvents();
    }
    board.~SigilBoard();
    glfwTerminate();
    return 0;
}