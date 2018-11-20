// settings
#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

GLFWwindow* window;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool biased = true;
bool clamped = true;
bool shadows = true;
bool animateBulb = true;
bool animations = true;
bool Bpress = false;
bool Cpress = false;
bool Opress = false;
bool Lpress = false;
bool Npress = false;
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

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !Bpress) {
        biased = !biased;
        Bpress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
        Bpress = false;

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !Cpress) {
        clamped = !clamped;
        Cpress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
        Cpress = false;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !Opress) {
        shadows = !shadows;
        Opress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE)
        Opress = false;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !Lpress) {
        animateBulb = !animateBulb;
        Lpress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
        Lpress = false;

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !Npress) {
        animations = !animations;
        Npress = true;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
        Npress = false;
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}