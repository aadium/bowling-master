#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

// Ball properties
struct Ball {
    float x, y;
    float radius;
    float velocityX, velocityY;
    bool visible;
};

// Bottle properties
struct Bottle {
    float x, y;
    float radius;
    float velocityX, velocityY;
    bool toppled;
    float toppledTime; // Time since the bottle was toppled
};

// Game state
Ball ball = {0.0f, -0.8f, 0.05f, 0.0f, 0.0f, true}; // Initialize ball as visible
std::vector<Bottle> bottles;
int throws = 0;
bool ballInMotion = false;
bool gameOver = false; // Add game over flag
const float trackLeftEdge = -0.5f;
const float trackRightEdge = 0.5f;
const float toppledDuration = 3.0f; // Time in seconds before a toppled bottle disappears
float timeSinceLastBottleDisappeared = 0.0f; // Time since the last bottle disappeared

void initBottles() {
    bottles.clear();
    float startX = 0.0f;
    float startY = 0.8f;
    float spacing = 0.1f;
    int bottleCount = 4;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < bottleCount; ++j) {
            bottles.push_back({startX + (j - bottleCount / 2.0f) * spacing, startY - i * spacing, 0.03f, 0.0f, 0.0f, false, 0.0f});
        }
        bottleCount--;
    }
    ball.visible = true; // Show the ball when bottles are reset
    gameOver = false; // Reset game over flag
}

void renderCircle(float x, float y, float radius) {
    const int numSegments = 50;
    float angleStep = 2.0f * 3.14f / numSegments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float px = x + cos(angle) * radius;
        float py = y + sin(angle) * radius;
        glVertex2f(px, py);
    }
    glEnd();
}

void renderTrackEdges() {
    glBegin(GL_LINES);
    glVertex2f(trackLeftEdge, -1.0f);
    glVertex2f(trackLeftEdge, 1.0f);
    glVertex2f(trackRightEdge, -1.0f);
    glVertex2f(trackRightEdge, 1.0f);
    glEnd();
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !ballInMotion && !gameOver) {
        ball.velocityY = 0.02f;
        ballInMotion = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !ballInMotion && !gameOver) {
        ball.x -= 0.01f;
        if (ball.x - ball.radius < trackLeftEdge) {
            ball.x = trackLeftEdge + ball.radius;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !ballInMotion && !gameOver) {
        ball.x += 0.01f;
        if (ball.x + ball.radius > trackRightEdge) {
            ball.x = trackRightEdge - ball.radius;
        }
    }
}

void updateBall() {
    if (ballInMotion) {
        ball.y += ball.velocityY;
        if (ball.y > 1.0f) {
            ballInMotion = false;
            ball.y = -0.8f;
            ball.velocityY = 0.0f;
            throws++;
            if (throws >= 2) {
                timeSinceLastBottleDisappeared = 0.0f; // Reset the timer
            }
        }
    }
}

void updateBottles(float deltaTime) {
    for (auto& bottle : bottles) {
        bottle.x += bottle.velocityX;
        bottle.y += bottle.velocityY;
        bottle.velocityX *= 0.99f; // Damping
        bottle.velocityY *= 0.99f; // Damping
        if (bottle.toppled) {
            bottle.toppledTime += deltaTime;
        }
    }
    // Remove bottles that have been toppled for longer than the duration
    bottles.erase(std::remove_if(bottles.begin(), bottles.end(), [](const Bottle& bottle) {
        return bottle.toppled && bottle.toppledTime > toppledDuration;
    }), bottles.end());

    // Update the timer if all bottles are gone
    if (bottles.empty() && throws >= 2) {
        ball.visible = false; // Hide the ball when all bottles are gone
        timeSinceLastBottleDisappeared += deltaTime;
        if (timeSinceLastBottleDisappeared > 3.0f) {
            gameOver = true; // Set game over flag
        }
    }
}

void handleCollisions() {
    // Ball and bottle collisions
    for (auto& bottle : bottles) {
        float dx = bottle.x - ball.x;
        float dy = bottle.y - ball.y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < ball.radius + bottle.radius) {
            float angle = atan2(dy, dx);
            float totalVelocity = sqrt(ball.velocityY * ball.velocityY);
            bottle.velocityX = cos(angle) * totalVelocity;
            bottle.velocityY = sin(angle) * totalVelocity;
            bottle.toppled = true;
        }
    }

    // Bottle and bottle collisions
    for (size_t i = 0; i < bottles.size(); ++i) {
        for (size_t j = i + 1; j < bottles.size(); ++j) {
            float dx = bottles[j].x - bottles[i].x;
            float dy = bottles[j].y - bottles[i].y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < bottles[i].radius + bottles[j].radius) {
                float angle = atan2(dy, dx);
                float totalVelocity = sqrt(bottles[i].velocityX * bottles[i].velocityX + bottles[i].velocityY * bottles[i].velocityY);
                bottles[j].velocityX = cos(angle) * totalVelocity;
                bottles[j].velocityY = sin(angle) * totalVelocity;
                bottles[i].toppled = true;
                bottles[j].toppled = true;
            }
        }
    }
}

void renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void renderGame() {
    // Render ball if visible
    if (ball.visible) {
        glColor3f(1.0f, 1.0f, 1.0f); // Reset color to white
        renderCircle(ball.x, ball.y, ball.radius);
    }

    // Render bottles
    for (const auto& bottle : bottles) {
        if (bottle.toppled) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red color for toppled bottles
        } else {
            glColor3f(1.0f, 1.0f, 1.0f); // White color for standing bottles
        }
        renderCircle(bottle.x, bottle.y, bottle.radius);
    }

    // Render track edges
    glColor3f(1.0f, 1.0f, 1.0f); // Reset color to white
    renderTrackEdges();

    // Render number of toppled bottles
    int toppledCount = std::count_if(bottles.begin(), bottles.end(), [](const Bottle& bottle) {
        return bottle.toppled;
    });
    renderText(-0.9f, 0.9f, "Toppled Bottles: " + std::to_string(toppledCount));
}

void renderFinalScore() {
    // Render final score dialog
    glColor3f(1.0f, 1.0f, 1.0f); // White color for text
    renderText(-0.2f, 0.0f, "Final Score: " + std::to_string(throws));
    renderText(-0.1f, -0.2f, "Press ESC to Close");
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 1000, "Bowling Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Enable V-Sync
    glfwSwapInterval(1);

    glViewport(0, 0, 1600, 1000);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    initBottles();

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Input handling
        glfwPollEvents();
        processInput(window);

        // Update game state
        if (!gameOver) {
            updateBall();
            updateBottles(deltaTime);
            handleCollisions();
        }

        // Rendering code
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render game objects or final score dialog
        if (gameOver) {
            renderFinalScore();
        } else {
            renderGame();
        }

        // Swap buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}