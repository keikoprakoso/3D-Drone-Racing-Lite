#include "controls.h"
#include <iostream>
#include <fstream>
#include <chrono>

Controls::Controls() : thrust(0.0f), targetPosition(0.0f, 5.0f, 0.0f), pidKp(1.0f), pidKi(0.1f), pidKd(0.1f), integral(0.0f), previousError(0.0f) {}

Controls::~Controls() {
    saveLogToFile();
}

void Controls::update(float deltaTime, GLFWwindow* window) {
    // Manual controls
    thrust = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        thrust.y += 15.0f; // upward
        std::cout << "SPACE pressed - upward thrust" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        thrust.z -= 5.0f; // forward
        std::cout << "W pressed - forward thrust" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        thrust.z += 5.0f; // backward
        std::cout << "S pressed - backward thrust" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        thrust.x -= 5.0f; // left
        std::cout << "A pressed - left thrust" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        thrust.x += 5.0f; // right
        std::cout << "D pressed - right thrust" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        thrust.y += 10.0f; // up
        std::cout << "Q pressed - upward thrust" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        thrust.y -= 5.0f; // down (less force for controlled descent)
        std::cout << "E pressed - downward thrust" << std::endl;
    }

    // PID for auto-hover (simple implementation)
    // This would need current position and velocity from physics
    // For now, just manual controls
}

glm::vec3 Controls::getThrust() {
    return thrust;
}

void Controls::setTargetPosition(const glm::vec3& pos) {
    targetPosition = pos;
}

glm::vec3 Controls::getTargetPosition() {
    return targetPosition;
}

void Controls::logData(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& thrust) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string logEntry = std::to_string(time) + "," +
                          std::to_string(position.x) + "," + std::to_string(position.y) + "," + std::to_string(position.z) + "," +
                          std::to_string(velocity.x) + "," + std::to_string(velocity.y) + "," + std::to_string(velocity.z) + "," +
                          std::to_string(thrust.x) + "," + std::to_string(thrust.y) + "," + std::to_string(thrust.z);
    logBuffer.push_back(logEntry);
}

void Controls::saveLogToFile() {
    std::ofstream file("data/logs/drone_log.csv");
    if (file.is_open()) {
        file << "timestamp,pos_x,pos_y,pos_z,vel_x,vel_y,vel_z,thrust_x,thrust_y,thrust_z\n";
        for (const auto& entry : logBuffer) {
            file << entry << "\n";
        }
        file.close();
        std::cout << "Log saved to data/logs/drone_log.csv" << std::endl;
    }
}