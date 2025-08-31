#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class DebugDrawer : public btIDebugDraw {
public:
    DebugDrawer();
    ~DebugDrawer();

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
    void reportErrorWarning(const char* warningString) override;
    void draw3dText(const btVector3& location, const char* textString) override;
    void setDebugMode(int debugMode) override;
    int getDebugMode() const override;

    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    struct Line {
        glm::vec3 start;
        glm::vec3 end;
        glm::vec3 color;
    };

    std::vector<Line> lines;
    GLuint VAO, VBO;
    GLuint shaderProgram;
    int debugMode;

    void createShaderProgram();
    void createBuffers();
};

#endif