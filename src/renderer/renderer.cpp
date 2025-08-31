#include "renderer.h"
#include <iostream>
#include <fstream>

Renderer::Renderer() {}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &torusVAO);
    glDeleteBuffers(1, &torusVBO);
    glDeleteProgram(shaderProgram);
}

bool Renderer::init() {
    try {
        // Create and bind a default VAO for macOS core profile compatibility
        GLuint defaultVAO;
        glGenVertexArrays(1, &defaultVAO);
        glBindVertexArray(defaultVAO);

        createShaderProgram();
        createGroundPlane();
        createCube();
        createTorus();

        // Set projection matrix
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Set initial view matrix
        view = glm::lookAt(glm::vec3(0, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        // Initialize positions
        dronePosition = glm::vec3(0, 1, 0);
        ringPositions = {glm::vec3(3, 1, 0)}; // Default single ring

        // Initialize camera parameters
        cameraDistance = 5.0f;
        cameraAngle = 0.0f;
        cameraHeight = 2.0f;
        cameraYaw = 0.0f;
        cameraPitch = 0.0f;

        std::cout << "Renderer initialized successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to initialize renderer: " << e.what() << std::endl;
        return false;
    }
}

void Renderer::render() {
    glUseProgram(shaderProgram);

    // Set lighting uniforms
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 1.2f, 1.0f, 2.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos2"), -1.2f, 2.0f, -2.0f); // Secondary light position
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 5.0f, 5.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor2"), 0.7f, 0.8f, 1.0f); // Softer blue fill light
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.8f, 0.8f, 0.9f); // Brighter, slightly blue-tinted

    // Set matrices
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Render ground plane
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // Render sphere (drone)
    glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), dronePosition);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 16 * 16 * 6, GL_UNSIGNED_INT, 0); // Sphere with 16x16 divisions
    glBindVertexArray(0);

    // Render toruses (rings)
    for (const auto& ringPos : ringPositions) {
        glm::mat4 torusModel = glm::translate(glm::mat4(1.0f), ringPos);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(torusModel));
        glBindVertexArray(torusVAO);
        // Calculate actual vertex count from torus generation (each triangle has 3 vertices)
        int numMajor = 16;
        int numMinor = 8;
        int trianglesPerQuad = 2; // 2 triangles per quad
        int verticesPerTriangle = 3;
        int vertexCount = numMajor * numMinor * trianglesPerQuad * verticesPerTriangle;
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }
}

void Renderer::setCameraPosition(float x, float y, float z) {
    view = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Renderer::setCameraTarget(float x, float y, float z) {
    view = glm::lookAt(glm::vec3(0, 5, 5), glm::vec3(x, y, z), glm::vec3(0, 1, 0));
}

void Renderer::setDronePosition(float x, float y, float z) {
    dronePosition = glm::vec3(x, y, z);
}


void Renderer::setRingPositions(const std::vector<glm::vec3>& positions) {
    ringPositions = positions;
}

void Renderer::updateCamera(float deltaTime) {
    // Calculate camera position based on drone position and camera parameters
    float camX = dronePosition.x + cameraDistance * cos(cameraAngle);
    float camZ = dronePosition.z + cameraDistance * sin(cameraAngle);
    float camY = dronePosition.y + cameraHeight;

    // Apply mouse look transformation if yaw/pitch are set
    glm::vec3 cameraPos = glm::vec3(camX, camY, camZ);
    glm::vec3 cameraTarget = dronePosition;

    if (cameraYaw != 0.0f || cameraPitch != 0.0f) {
        // Create rotation matrices for yaw and pitch
        glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), cameraYaw, glm::vec3(0, 1, 0));
        glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), cameraPitch, glm::vec3(1, 0, 0));

        // Apply rotations to camera position relative to target
        glm::vec3 relativePos = cameraPos - cameraTarget;
        relativePos = glm::vec3(yawRotation * glm::vec4(relativePos, 1.0f));
        relativePos = glm::vec3(pitchRotation * glm::vec4(relativePos, 1.0f));

        cameraPos = cameraTarget + relativePos;
    }

    // Update view matrix
    view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0, 1, 0));
}

void Renderer::setCameraDistance(float distance) {
    cameraDistance = distance;
}

void Renderer::setCameraAngle(float angle) {
    cameraAngle = angle;
}

void Renderer::setCameraYaw(float yaw) {
    cameraYaw = yaw;
}

void Renderer::setCameraPitch(float pitch) {
    cameraPitch = pitch;
}

void Renderer::resetCamera() {
    cameraDistance = 5.0f;
    cameraAngle = 0.0f;
    cameraHeight = 2.0f;
    cameraYaw = 0.0f;
    cameraPitch = 0.0f;
}

