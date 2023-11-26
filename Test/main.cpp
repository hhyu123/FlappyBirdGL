#include <iostream>
#include <cstdlib>
#include <cmath>
#include <GLUT/GLUT.h>

// Define window dimensions
const int windowWidth = 800;
const int windowHeight = 1000;

// Bird parameters
float birdX = 100.0f;
float birdY = windowHeight / 2.0f;
float birdRadius = 20.0f;
float birdVelocity = 0.0f;
float gravity = 0.5f;
float jumpForce = 8.0f;

// Pipe parameters
const int numPipes = 5;
float pipeWidth = 50.0f;
float pipeHeight = 300.0f;
float pipeSpacing = 200.0f;
float pipeVelocity = 5.0f;
float pipes[numPipes] = {0.0f};

int score = 0;
bool gameEnded = false;

// Function to draw a circle
void drawCircle(float x, float y, float radius) {
    const int numSegments = 100;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < numSegments; ++i) {
        float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float xPos = x + radius * std::cos(theta);
        float yPos = y + radius * std::sin(theta);
        glVertex2f(xPos, yPos);
    }
    glEnd();
}

// Function to draw a rectangle
void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Function to draw the bird
void drawBird() {
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow color
    drawCircle(birdX, birdY, birdRadius);
}

// Function to draw a pipe
void drawPipe(float x) {
    glColor3f(0.0f, 1.0f, 0.0f);  // Green color
    drawRectangle(x, 0.0f, pipeWidth, windowHeight - pipeSpacing);
    drawRectangle(x, windowHeight - pipeSpacing + pipeHeight, pipeWidth, windowHeight);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' && !gameEnded) {
        birdVelocity = jumpForce;
    }
}

// Function to update the game state
void update() {
    if (!gameEnded) {
        // Update bird position based on velocity
        birdY += birdVelocity;

        // Apply gravity to bird velocity
        birdVelocity -= gravity;

        // Check for collision with top and bottom boundaries
        if (birdY + birdRadius > windowHeight || birdY - birdRadius < 0) {
            std::cout << "Game Over!" << std::endl;
            gameEnded = true;
        }

        // Update pipe positions
        for (int i = 0; i < numPipes; ++i) {
            pipes[i] -= pipeVelocity;

            // Check for collision with bird
            if (birdX + birdRadius > pipes[i] && birdX - birdRadius < pipes[i] + pipeWidth) {
                if (birdY - birdRadius < windowHeight - pipeSpacing || birdY + birdRadius > windowHeight - pipeSpacing + pipeHeight) {
                    std::cout << "Game Over!" << std::endl;
                    gameEnded = true;
                }
            }

            // If a pipe goes off-screen, reset its position and increase the score
            if (pipes[i] + pipeWidth < 0) {
                pipes[i] = windowWidth;
                score++;
                std::cout << "Score: " << score << std::endl;
            }
        }
    }

    glutPostRedisplay();  // Request a redraw
}

// Function to draw the scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBird();

    // Draw pipes
    for (int i = 0; i < numPipes; ++i) {
        drawPipe(pipes[i]);
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Flappy Bird");

    glOrtho(0.0, windowWidth, 0.0, windowHeight, -1.0, 1.0);
    glutDisplayFunc(drawScene);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboard);

    // Initialize pipe positions
    for (int i = 0; i < numPipes; ++i) {
        pipes[i] = windowWidth + i * pipeSpacing;
    }

    glutMainLoop();

    return 0;
}
