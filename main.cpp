#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>

// Vertex data for a flat rectangle
constexpr float vertices[] = {
    -0.1f, -0.01f,
    0.1f, -0.01f,
    0.1f, 0.01f,
    -0.1f, 0.01f
};

// Rectangle position
float posX = 0.0f;
float posY = -0.8f;

// Ball properties
float ballX = 0.0f;
float ballY = 0.0f;
float ballRadius = 0.02f;
float ballVelocityX = 0.006f;
float ballVelocityY = -0.018f;

void renderRectangle()
{
    // Adjusted vertex data based on position
    float adjustedVertices[] = {
        vertices[0] + posX, vertices[1] + posY,
        vertices[2] + posX, vertices[3] + posY,
        vertices[4] + posX, vertices[5] + posY,
        vertices[6] + posX, vertices[7] + posY
    };

    // Render the rectangle
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, adjustedVertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void renderBall()
{
    const int numSegments = 50; // Reduced number of segments
    float angleStep = 2.0f * 3.14f / numSegments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ballX, ballY);
    for (int i = 0; i <= numSegments; ++i)
    {
        float angle = i * angleStep;
        float x = ballX + cos(angle) * ballRadius;
        float y = ballY + sin(angle) * ballRadius;
        glVertex2f(x, y);
    }
    glEnd();
}

void processInput(GLFWwindow* window)
{
    constexpr float moveSpeed = 0.01f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        posX -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        posX += moveSpeed;

    if (posX < -1.0f)
        posX = -1.0f;
    if (posX > 1.0f)
        posX = 1.0f;
}

void resetPosition()
{
    posX = 0.0f;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void updateBall()
{
    ballX += ballVelocityX;
    ballY += ballVelocityY;

    // Check for collision with window edges
    if (ballX - ballRadius < -1.0f || ballX + ballRadius > 1.0f)
    {
        ballVelocityX = -ballVelocityX;
    }
    if (ballY + ballRadius > 1.0f)
    {
        ballVelocityY = -ballVelocityY;
    }

    // Check for collision with rectangle
    if (ballY - ballRadius < posY + 0.01f && ballY - ballRadius > posY - 0.01f &&
        ballX > posX - 0.1f && ballX < posX + 0.1f)
    {
        ballVelocityY = -ballVelocityY;
    }

    // Check if ball hits the bottom of the window
    if (ballY - ballRadius < -1.0f)
    {
        // Reset ball position
        ballX = 0.0f;
        ballY = 0.5f;
        ballVelocityX = 0.006f;
        ballVelocityY = -0.018f;
    }
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

    // Enable V-Sync
    glfwSwapInterval(1);

    glViewport(0, 0, 1600, 1000);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        glfwPollEvents();
        processInput(window);

        // Update ball position
        updateBall();

        // Rendering code
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Reset position
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            resetPosition();

        // Render the rectangle
        renderRectangle();

        // Render the ball
        renderBall();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}