void Renderer::createShaderProgram() {
    std::string vertexSource = loadShaderSource("assets/shaders/vertex.glsl");
    std::string fragmentSource = loadShaderSource("assets/shaders/fragment.glsl");

    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "ERROR: Failed to load shader sources. Cannot create shader program." << std::endl;
        return;
    }

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);

    // Check for errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    // Check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::createGroundPlane() {
    float vertices[] = {
        // positions          // normals
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::createCube() {
    // Create a smooth sphere for the drone - much more visually appealing and less straining
    std::vector<float> vertices;

    const int stacks = 16;  // latitude divisions
    const int slices = 16;  // longitude divisions
    const float radius = 0.3f; // sphere radius

    for (int i = 0; i <= stacks; ++i) {
        float phi = 3.14159f * i / stacks; // from 0 to π
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * 3.14159f * j / slices; // from 0 to 2π

            // Vertex position
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            // Normal (same as position for unit sphere)
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            // Texture coordinates
            float u = (float)j / slices;
            float v = (float)i / stacks;

            vertices.insert(vertices.end(), {x, y, z, nx, ny, nz, u, v});
        }
    }

    // Generate indices for triangles
    std::vector<unsigned int> indices;
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            unsigned int first = (unsigned int)(i * (slices + 1) + j);
            unsigned int second = first + slices + 1;

            // Two triangles per quad
            indices.insert(indices.end(), {first, second, first + 1});
            indices.insert(indices.end(), {second, second + 1, first + 1});
        }
    }

    // Create VAO and VBO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &torusVBO); // Reuse torusVBO for indices

    glBindVertexArray(cubeVAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::createTorus() {
    std::vector<float> vertices;
    float R = 1.0f; // major radius
    float r = 0.3f; // minor radius
    int numMajor = 16;
    int numMinor = 8;

    for (int i = 0; i < numMajor; ++i) {
        float theta = 2.0f * 3.14159f * i / numMajor;
        float nextTheta = 2.0f * 3.14159f * (i + 1) / numMajor;

        for (int j = 0; j < numMinor; ++j) {
            float phi = 2.0f * 3.14159f * j / numMinor;
            float nextPhi = 2.0f * 3.14159f * (j + 1) / numMinor;

            // First triangle
            vertices.push_back((R + r * cos(phi)) * cos(theta));
            vertices.push_back(r * sin(phi));
            vertices.push_back((R + r * cos(phi)) * sin(theta));
            vertices.push_back(cos(theta) * cos(phi));
            vertices.push_back(sin(phi));
            vertices.push_back(sin(theta) * cos(phi));

            vertices.push_back((R + r * cos(nextPhi)) * cos(theta));
            vertices.push_back(r * sin(nextPhi));
            vertices.push_back((R + r * cos(nextPhi)) * sin(theta));
            vertices.push_back(cos(theta) * cos(nextPhi));
            vertices.push_back(sin(nextPhi));
            vertices.push_back(sin(theta) * cos(nextPhi));

            vertices.push_back((R + r * cos(phi)) * cos(nextTheta));
            vertices.push_back(r * sin(phi));
            vertices.push_back((R + r * cos(phi)) * sin(nextTheta));
            vertices.push_back(cos(nextTheta) * cos(phi));
            vertices.push_back(sin(phi));
            vertices.push_back(sin(nextTheta) * cos(phi));

            // Second triangle
            vertices.push_back((R + r * cos(nextPhi)) * cos(theta));
            vertices.push_back(r * sin(nextPhi));
            vertices.push_back((R + r * cos(nextPhi)) * sin(theta));
            vertices.push_back(cos(theta) * cos(nextPhi));
            vertices.push_back(sin(nextPhi));
            vertices.push_back(sin(theta) * cos(nextPhi));

            vertices.push_back((R + r * cos(nextPhi)) * cos(nextTheta));
            vertices.push_back(r * sin(nextPhi));
            vertices.push_back((R + r * cos(nextPhi)) * sin(nextTheta));
            vertices.push_back(cos(nextTheta) * cos(nextPhi));
            vertices.push_back(sin(nextPhi));
            vertices.push_back(sin(nextTheta) * cos(nextPhi));

            vertices.push_back((R + r * cos(phi)) * cos(nextTheta));
            vertices.push_back(r * sin(phi));
            vertices.push_back((R + r * cos(phi)) * sin(nextTheta));
            vertices.push_back(cos(nextTheta) * cos(phi));
            vertices.push_back(sin(phi));
            vertices.push_back(sin(nextTheta) * cos(phi));
        }
    }

    glGenVertexArrays(1, &torusVAO);
    glGenBuffers(1, &torusVBO);
    glBindVertexArray(torusVAO);
    glBindBuffer(GL_ARRAY_BUFFER, torusVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::string Renderer::loadShaderSource(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open shader file: " << path << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (content.empty()) {
        std::cerr << "ERROR: Shader file is empty or failed to read: " << path << std::endl;
        return "";
    }

    std::cout << "Successfully loaded shader: " << path << " (" << content.length() << " characters)" << std::endl;
    return content;
}