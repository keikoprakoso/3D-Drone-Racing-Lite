#ifndef MISSION_H
#define MISSION_H

#include <vector>
#include <glm/glm.hpp>

class Mission {
public:
    Mission();
    ~Mission();
    void init();
    void update(const glm::vec3& dronePos);
    bool checkRingCollision(const glm::vec3& dronePos, float radius);
    void reset();
    int getCurrentRingIndex();
    int getTotalRings();
    bool isMissionComplete();
    std::vector<glm::vec3> getRingPositions();
private:
    std::vector<glm::vec3> ringPositions;
    int currentRingIndex;
    bool missionComplete;
};

#endif