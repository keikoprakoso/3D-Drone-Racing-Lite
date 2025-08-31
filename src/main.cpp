#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "renderer/renderer.h"
#include "physics/physics.h"
#include "controls/controls.h"
#include "mission/mission.h"
#include "mission/mission.h"

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main() {
    // Set GLFW error callback
    glfwSetErrorCallback(glfwErrorCallback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW window hints for OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create a windowed GLFW window with reasonable size
    GLFWwindow* window = glfwCreateWindow(1024, 768, "3D Drone Racing Lite", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable VSync for smooth rendering
    glfwSwapInterval(1);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Validate OpenGL context
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    // Check for required extensions
    if (!GLAD_GL_VERSION_3_3) {
        std::cerr << "OpenGL 3.3 is not supported!" << std::endl;
        return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Enable face culling for better performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Set the viewport
    glViewport(0, 0, 1280, 720);

    // Initialize renderer
    Renderer renderer;
    if (!renderer.init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialize physics
    Physics physics;
    if (!physics.init()) {
        std::cerr << "Failed to initialize physics" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialize controls
    Controls controls;

    // Initialize mission
    Mission mission;
    mission.init();

    // Print control instructions
    std::cout << "\n=== 3D Drone Racing Lite Controls ===" << std::endl;
    std::cout << "Movement: WASD (forward/back/left/right)" << std::endl;
    std::cout << "Vertical: Q (up), E (down)" << std::endl;
    std::cout << "Thrust: SPACE (additional upward force)" << std::endl;
    std::cout << "Camera: Arrow keys (orbit), Right-click + mouse (look around)" << std::endl;
    std::cout << "Reset: R (drone & mission), C (camera)" << std::endl;
    std::cout << "Debug: F1 (physics visualization), F2 (performance info)" << std::endl;
    std::cout << "=====================================\n" << std::endl;

    // Main render loop
    float lastTime = glfwGetTime();
    float fpsUpdateTimer = 0.0f;
    int frameCount = 0;
    float fps = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update FPS counter
        frameCount++;
        fpsUpdateTimer += deltaTime;
        if (fpsUpdateTimer >= 0.5f) { // Update FPS every 0.5 seconds
            fps = frameCount / fpsUpdateTimer;
            frameCount = 0;
            fpsUpdateTimer = 0.0f;
        }

        // Update controls
        controls.update(deltaTime, window);

        // Apply thrust from controls
        physics.applyThrust(controls.getThrust());

        // Enhanced camera controls
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            renderer.setCameraAngle(renderer.getCameraAngle() - 2.0f * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            renderer.setCameraAngle(renderer.getCameraAngle() + 2.0f * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            renderer.setCameraDistance(renderer.getCameraDistance() - 5.0f * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            renderer.setCameraDistance(renderer.getCameraDistance() + 5.0f * deltaTime);
        }
    
        // Mouse look controls (hold right mouse button)
        static bool mouseLookEnabled = false;
        static double lastMouseX = 0.0, lastMouseY = 0.0;
    
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            if (!mouseLookEnabled) {
                // First time enabling mouse look
                glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                mouseLookEnabled = true;
            } else {
                // Update camera based on mouse movement
                double mouseX, mouseY;
                glfwGetCursorPos(window, &mouseX, &mouseY);
    
                float sensitivity = 0.002f;
                float deltaX = (float)(mouseX - lastMouseX) * sensitivity;
                float deltaY = (float)(mouseY - lastMouseY) * sensitivity;
    
                // Update camera angles
                static float cameraYaw = 0.0f;
                static float cameraPitch = 0.0f;
    
                cameraYaw += deltaX;
                cameraPitch -= deltaY; // Inverted Y axis
    
                // Clamp pitch to prevent flipping
                cameraPitch = glm::clamp(cameraPitch, -glm::pi<float>()/2.0f + 0.1f, glm::pi<float>()/2.0f - 0.1f);
    
                renderer.setCameraYaw(cameraYaw);
                renderer.setCameraPitch(cameraPitch);
    
                lastMouseX = mouseX;
                lastMouseY = mouseY;
            }
        } else if (mouseLookEnabled) {
            // Disable mouse look
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseLookEnabled = false;
        }

        // Enhanced keyboard shortcuts
        static bool debugKeyPressed = false;
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !debugKeyPressed) {
            physics.toggleDebugMode();
            debugKeyPressed = true;
            std::cout << "Debug mode " << (physics.isDebugModeEnabled() ? "enabled" : "disabled") << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
            debugKeyPressed = false;
        }

        // Reset drone and mission
        static bool resetKeyPressed = false;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !resetKeyPressed) {
            physics.resetDrone();
            mission.reset();
            resetKeyPressed = true;
            std::cout << "Drone and mission reset!" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
            resetKeyPressed = false;
        }

        // Camera reset
        static bool cameraResetPressed = false;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cameraResetPressed) {
            renderer.resetCamera();
            cameraResetPressed = true;
            std::cout << "Camera reset to default position!" << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
            cameraResetPressed = false;
        }

        // Performance info toggle
        static bool perfInfoPressed = false;
        static bool showPerfInfo = true;
        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !perfInfoPressed) {
            showPerfInfo = !showPerfInfo;
            perfInfoPressed = true;
            std::cout << "Performance info " << (showPerfInfo ? "enabled" : "disabled") << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) {
            perfInfoPressed = false;
        }

        // Step physics
        physics.step(deltaTime);

        // Get drone position and update renderer
        glm::vec3 dronePos = physics.getDronePosition();
        renderer.setDronePosition(dronePos.x, dronePos.y, dronePos.z);

        // Update mission
        mission.update(dronePos);

        // Update renderer with ring positions
        renderer.setRingPositions(mission.getRingPositions());

        // Check for crash (drone below ground)
        if (dronePos.y < 0) {
            mission.reset();
            // Reset drone position
            physics.resetDrone();
        }

        // Log data
        glm::vec3 droneVel = physics.getDroneVelocity();
        glm::vec3 thrust = controls.getThrust();
        controls.logData(dronePos, droneVel, thrust);

        // Update camera
        renderer.updateCamera(deltaTime);

        // Update viewport for resizable window
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Clear the screen with enhanced atmospheric background
        // Create a gradient sky effect based on time
        float time = glfwGetTime();
        float skyR = 0.4f + 0.1f * sin(time * 0.5f);
        float skyG = 0.6f + 0.1f * cos(time * 0.3f);
        float skyB = 0.8f + 0.1f * sin(time * 0.7f);
        glClearColor(skyR, skyG, skyB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        renderer.render();

        // Render physics debug information
        physics.renderDebug(renderer.getViewMatrix(), renderer.getProjectionMatrix());

        // Update window title with enhanced info
        if (showPerfInfo) {
            std::string title = "3D Drone Racing Lite - FPS: " + std::to_string((int)fps);
            if (physics.isDebugModeEnabled()) {
                title += " [DEBUG]";
            }
            title += " | Rings: " + std::to_string(mission.getCurrentRingIndex()) + "/" + std::to_string(mission.getTotalRings());
            glfwSetWindowTitle(window, title.c_str());
        } else {
            glfwSetWindowTitle(window, "3D Drone Racing Lite");
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}