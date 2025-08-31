#include "physics.h"
#include <iostream>

Physics::Physics() : debugDrawer(nullptr) {}

Physics::~Physics() {
    if (dynamicsWorld) {
        // Remove rigid bodies from world before deleting
        if (groundBody) dynamicsWorld->removeRigidBody(groundBody);
        if (droneBody) dynamicsWorld->removeRigidBody(droneBody);
        if (ringBody) dynamicsWorld->removeRigidBody(ringBody);
        delete dynamicsWorld;
    }
    if (solver) {
        delete solver;
    }
    if (overlappingPairCache) {
        delete overlappingPairCache;
    }
    if (dispatcher) {
        delete dispatcher;
    }
    if (collisionConfiguration) {
        delete collisionConfiguration;
    }
    // Delete rigid bodies
    if (groundBody) delete groundBody;
    if (droneBody) delete droneBody;
    if (ringBody) delete ringBody;
    // Delete shapes
    if (groundShape) delete groundShape;
    if (droneShape) delete droneShape;
    if (ringShape) delete ringShape;
    // Delete motion states
    if (groundMotionState) delete groundMotionState;
    if (droneMotionState) delete droneMotionState;
    if (ringMotionState) delete ringMotionState;
}

bool Physics::init() {
    try {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    // Initialize debug drawer
    debugDrawer = new DebugDrawer();
    dynamicsWorld->setDebugDrawer(debugDrawer);
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);

    // Create ground
    groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
    groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    groundBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundBody);

    // Create drone - spherical shape for smooth collision
    droneShape = new btSphereShape(0.3f);
    droneMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
    btScalar mass = 1;
    btVector3 droneInertia(0, 0, 0);
    droneShape->calculateLocalInertia(mass, droneInertia);
    btRigidBody::btRigidBodyConstructionInfo droneRigidBodyCI(mass, droneMotionState, droneShape, droneInertia);
    droneBody = new btRigidBody(droneRigidBodyCI);
    dynamicsWorld->addRigidBody(droneBody);

    // Create ring
    ringShape = new btCylinderShape(btVector3(1, 0.1, 1));
    ringMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(3, 1, 0)));
    btRigidBody::btRigidBodyConstructionInfo ringRigidBodyCI(0, ringMotionState, ringShape, btVector3(0, 0, 0));
    ringBody = new btRigidBody(ringRigidBodyCI);
    dynamicsWorld->addRigidBody(ringBody);

    std::cout << "Physics initialized successfully" << std::endl;
    return true;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to initialize physics: " << e.what() << std::endl;
        return false;
    }
}

void Physics::step(float deltaTime) {
    dynamicsWorld->stepSimulation(deltaTime, 10);
}

btRigidBody* Physics::getDroneBody() {
    return droneBody;
}

void Physics::applyThrust(const glm::vec3& force) {
    droneBody->applyCentralForce(btVector3(force.x, force.y, force.z));
}

glm::vec3 Physics::getDronePosition() {
    btTransform trans;
    droneBody->getMotionState()->getWorldTransform(trans);
    btVector3 pos = trans.getOrigin();
    return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

glm::vec3 Physics::getDroneVelocity() {
    btVector3 vel = droneBody->getLinearVelocity();
    return glm::vec3(vel.getX(), vel.getY(), vel.getZ());
}

glm::vec3 Physics::getRingPosition() {
    btTransform trans;
    ringBody->getMotionState()->getWorldTransform(trans);
    btVector3 pos = trans.getOrigin();
    return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

void Physics::resetDrone() {
    droneBody->setLinearVelocity(btVector3(0, 0, 0));
    droneBody->setAngularVelocity(btVector3(0, 0, 0));
    droneBody->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
}

void Physics::renderDebug(const glm::mat4& view, const glm::mat4& projection) {
    if (debugDrawer && dynamicsWorld) {
        dynamicsWorld->debugDrawWorld();
        debugDrawer->render(view, projection);
    }
}

void Physics::toggleDebugMode() {
    if (debugDrawer) {
        int currentMode = debugDrawer->getDebugMode();
        if (currentMode == 0) {
            debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
        } else {
            debugDrawer->setDebugMode(0);
        }
    }
}

bool Physics::isDebugModeEnabled() const {
    if (debugDrawer) {
        return debugDrawer->getDebugMode() != 0;
    }
    return false;
}