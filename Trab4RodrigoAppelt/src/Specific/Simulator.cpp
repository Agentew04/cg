#include "Simulator.h"

#include "../gl_canvas2d.h"
#include "../3D/Primitive.h"
#include "../3D/Perspective.h"

#define PI 3.1415926535897932384626433832795
#define DEG_90 1.5707963267948966192313216916398
#define RPM_TO_RADS 0.10471975511965977461542144610932 // 2pi/60
#define RAD_TO_DEG 180.0/PI // 180/pi
#define DEG_TO_RAD PI/180.0 // pi/180

/*
Testando Macros
*/
#define scaleX(v,f) ((v).vertexList = P3D::scaleVectorX((v).vertexList,f))
#define scaleY(v,f) ((v).vertexList = P3D::scaleVectorY((v).vertexList,f))
#define scaleZ(v,f) ((v).vertexList = P3D::scaleVectorZ((v).vertexList,f))
#define scale(v,fx,fy,fz) ((v).vertexList = P3D::scaleVector((v).vertexList, Vector3(fx,fy,fz)))
#define scalev(v,f) ((v).vertexList = P3D::scaleVector((v).vertexList, f))

#define translateX(v,f) ((v).vertexList = P3D::translateVectorX((v).vertexList,f))
#define translateY(v,f) ((v).vertexList = P3D::translateVectorY((v).vertexList,f))
#define translateZ(v,f) ((v).vertexList = P3D::translateVectorZ((v).vertexList,f))
#define translate(v,fx,fy,fz) ((v).vertexList = P3D::translateVector((v).vertexList, Vector3(fx,fy,fz)))
#define translatev(v,f) ((v).vertexList = P3D::translateVector((v).vertexList, f))

#define rotateX(v,f) ((v).vertexList = P3D::rotateVectorX((v).vertexList,f))
#define rotateY(v,f) ((v).vertexList = P3D::rotateVectorY((v).vertexList,f))
#define rotateZ(v,f) ((v).vertexList = P3D::rotateVectorZ((v).vertexList,f))
#define rotate(v,fx,fy,fz) ((v).vertexList = P3D::rotateVector((v).vertexList, Vector3(fx,fy,fz)))
#define rotatev(v,f) ((v).vertexList = P3D::rotateVector((v).vertexList, f))

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
    simval.crankshaftAxisHeight = 10;
    simval.crankshaftAxisRadius = 6;

    simval.spacePistonCrankshaft = 10;

    simval.pistonArmLength = simval.crankshaftLength*2 + simval.spacePistonCrankshaft;
    simval.pistonBaseLength = simval.pistonArmLength; // a base vai ficar com um pouco de espaco sobrando em cima. intencional!
    simval.pistonOrigin = simval.crankshaftOrigin;
    simval.pistonOrigin.z += simval.crankshaftLength + simval.spacePistonCrankshaft + simval.pistonArmLength;
    simval.pistonDirection = (simval.crankshaftEnd - simval.pistonOrigin).normalized() * simval.pistonBaseLength;
    simval.pistonBaseEnd = simval.pistonOrigin + simval.pistonDirection;

    simval.crankshaftGearLength = 100;
    simval.gearJointPosition = simval.crankshaftOrigin + Vector3(simval.crankshaftGearLength - simval.crankshaftAxisHeight/2, 0, 0);
    simval.gearRadius = 30;
}

std::vector<Primitive> Simulator::createCrankShaft() const{
    auto axis = Primitive::createCylinder(15, 1, simval.crankshaftAxisRadius);
    rotateY(axis, DEG_90); // deita o cilindro
    translateX(axis,0.5f); // faz crescer pra um lado so
    scaleX(axis, simval.crankshaftGearLength); // teria q trocar isso para o eixo cardan
    translateX(axis, -simval.crankshaftAxisHeight/2);
    rotateX(axis, simval.crankshaftAngle);
    axis.color = Vector3(0.2,0.2,0.2);

    std::vector<Primitive> crankShaft;
    crankShaft.push_back(axis);

    // criar 'manivela', maozinha
    auto shaft = Primitive::createCube(1);
    translateZ(shaft, 0.5f);
    scale(shaft, simval.crankshaftAxisHeight, simval.crankshaftAxisHeight, simval.crankshaftLength - 2*simval.crankshaftAxisRadius);
    translateZ(shaft, simval.crankshaftAxisRadius);
    rotateX(shaft, simval.crankshaftAngle);
    shaft.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(shaft);

    // agora, cria a pontinha. quase igual ao centro
    auto endPoint = Primitive::createCylinder(15, 2*simval.crankshaftAxisHeight, simval.crankshaftAxisRadius);
    rotateY(endPoint, DEG_90); // deita o cilindro tbm
    rotateX(endPoint, simval.crankshaftAngle);
    translateX(endPoint, -simval.crankshaftAxisHeight/2);
    translatev(endPoint, simval.crankshaftEnd);
    endPoint.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(endPoint);

    return crankShaft;
}

std::vector<Primitive> Simulator::createPiston() const{
    float displacement = 10;
    Vector3 reference = Vector3(0,1,0);

    float pistonAngle = -(acos(reference.dot(simval.pistonDirection.normalized())) - DEG_90);

    auto base = Primitive::createCylinder(15, 1, 1);
    translateZ(base, -0.5f);
    scale(base, 7, 7, simval.pistonBaseLength);
    rotateX(base, pistonAngle);
    translatev(base, simval.pistonOrigin);
    translateX(base, -displacement);
    base.color = Vector3(0.8, 0.8, 0);

    float armDelta = (simval.crankshaftEnd - simval.pistonBaseEnd).magnitude;
    auto arm = Primitive::createCylinder(15,1,1);
    translateZ(arm, -0.5f);
    scale(arm, 5, 5, simval.pistonBaseLength);
    translateZ(arm, -armDelta);
    rotateX(arm, pistonAngle);
    translatev(arm, simval.pistonOrigin);
    translateX(arm, -displacement);
    arm.color = Vector3(0.5, 0.5, 0.5);

    return { arm, base };
}

std::vector<Primitive> Simulator::createGears() const {
    std::vector<Primitive> parts;
    int teeth = 20;
    float displacement = 2*PI / teeth / 4.0; // 4 segmentos por dente
    float baseAngle = simval.crankshaftAngle;

    Primitive gear1 = Primitive::createGear(teeth, 0.05);
    scale(gear1, simval.gearRadius, simval.gearRadius, simval.gearRadius); // escala pro tamanho da engrenagem
    rotateY(gear1, DEG_90); // endireita
    rotateX(gear1, displacement + simval.crankshaftAngle); // rotaciona ela
    translatev(gear1, simval.gearJointPosition); // move pro local certo
    translateX(gear1, simval.gearRadius * 0.25); // conecta extamente no virabrequim
    gear1.color = Vector3(1,0,0);

    Primitive gear2 = Primitive::createGear(teeth, 0.05);
    scale(gear2, simval.gearRadius, simval.gearRadius, simval.gearRadius);
    rotateY(gear2, DEG_90); // endireita
    rotateX(gear2, -simval.crankshaftAngle); // rotaciona ela
    translatev(gear2, simval.gearJointPosition); // move pro local certo
    translateY(gear2, 2*simval.gearRadius); // joga pro lado pra conectar
    translateX(gear2, simval.gearRadius * 0.25);
    gear2.color = Vector3(0,0,1);

    parts.push_back(gear1);
    parts.push_back(gear2);

    return parts;
}