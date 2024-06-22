#include "Simulator.h"

#include "../gl_canvas2d.h"
#include "../3D/Primitive.h"
#include "../3D/Perspective.h"

//#define PI 3.1415926535897932384626433832795
#define DEG_90 1.5707963267948966192313216916398
#define RPM_TO_RADS 0.10471975511965977461542144610932 // 2pi/60

Simulator::Simulator(){
    simval = {};
    simval.rpm = 60;
}

SimulationValues& Simulator::getValues(){
    return simval;
}

float time = 0;

void Simulator::update(float delta){
    simval.crankshaftOrigin = Vector3::zero();
    simval.crankshaftLength = 100;
    time += delta * simval.rpm;
    simval.crankshaftAngle = time * RPM_TO_RADS;
    simval.crankshaftDirection = Vector3(0, -sin(simval.crankshaftAngle), cos(simval.crankshaftAngle));
    simval.crankshaftEnd = simval.crankshaftOrigin + (simval.crankshaftDirection * simval.crankshaftLength);

    simval.spacePistonCrankshaft = 10;

    simval.pistonArmLength = simval.crankshaftLength*2 + simval.spacePistonCrankshaft;
    simval.pistonBaseLength = simval.pistonArmLength; // a base vai ficar com um pouco de espaco sobrando em cima. intencional!
    simval.pistonOrigin = simval.crankshaftOrigin;
    simval.pistonOrigin.z += simval.crankshaftLength + simval.spacePistonCrankshaft + simval.pistonArmLength;
    simval.pistonDirection = (simval.crankshaftEnd - simval.pistonOrigin).normalized() * simval.pistonBaseLength;
    simval.pistonBaseEnd = simval.pistonOrigin + simval.pistonDirection;

    auto crankShaft = createCrankShaft(simval);
    auto piston = createPiston(simval);
}

std::vector<Primitive> Simulator::createCrankShaft(SimulationValues& simval){
    float axisRadius = 6;
    float axisPassLenght = 50;
    float axisHeight = 10;

    auto axis = Primitive::createCylinder(15, axisPassLenght, axisRadius);
    axis.vertexList = P3D::rotateVectorY(axis.vertexList, DEG_90); // deita o cilindro
    axis.vertexList = P3D::rotateVectorX(axis.vertexList, simval.crankshaftAngle);
    axis.color = Vector3(0.2,0.2,0.2);

    std::vector<Primitive> crankShaft;
    crankShaft.push_back(axis);

    // criar 'manivela', maozinha
    auto shaft = Primitive::createCube(1);
    shaft.vertexList = P3D::translateVectorZ(shaft.vertexList, 0.5f);
    shaft.vertexList = P3D::scaleVector(shaft.vertexList, Vector3(axisHeight, axisHeight, simval.crankshaftLength - 2*axisRadius));
    shaft.vertexList = P3D::translateVectorZ(shaft.vertexList, axisRadius);
    shaft.vertexList = P3D::rotateVectorX(shaft.vertexList, simval.crankshaftAngle);
    shaft.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(shaft);

    // agora, cria a pontinha. quase igual ao centro
    auto endPoint = Primitive::createCylinder(15, axisHeight, axisRadius);
    endPoint.vertexList = P3D::rotateVectorY(endPoint.vertexList, DEG_90); // deita o cilindro tbm
    endPoint.vertexList = P3D::rotateVectorX(endPoint.vertexList, simval.crankshaftAngle);
    endPoint.vertexList = P3D::translateVector(endPoint.vertexList, simval.crankshaftEnd);
    endPoint.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(endPoint);

    return crankShaft;
}

std::vector<Primitive> Simulator::createPiston(SimulationValues& simval){

    Vector3 reference = Vector3(0,1,0);

    float pistonAngle = -(acos(reference.dot(simval.pistonDirection.normalized())) - DEG_90);

    auto base = Primitive::createCylinder(15, 1, 1);
    base.vertexList = P3D::translateVectorZ(base.vertexList, -0.5f);
    base.vertexList = P3D::scaleVector(base.vertexList, Vector3(10, 10, simval.pistonBaseLength));
    base.vertexList = P3D::rotateVectorX(base.vertexList, pistonAngle);
    base.vertexList = P3D::translateVector(base.vertexList, simval.pistonOrigin);
    base.color = Vector3(0.8, 0.8, 0);

    float armDelta = (simval.crankshaftEnd - simval.pistonBaseEnd).magnitude;
    auto arm = Primitive::createCylinder(15,1,1);
    arm.vertexList = P3D::translateVectorZ(arm.vertexList, -0.5f);
    arm.vertexList = P3D::scaleVector(arm.vertexList, Vector3(7, 7, simval.pistonBaseLength));
    arm.vertexList = P3D::translateVectorZ(arm.vertexList, -armDelta);
    arm.vertexList = P3D::rotateVectorX(arm.vertexList, pistonAngle);
    arm.vertexList = P3D::translateVector(arm.vertexList, simval.pistonOrigin);
    arm.color = Vector3(0.5, 0.5, 0.5);

    return { base, arm };
}
