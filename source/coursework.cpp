#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);

// Create camera object
Camera camera(glm::vec3(2.0f, 5.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// Object struct
struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};

// Frame timer
float previousTime = 0.0f; // time of previous iteration of the loop
float deltaTime = 0.0f;    // time elapsed since last iteration of the loop

int main(void)
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow *window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework - Callum Newsome", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /**
     * Rectangle object
     */
    // Define vertices
    const float width = 2.0f;
    const float height = 1.0f;
    const float depth = 3.0f;

    const float vertices[] = {
        // front
        -width,
        -height,
        depth,
        width,
        -height,
        depth,
        width,
        height,
        depth,
        -width,
        -height,
        depth,
        width,
        height,
        depth,
        -width,
        height,
        depth,

        // right
        width,
        -height,
        depth,
        width,
        -height,
        -depth,
        width,
        height,
        -depth,
        width,
        -height,
        depth,
        width,
        height,
        -depth,
        width,
        height,
        depth,

        // back
        width,
        -height,
        -depth,
        -width,
        -height,
        -depth,
        -width,
        height,
        -depth,
        width,
        -height,
        -depth,
        -width,
        height,
        -depth,
        width,
        height,
        -depth,

        // left
        -width,
        -height,
        -depth,
        -width,
        -height,
        depth,
        -width,
        height,
        depth,
        -width,
        -height,
        -depth,
        -width,
        height,
        depth,
        -width,
        height,
        -depth,

        // bottom
        -width,
        -height,
        -depth,
        width,
        -height,
        -depth,
        width,
        -height,
        depth,
        -width,
        -height,
        -depth,
        width,
        -height,
        depth,
        -width,
        -height,
        depth,

        // top
        -width,
        height,
        depth,
        width,
        height,
        depth,
        width,
        height,
        -depth,
        -width,
        height,
        depth,
        width,
        height,
        -depth,
        -width,
        height,
        -depth,
    };

    // Define texture coordinates
    const float textCoords[] = {
        // front
        0.0f,
        0.0f, // vertex co-ordinates are the same for each side
        1.0f,
        0.0f, // of the object so repeat every six vertices
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        // right
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        // back
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        // left
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        // bottom
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        // top
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
    };

    // Define indices
    unsigned int indices[] = {
        0, 1, 2, 3, 4, 5,       // front
        6, 7, 8, 9, 10, 11,     // right
        12, 13, 14, 15, 16, 17, // back
        18, 19, 20, 21, 22, 23, // left
        24, 25, 26, 27, 28, 29, // bottom
        30, 31, 32, 33, 34, 35  // top
    };

    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    unsigned int vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Texture buffer
    unsigned int textCoordsBuffer;
    glGenBuffers(1, &textCoordsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), textCoords, GL_STATIC_DRAW);

    unsigned int elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Compile shader program
    unsigned int shaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    // Load the textures
    unsigned int texture;
    texture = loadTexture("../assets/tex.jpg");

    // Send the texture uniforms to the fragment shader
    glUseProgram(shaderID);
    unsigned int textureID;
    textureID = glGetUniformLocation(shaderID, "texture");
    glUniform1i(textureID, 0);

    // Object positions
    glm::vec3 objectPositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(4.0f, 0.0f, 0.0f),
        glm::vec3(6.0f, 0.0f, 0.0f),

        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(2.0f, 2.0f, 0.0f),
        glm::vec3(4.0f, 2.0f, 0.0f),
        glm::vec3(6.0f, 2.0f, 0.0f),

        glm::vec3(0.0f, 4.0f, 0.0f),
        glm::vec3(2.0f, 4.0f, 0.0f),
        glm::vec3(4.0f, 4.0f, 0.0f),
        glm::vec3(6.0f, 4.0f, 0.0f),

        glm::vec3(0.0f, 6.0f, 0.0f),
        glm::vec3(2.0f, 6.0f, 0.0f),
        glm::vec3(4.0f, 6.0f, 0.0f),
        glm::vec3(6.0f, 6.0f, 0.0f),
    };
    int positionsLength = sizeof(objectPositions) / sizeof(objectPositions[0]);

    std::vector<Object> objects;

    // Add positions to objects vector
    Object object;
    object.name = "rectangle";
    object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
    object.angle = Maths::radians(20.0f);

    for (unsigned int i = 0; i < positionsLength; i++)
    {
        object.position = objectPositions[i];
        objects.push_back(object);
    }

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;

        // Get inputs
        keyboardInput(window);

        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Send vertex buffer object
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        // Send texture coords buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

        // Calculate the view and projection matrix
        camera.calculateMatrices();

        for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 model = translate * scale;

            // Calculate the MVP matrix and send to shader
            glm::mat4 MVP = camera.calculateMVPFromModel(model);
            unsigned int MVPID = glGetUniformLocation(shaderID, "MVP");
            glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);

            // Draw triangles
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteBuffers(1, &textCoordsBuffer);
    glDeleteProgram(shaderID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /**
     * Speed
     */
    float baseSpeed = 5.0f;

    // Increase speed when holding shift
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        baseSpeed *= 3.0f;

    /**
     * Movement
     */
    // Forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eyePos += baseSpeed * deltaTime * camera.cameraFront;

    // Back
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eyePos -= baseSpeed * deltaTime * camera.cameraFront;

    // Left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eyePos -= baseSpeed * deltaTime * camera.cameraRight;

    // Right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eyePos += baseSpeed * deltaTime * camera.cameraRight;

    // Up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.eyePos += baseSpeed * deltaTime * camera.cameraUp;

    // Down
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.eyePos -= baseSpeed * deltaTime * camera.cameraUp;
}
