#include "debug_drawer.h"
#include <iostream>
#include <fstream>

DebugDrawer::DebugDrawer() : debugMode(DBG_DrawWireframe) {
    createShaderProgram();
    createBuffers();
}

DebugDrawer::~DebugDrawer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    lines.push_back({
        glm::vec3(from.x(), from.y(), from.z()),
        glm::vec3(to.x(), to.y(), to.z()),
        glm::vec3(color.x(), color.y(), color.z())
    });
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
    // Draw a small line to show contact point
    btVector3 to = PointOnB + normalOnB * distance;
    drawLine(PointOnB, to, color);
}

void DebugDrawer::reportErrorWarning(const char* warningString) {
    std::cerr << "Bullet Physics Warning: " << warningString << std::endl;
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString) {
    // Text rendering not implemented for simplicity
}

void DebugDrawer::setDebugMode(int debugMode) {
    this->debugMode = debugMode;
}

int DebugDrawer::getDebugMode() const {
    return debugMode;
}

void DebugDrawer::render(const glm::mat4& view, const glm::mat4& projection) {
    if (lines.empty()) return;

    glUseProgram(shaderProgram);

    // Set uniforms
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    // Prepare vertex data
    std::vector<float> vertices;
    for (const auto& line : lines) {
        // Start point
        vertices.push_back(line.start.x);
        vertices.push_back(line.start.y);
        vertices.push_back(line.start.z);
        vertices.push_back(line.color.r);
        vertices.push_back(line.color.g);
        vertices.push_back(line.color.b);

        // End point
        vertices.push_back(line.end.x);
        vertices.push_back(line.end.y);
        vertices.push_back(line.end.z);
        vertices.push_back(line.color.r);
        vertices.push_back(line.color.g);
        vertices.push_back(line.color.b);
    }

    // Update VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // Draw lines
    glDrawArrays(GL_LINES, 0, lines.size() * 2);

    // Clear lines for next frame
    lines.clear();
}

void DebugDrawer::createShaderProgram() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 color;

        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            color = aColor;
            gl_Position = projection * view * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 color;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(color, 1.0);
        }
    )";

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void DebugDrawer::createBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}