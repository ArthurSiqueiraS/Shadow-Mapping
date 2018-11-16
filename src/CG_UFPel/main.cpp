#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define nCubes 6

void renderScene(Shader lightShader, Shader phongShader, vector<Model> models);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// build and compile shaders
// -------------------------

// Set light source position and intensity
glm::vec3 lightPos(-10.0f, 6.0f, -7.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 c[nCubes];

int main()
{
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // Phong shader
    Shader phongShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());
    // Basic shader to render light source
    Shader lightShader(FileSystem::getPath("resources/light.vs").c_str(), FileSystem::getPath("resources/light.fs").c_str());

    // load models
    // -----------
    // Cyborg model
    Model cyborg(FileSystem::getPath("resources/objects/cyborg/cyborg.obj"));
    // Light source 'bulb'
    Model light(FileSystem::getPath("resources/objects/planet/planet.obj"));
    // Cube .objs from https://www.turbosquid.com/
    Model cube(FileSystem::getPath("resources/objects/Cube.obj"));
    Model openCube(FileSystem::getPath("resources/objects/bl.obj"));
    // Floor mat .obj from https://free3d.com/
    Model floorMat(FileSystem::getPath("resources/objects/floor/16859_floor_mat_V2.obj"));
    
    vector<Model> models;
    models.push_back(light);
    models.push_back(cyborg);
    models.push_back(cube);
    models.push_back(openCube);
    models.push_back(floorMat);

    // Set each cube's location in space
    c[0] = glm::vec3(-4.0, 0.0, -8.0);
    c[1] = glm::vec3(-3.0, -1.0, -10.0);
    c[2] = glm::vec3(1.0, -4.0, -11.0);
    c[3] = glm::vec3(4.0, -5.0, -9.0);
    c[4] = glm::vec3(2.0, 1.0, -8.0);
    c[5] = glm::vec3(-3.5, -2.0, -12.0);

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderScene(lightShader, phongShader, models);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void renderScene(Shader lightShader, Shader shader, vector<Model> models) {
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    // render light source bulb
    lightShader.use();
    glm::mat4 model;
    lightShader.setVec3("lightColor", lightColor);
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2));
    lightShader.setMat4("model", model);
    models[0].Draw(lightShader);   

    shader.use();
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.Position); 
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // render cyborg model
    model = glm::mat4();
    shader.setVec3("color", 0.0, 0.0, 0.0);
    model = glm::translate(model, glm::vec3(0.0f, -5.0f, -10.0f)); 
    model = glm::rotate(model, glm::radians(70.0f), glm::vec3(0.0, -1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f)); 
    shader.setMat3("inv", glm::mat3(glm::transpose(glm::inverse(model))));
    shader.setMat4("model", model);
    models[1].Draw(shader);        

    // render the floor
    model = glm::mat4();
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
    model = glm::translate(model, glm::vec3(0.0f, 10.0f, -5.25f)); 
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.2f)); 
    shader.setVec3("color", 0.60, 0.38, 0.09);
    shader.setMat3("inv", glm::mat3(glm::transpose(glm::inverse(model))));
    shader.setMat4("model", model);
    models[4].Draw(shader);

    // render wall
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, 5.0f, -20.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.2f)); 
    shader.setMat3("inv", glm::mat3(glm::transpose(glm::inverse(model))));
    shader.setMat4("model", model);
    models[4].Draw(shader);

    // render open and closed cubes in alternated order
    int open;
    for(int i = 0; i < nCubes; ++i) {
        open = i % 2 == 0;
        model = glm::mat4();
        // model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, open, 0.0));
        model = glm::translate(model, c[i]);
        shader.setVec3("color", glm::vec3(0.30, 0.5, 0.29) + glm::vec3(-open * 0.1, -open * 0.15, open * 0.2));
        shader.setMat3("inv", glm::mat3(glm::transpose(glm::inverse(model))));
        shader.setMat4("model", model);
        if(open) 
            models[2].Draw(shader);
        else
            models[3].Draw(shader);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
