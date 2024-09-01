#include <iostream>
#include <GLFW/glfw3.h>

// Vertex data for a triangle
constexpr float vertices[] = {
    -0.1f, -0.1f,
     0.1f, -0.1f,
     0.0f,  0.2f
};

// Triangle position
float posX = 0.0f;
float posY = 0.0f;

void renderTriangle()
{
    // Adjusted vertex data based on position
    const float adjustedVertices[] = {
        vertices[0] + posX, vertices[1] + posY,
        vertices[2] + posX, vertices[3] + posY,
        vertices[4] + posX, vertices[5] + posY
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, adjustedVertices);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void processInput(GLFWwindow* window)
{
    constexpr float moveSpeed = 0.001f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        posY += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        posY -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        posX -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        posX += moveSpeed;
}

void resetPosition()
{
    posX = 0.0f;
    posY = 0.0f;
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 1000, "Game Engine", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glViewport(0, 0, 1600, 1000);

    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        glfwPollEvents();
        processInput(window);

        // Rendering code
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Reset position
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            resetPosition();

        // Render the triangle
        renderTriangle();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}