#include "mission.h"
#include <iostream>

Mission::Mission() : currentRingIndex(0), missionComplete(false) {}

Mission::~Mission() {}

void Mission::init() {
    // Create multiple rings in a path
    ringPositions = {
        glm::vec3(3, 1, 0),
        glm::vec3(6, 2, 3),
        glm::vec3(9, 3, 0),
        glm::vec3(12, 2, -3),
        glm::vec3(15, 1, 0)
    };
    currentRingIndex = 0;
    missionComplete = false;
    std::cout << "Mission initialized with " << ringPositions.size() << " rings" << std::endl;
}

void Mission::update(const glm::vec3& dronePos) {
    if (missionComplete) return;

    if (checkRingCollision(dronePos, 0.5f)) {
        currentRingIndex++;
        std::cout << "Ring " << currentRingIndex << " passed!" << std::endl;

        if (currentRingIndex >= ringPositions.size()) {
            missionComplete = true;
            std::cout << "Mission Complete!" << std::endl;
        }
    }
}

bool Mission::checkRingCollision(const glm::vec3& dronePos, float radius) {
    if (currentRingIndex >= ringPositions.size()) return false;

    glm::vec3 ringPos = ringPositions[currentRingIndex];
    float distance = glm::length(dronePos - ringPos);
    return distance < (1.0f + radius); // Ring radius 1.0 + drone radius
}

void Mission::reset() {
    currentRingIndex = 0;
    missionComplete = false;
    std::cout << "Mission reset" << std::endl;
}

int Mission::getCurrentRingIndex() {
    return currentRingIndex;
}

int Mission::getTotalRings() {
    return ringPositions.size();
}

bool Mission::isMissionComplete() {
    return missionComplete;
}

std::vector<glm::vec3> Mission::getRingPositions() {
    return ringPositions;
}