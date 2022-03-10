// ����� ������� 
// ������ �8�-305�-18
// ������� 

#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/normal.hpp>
#include <gtx/norm.hpp>

#include "ShaderLoader.h"
#include "Camera.h"

#include "Cube.h"
#include "Cylinder.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void sinusToggleCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
glm::quat lookAt(glm::vec3 direction, glm::vec3 desiredUp);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(1.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

int rotate_flat = 90;
int rotate_vert = 0;

float a = 1;
float b = 1;
float c = 1;

bool sinus_light = false;

float up_factor = 0.5;
float down_factor = -0.5;

int x_angle = 0;
int y_angle = 0;
int z_angle = 0;

// colours
glm::vec3 backgroundColour(0.1f, 0.1f, 0.1f);
glm::vec3 objectColour(1.0f, 0.3f, 0.31f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// cylinder parameters
GLfloat radius = 0.5f;
GLfloat height = 0.3;
GLint numOfSectors = 360;
glm::vec3 center(0, 0, 0);
GLfloat sectorStep = 1;
bool wireframe = false;
bool startRotation = false;
glm::mat4 cylinderRotation = glm::mat4(1.0f);
int angle = 0;

// lightingS
glm::vec3 lightPos(3.0f, 1.0f, 0.0f);
glm::vec3 rotation2Light;

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
    glfwSetKeyCallback(window, sinusToggleCallback);
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
    Shader lightingShader("./shaders/Lighting.vert", "./shaders/Lighting.frag");
    Shader lightCubeShader("./shaders/LightCube.vert", "./shaders/LightCube.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    /*Ellipsoid ellipsoid(a, b, c, up_factor, down_factor, layer_count, point_count);
    std::vector<float> ellipsoid_polygons = ellipsoid.GetTriangles();*/
    Cube cube;
    std::vector<float> cube_indices = cube.GetTriangles();

    Cylinder cylinder(center, height, radius, numOfSectors);
    std::vector<GLfloat> cylinder_indices = cylinder.getIndices();
    
    unsigned int cylinderVBO, cylinderVAO;
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cylinder_indices.size(), cylinder_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(cylinderVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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

        cylinder.setNumOfSectors(numOfSectors);
        cylinder.setHeight(height);
        cylinder.setRadius(radius);

        // be sure to activate shader when setting uniforms/drawing objects
        if (cylinder.isChanged()) {
            cylinder_indices = cylinder.getIndices();
            glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cylinder_indices.size(), cylinder_indices.data(), GL_DYNAMIC_DRAW);

        }

        lightingShader.use();
        glm::vec3 lightColor = {1.f,1.f,1.f};
        if (sinus_light) {
            lightColor.x = std::sin(glfwGetTime() * 2.f);
            lightColor.y = std::sin(glfwGetTime() * .7f);
            lightColor.z = std::sin(glfwGetTime() * 1.3f);
        }

        lightingShader.setVec3("objectColor", objectColour);
        lightingShader.setVec3("lightColor", lightColor);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        if (startRotation)
        {
            angle = (angle + 2) % 360;
            model *= cylinderRotation;
            glm::vec4 rotateVec4(center - lightPos, 0);
            glm::vec3 rotateVec3(rotateVec4 * cylinderRotation);
            model = glm::rotate(model, glm::radians((float)angle), rotateVec3);
            cylinderRotation = glm::mat4(1.0f);
        }
        lightingShader.setMat4("model", model);

        // render the cylinder
        glBindVertexArray(cylinderVBO);

        if (wireframe)
        {
            glEnable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            lightingShader.setVec3("objectColor", backgroundColour);
            glDrawArrays(GL_TRIANGLES, 0, cylinder_indices.size() / 6);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0, 1.0);
            lightingShader.setVec3("objectColor", objectColour);
            glDrawArrays(GL_TRIANGLES, 0, cylinder_indices.size() / 6);
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, cylinder_indices.size() / 6);
        }

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        float len = lightPos.length();
        float teta = glm::radians<float>(rotate_flat);
        float fi = glm::radians<float>(rotate_vert);
        lightPos = {len * std::sin(teta) * std::cos(fi),
                    len * std::sin(teta) * std::sin(fi),
                    len * std::cos(teta)};
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, cylinder_indices.size() / 6);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
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

    // change radius
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        radius = std::max(radius - 0.01, -1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        radius = std::min(radius + 0.01, 1.0);
    }
    // change height
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        height = std::min(height + 0.01, 4.0);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        height = std::max(height - 0.01, -4.0);
    }
    // rotate light source
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rotate_flat = (rotate_flat + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        rotate_flat = (rotate_flat - 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        rotate_vert = (rotate_vert + 2) % 360;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        rotate_vert = (rotate_vert - 2) % 360;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        sectorStep = 1;
        std::cout << "step " << sectorStep << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        sectorStep = 5;
        std::cout << "step " << sectorStep << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        sectorStep = 10;
        std::cout << "step " << sectorStep << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        if (numOfSectors < 360 && (numOfSectors + sectorStep) <= 360)
        {
            numOfSectors += sectorStep;
            std::cout << "point count " << numOfSectors << std::endl;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (numOfSectors > 1 && (numOfSectors - sectorStep) >= 1)
        {
            numOfSectors -= sectorStep;
            std::cout << "point count " << numOfSectors << std::endl;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        wireframe = !wireframe;

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

    // execute order 14
    //-----------------
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cylinderRotation *= glm::inverse(glm::lookAt(center, lightPos, glm::vec3(0,1,0)));
        startRotation = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
    {
        startRotation = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void sinusToggleCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        return;
    }
    if (key == GLFW_KEY_C) {
        sinus_light = !sinus_light;
    }
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

glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = dot(start, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f) {
        // special case when vectors in opposite directions :
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        // This implementation favors a rotation around the Up axis,
        // since it's often what you want to do.
        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return angleAxis(glm::radians(180.0f), rotationAxis);
    }

    // Implementation from Stan Melax's Game Programming Gems 1 article
    rotationAxis = cross(start, dest);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return glm::quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

glm::quat lookAt(glm::vec3 direction, glm::vec3 desiredUp) 
{

    if (length2(direction) < 0.0001f)
        return glm::quat();

    // Recompute desiredUp so that it's perpendicular to the direction
    // You can skip that part if you really want to force desiredUp
    glm::vec3 right = cross(direction, desiredUp);
    desiredUp = cross(right, direction);

    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    glm::quat rot1 = rotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);
    // Because of the 1rst rotation, the up is probably completely screwed up. 
    // Find the rotation between the "up" of the rotated object, and the desired up
    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat rot2 = rotationBetweenVectors(newUp, desiredUp);

    // Apply them
    return rot2 * rot1; // remember, in reverse order.
}