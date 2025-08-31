#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "debug_drawer.h"

class Physics {
public:
    Physics();
    ~Physics();
    bool init();
    void step(float deltaTime);
    btRigidBody* getDroneBody();
    void applyThrust(const glm::vec3& force);
    glm::vec3 getDronePosition();
    glm::vec3 getDroneVelocity();
    glm::vec3 getRingPosition();
    void resetDrone();
    void renderDebug(const glm::mat4& view, const glm::mat4& projection);
    void toggleDebugMode();
    bool isDebugModeEnabled() const;
private:
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btRigidBody* droneBody;
    btRigidBody* groundBody;
    btRigidBody* ringBody;
    btCollisionShape* groundShape;
    btCollisionShape* droneShape;
    btCollisionShape* ringShape;
    btMotionState* groundMotionState;
    btMotionState* droneMotionState;
    btMotionState* ringMotionState;
    DebugDrawer* debugDrawer;
};

#endif