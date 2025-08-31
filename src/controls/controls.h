#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class Controls {
public:
    Controls();
    ~Controls();
    void update(float deltaTime, GLFWwindow* window);
    glm::vec3 getThrust();
    void setTargetPosition(const glm::vec3& pos);
    glm::vec3 getTargetPosition();
    void logData(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& thrust);
private:
    glm::vec3 thrust;
    glm::vec3 targetPosition;
    float pidKp, pidKi, pidKd;
    glm::vec3 integral;
    glm::vec3 previousError;
    std::vector<std::string> logBuffer;
    void saveLogToFile();
};

#endif