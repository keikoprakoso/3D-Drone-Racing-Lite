#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();
    bool init();
    void render();
    void setCameraPosition(float x, float y, float z);
    void setCameraTarget(float x, float y, float z);
    void updateCamera(float deltaTime);
    void setCameraDistance(float distance);
    void setCameraAngle(float angle);
    void setCameraYaw(float yaw);
    void setCameraPitch(float pitch);
    void resetCamera();
    float getCameraDistance() const { return cameraDistance; }
    float getCameraAngle() const { return cameraAngle; }
    float getCameraYaw() const { return cameraYaw; }
    float getCameraPitch() const { return cameraPitch; }
    void setDronePosition(float x, float y, float z);
    void setRingPositions(const std::vector<glm::vec3>& positions);
    const glm::mat4& getViewMatrix() const { return view; }
    const glm::mat4& getProjectionMatrix() const { return projection; }
private:
    GLuint shaderProgram;
    GLuint groundVAO, groundVBO;
    GLuint cubeVAO, cubeVBO;
    GLuint torusVAO, torusVBO;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 dronePosition;
    std::vector<glm::vec3> ringPositions;
    float cameraDistance;
    float cameraAngle;
    float cameraHeight;
    float cameraYaw;
    float cameraPitch;
    void createShaderProgram();
    void createGroundPlane();
    void createCube();
    void createTorus();
    std::string loadShaderSource(const std::string& path);
};

#endif