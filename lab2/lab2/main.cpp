// Шимко Василий 
// Группа М8О-305Б-18
// Вариант 14

#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/normal.hpp>

#include "ShaderLoader.h"
#include "Camera.h"

#include "obelisk.h"
#include "Cube.h"
#include "Cylinder.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(2.0f, 2.0f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
glm::vec3 up(0.0f, 1.0f, 0.0f);

int rotate_flat = 90;
int rotate_vert = 0;

float a = 1;
float b = 1;
float c = 1;

int x_angle = 90;
int y_angle = 0;
int z_angle = 0;

// colours
glm::vec3 backgroundColour(0.1f, 0.1f, 0.1f);
glm::vec3 objectColour(1.0f, 0.3f, 0.31f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// parameters
bool isometric = true;
bool ortho_axis = false;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shimko Vasiliy, group M8O-305B-18", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

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
    /*glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);*/

    // build and compile our shader zprogram
    // ------------------------------------
    Shader objectShader("./shaders/object.vert", "./shaders/object.frag");
    Shader axisShader("./shaders/axis.vert", "./shaders/axis.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    Obelisk obelisk;
    std::vector<GLfloat> obelisk_indices = obelisk.getIndices();

    // first, configure the cube's xVAO (and cylinderVBO)
    unsigned int cylinderVBO, cylinderVAO;
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obelisk_indices.size(), obelisk_indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(cylinderVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    float xAxis[] = {
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f, // x  
    };

    float yAxis[] = {
         0.0f, 0.0f, 0.0f,
         0.0f,  1.0f, 0.0f, // y 
    };

    float zAxis[] = {
         0.0f, 0.0f, 0.0f,
         0.0f,  0.0f, 1.0f  // z   
    };

    unsigned int xVBO, xVAO, yVBO, yVAO, zVBO, zVAO;
    glGenVertexArrays(1, &xVAO);
    glGenBuffers(1, &xVBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(xVAO);

    glBindBuffer(GL_ARRAY_BUFFER, xVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(xAxis), xAxis, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // YYYYYYYYYYYYYYYYYY
    glGenVertexArrays(1, &yVAO);
    glGenBuffers(1, &yVBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(yVAO);

    glBindBuffer(GL_ARRAY_BUFFER, yVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yAxis), yAxis, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ZZZZZZZZZZZZZZZZZZZ
    glGenVertexArrays(1, &zVAO);
    glGenBuffers(1, &zVBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(zVAO);

    glBindBuffer(GL_ARRAY_BUFFER, zVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zAxis), zAxis, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.use();

        // view/projection transformations
        
        /*if (isometric)
        {
            camera.Position = glm::vec3(2.0f, 2.0f, 2.0f);
        }
        else
        {
            if (ortho_axis == 1)
            {
                camera.Position = glm::vec3(2.0f, 0.0f, 0.0f);
            }
            if (ortho_axis == 2)
            {
                camera.Position = glm::vec3(0.0f, 2.0f, 0.0f);
                up = glm::vec3(0.0f, 0.0f, -1.0f);
            }
            if (ortho_axis == 3)
            {
                camera.Position = glm::vec3(0.0f, 0.0f, 2.0f);
            }
        }*/

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(0.0f, 0.0f, 0.0f), up);
        objectShader.setMat4("projection", projection);
        objectShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(float(x_angle)), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(float(y_angle)), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(float(z_angle)), glm::vec3(0,0,1));
        model = glm::scale(model, glm::vec3(a,b,c));
        objectShader.setMat4("model", model);

        // render the cylinder
        glBindVertexArray(cylinderVBO);

        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        objectShader.setVec3("objectColor", backgroundColour);
        glDrawArrays(GL_TRIANGLES, 0, obelisk_indices.size() / 3);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        objectShader.setVec3("objectColor", objectColour);
        glDrawArrays(GL_TRIANGLES, 0, obelisk_indices.size() / 3);
        glDisable(GL_POLYGON_OFFSET_FILL);

        axisShader.use();

        model = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(camera.Position, glm::vec3(0.0f, 0.0f, 0.0f), up);
        axisShader.setMat4("projection", projection);
        axisShader.setMat4("view", view);

        axisShader.setMat4("model", model);

        axisShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
        glBindVertexArray(xVBO);
        glDrawArrays(GL_LINES, 0, 2);
        axisShader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
        glBindVertexArray(yVBO);
        glDrawArrays(GL_LINES, 0, 2);
        axisShader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(zVBO);
        glDrawArrays(GL_LINES, 0, 2);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // close programm, when ESC button is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
   
    // rotate cylinder
    //----------------
    // against x axis
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        x_angle = (x_angle + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        x_angle = (x_angle - 2) % 360;
    }
    // against y axis
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        y_angle = (y_angle + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        y_angle = (y_angle - 2) % 360;
    }
    // against z axis
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        z_angle = (z_angle + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        z_angle = (z_angle - 2) % 360;
    }

    // rotate light source
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        rotate_flat = (rotate_flat + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        rotate_flat = (rotate_flat - 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        rotate_vert = (rotate_vert + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        rotate_vert = (rotate_vert - 2) % 360;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
    {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera.Position = glm::vec3(2.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        up = glm::vec3(0.0f, 0.0f, -1.0f);
        camera.Position = glm::vec3(0.0f, 2.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) 
    {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera.Position = glm::vec3(0.0f, 0.0f, 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera.Position = glm::vec3(2.0f, 2.0f, 2.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        glm::vec3 temp = objectColour;
        objectColour = backgroundColour;
        backgroundColour = temp;
    }

    // camera movement 
    //----------------
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

