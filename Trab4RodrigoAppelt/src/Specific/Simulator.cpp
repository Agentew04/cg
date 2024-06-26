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
#define SCALEX(v,f) ((v).vertexList = P3D::scaleVectorX((v).vertexList,f))
#define SCALEY(v,f) ((v).vertexList = P3D::scaleVectorY((v).vertexList,f))
#define SCALEZ(v,f) ((v).vertexList = P3D::scaleVectorZ((v).vertexList,f))
#define SCALE(v,fx,fy,fz) ((v).vertexList = P3D::scaleVector((v).vertexList, Vector3(fx,fy,fz)))
#define SCALEv(v,f) ((v).vertexList = P3D::scaleVector((v).vertexList, f))

#define TRANSLATEX(v,f) ((v).vertexList = P3D::translateVectorX((v).vertexList,f))
#define TRANSLATEY(v,f) ((v).vertexList = P3D::translateVectorY((v).vertexList,f))
#define TRANSLATEZ(v,f) ((v).vertexList = P3D::translateVectorZ((v).vertexList,f))
#define TRANSLATE(v,fx,fy,fz) ((v).vertexList = P3D::translateVector((v).vertexList, Vector3(fx,fy,fz)))
#define TRANSLATEv(v,f) ((v).vertexList = P3D::translateVector((v).vertexList, f))

#define ROTATEX(v,f) ((v).vertexList = P3D::rotateVectorX((v).vertexList,f))
#define ROTATEY(v,f) ((v).vertexList = P3D::rotateVectorY((v).vertexList,f))
#define ROTATEZ(v,f) ((v).vertexList = P3D::rotateVectorZ((v).vertexList,f))
#define ROTATE(v,fx,fy,fz) ((v).vertexList = P3D::rotateVector((v).vertexList, Vector3(fx,fy,fz)))
#define ROTATEv(v,f) ((v).vertexList = P3D::rotateVector((v).vertexList, f))

Simulator::Simulator(){
    simval = {};
    simval.rpm = 60;
}

SimulationValues& Simulator::getValues(){
    return simval;
}

float time = 0;

void Simulator::update(float delta){
    
    // virabrequim
    simval.crankshaftOrigin = Vector3::zero();
    simval.crankshaftLength = 100;
    time += delta * simval.rpm;
    simval.crankshaftAngle = time * RPM_TO_RADS;
    simval.crankshaftDirection = Vector3(0, -sin(simval.crankshaftAngle), cos(simval.crankshaftAngle));
    simval.crankshaftEnd = simval.crankshaftOrigin + (simval.crankshaftDirection * simval.crankshaftLength);
    simval.crankshaftAxisHeight = 10;
    simval.crankshaftAxisRadius = 6;

    simval.spacePistonCrankshaft = 10;

    // pistao 8pts
    simval.pistonArmLength = simval.crankshaftLength*2 + simval.spacePistonCrankshaft;
    simval.pistonBaseLength = simval.pistonArmLength; // a base vai ficar com um pouco de espaco sobrando em cima. intencional!
    simval.pistonOrigin = simval.crankshaftOrigin;
    simval.pistonOrigin.z += simval.crankshaftLength + simval.spacePistonCrankshaft + simval.pistonArmLength;
    simval.pistonDirection = (simval.crankshaftEnd - simval.pistonOrigin).normalized() * simval.pistonBaseLength;
    simval.pistonBaseEnd = simval.pistonOrigin + simval.pistonDirection;

    // engrenagens 2 pts
    simval.crankshaftGearLength = 100;
    simval.gearJointPosition = simval.crankshaftOrigin + Vector3(simval.crankshaftGearLength - simval.crankshaftAxisHeight/2, 0, 0);
    simval.gearRadius = 30;

    // eixo cardan 6 pts
    simval.driveshaftAngle = -DEG_90*0.5;
    simval.driveshaftLength = 200;
    simval.driveshaftJointPosition = simval.crankshaftOrigin - Vector3(simval.crankshaftGearLength + simval.crankshaftAxisHeight *1.5f, 0, 0);
    simval.driveshaftAxisRadius = simval.crankshaftAxisRadius;
    simval.driveshaftJointDiameter = simval.driveshaftAxisRadius*4;
    simval.driveshaftLength = simval.crankshaftGearLength;
    simval.driveshaftOverhang = simval.driveshaftAxisRadius*0.5;
    simval.driveshaftCaliber = simval.driveshaftAxisRadius*0.5;
    simval.driveshaftMiddleJointPosition = simval.driveshaftJointPosition - Vector3(simval.driveshaftLength, 0, 0);

    simval.driveshaftConnectorAngle;
}

std::vector<Primitive> Simulator::createCrankShaft() const{
    auto axis = Primitive::createCylinder(15, 1, simval.crankshaftAxisRadius);
    ROTATEY(axis, DEG_90); // deita o cilindro
    TRANSLATEX(axis,0.5f); // faz crescer pra um lado so
    SCALEX(axis, simval.crankshaftGearLength); // teria q trocar isso para o eixo cardan
    TRANSLATEX(axis, -simval.crankshaftAxisHeight/2);
    ROTATEX(axis, simval.crankshaftAngle);
    axis.color = Vector3(0.2,0.2,0.2);

    std::vector<Primitive> crankShaft;
    crankShaft.push_back(axis);

    // criar 'manivela', maozinha
    auto shaft = Primitive::createCube(1);
    TRANSLATEZ(shaft, 0.5f);
    SCALE(shaft, simval.crankshaftAxisHeight, simval.crankshaftAxisHeight, simval.crankshaftLength - 2*simval.crankshaftAxisRadius);
    TRANSLATEZ(shaft, simval.crankshaftAxisRadius);
    ROTATEX(shaft, simval.crankshaftAngle);
    shaft.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(shaft);

    // agora, cria a pontinha. quase igual ao centro
    auto endPoint = Primitive::createCylinder(15, 3*simval.crankshaftAxisHeight, simval.crankshaftAxisRadius);
    ROTATEY(endPoint, DEG_90); // deita o cilindro tbm
    ROTATEX(endPoint, simval.crankshaftAngle);
    TRANSLATEX(endPoint, -simval.crankshaftAxisHeight);
    TRANSLATEv(endPoint, simval.crankshaftEnd);
    endPoint.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(endPoint);

    // outra maozinha pra engatar com o outro lado do virabrequim
    TRANSLATEX(shaft, -2*simval.crankshaftAxisHeight);
    crankShaft.push_back(shaft);

    // outro lado do eixo
    TRANSLATEX(axis, simval.crankshaftAxisHeight*0.5f);
    TRANSLATEv(axis, simval.driveshaftJointPosition);
    crankShaft.push_back(axis);

    return crankShaft;
}

std::vector<Primitive> Simulator::createPiston() const{
    float displacement = 10;
    Vector3 reference = Vector3(0,1,0);

    float pistonAngle = -(acos(reference.dot(simval.pistonDirection.normalized())) - DEG_90);

    auto base = Primitive::createCylinder(15, 1, 1);
    TRANSLATEZ(base, -0.5f);
    SCALE(base, 7, 7, simval.pistonBaseLength);
    ROTATEX(base, pistonAngle);
    TRANSLATEv(base, simval.pistonOrigin);
    TRANSLATEX(base, -displacement);
    base.color = Vector3(0.8, 0.8, 0);

    float armDelta = (simval.crankshaftEnd - simval.pistonBaseEnd).magnitude;
    auto arm = Primitive::createCylinder(15,1,1);
    TRANSLATEZ(arm, -0.5f);
    SCALE(arm, 5, 5, simval.pistonBaseLength);
    TRANSLATEZ(arm, -armDelta);
    ROTATEX(arm, pistonAngle);
    TRANSLATEv(arm, simval.pistonOrigin);
    TRANSLATEX(arm, -displacement);
    arm.color = Vector3(0.5, 0.5, 0.5);

    return { arm, base };
}

std::vector<Primitive> Simulator::createGears() const {
    std::vector<Primitive> parts;
    int teeth = 20;
    float displacement = 2*PI / teeth / 4.0; // 4 segmentos por dente
    float baseAngle = simval.crankshaftAngle;

    Primitive gear1 = Primitive::createGear(teeth, 0.05);
    SCALE(gear1, simval.gearRadius, simval.gearRadius, simval.gearRadius); // escala pro tamanho da engrenagem
    ROTATEY(gear1, DEG_90); // endireita
    ROTATEX(gear1, displacement + simval.crankshaftAngle); // rotaciona ela
    TRANSLATEv(gear1, simval.gearJointPosition); // move pro local certo
    TRANSLATEX(gear1, simval.gearRadius * 0.25); // conecta extamente no virabrequim
    gear1.color = Vector3(1,0,0);

    Primitive gear2 = Primitive::createGear(teeth, 0.05);
    SCALE(gear2, simval.gearRadius, simval.gearRadius, simval.gearRadius);
    ROTATEY(gear2, DEG_90); // endireita
    ROTATEX(gear2, -simval.crankshaftAngle); // rotaciona ela
    TRANSLATEv(gear2, simval.gearJointPosition); // move pro local certo
    TRANSLATEY(gear2, 2*simval.gearRadius); // joga pro lado pra conectar
    TRANSLATEX(gear2, simval.gearRadius * 0.25);
    gear2.color = Vector3(0,0,1);

    parts.push_back(gear1);
    parts.push_back(gear2);

    return parts;
}

std::vector<Primitive> Simulator::createDriveShaft() const{
    std::vector<Primitive> parts;

    std::vector<Primitive> p1 = createDriveShaftPart();
    std::vector<Primitive> p2 = createDriveShaftPart();
    for(auto &p:p1){
        ROTATEX(p, simval.crankshaftAngle);
        TRANSLATEv(p, simval.driveshaftJointPosition - Vector3(simval.driveshaftLength, 0, 0));
    }

    for(auto &p:p2){
        SCALEX(p, -1);
        ROTATEX(p, simval.crankshaftAngle + DEG_90);
        ROTATEY(p, simval.driveshaftAngle);
        TRANSLATEv(p, simval.driveshaftMiddleJointPosition);
    }


    for(auto &p:p1)
        parts.push_back(p);
    for(auto &p:p2)
        parts.push_back(p);

    return parts;
}

std::vector<Primitive> Simulator::createDriveShaftPart() const{
    std::vector<Primitive> parts;

    Primitive axis = Primitive::createCylinder(15, 1, simval.driveshaftAxisRadius);
    ROTATEY(axis, DEG_90); // deita
    TRANSLATEX(axis, 0.5f); // coloca lado de baixo na origem
    SCALEX(axis, simval.driveshaftLength - simval.driveshaftCaliber - simval.driveshaftJointDiameter);
    TRANSLATEX(axis, simval.driveshaftCaliber + simval.driveshaftJointDiameter);
    axis.color = Vector3(1,0,0);
    parts.push_back(axis);

    Primitive basePlate = Primitive::createCube(1);
    SCALEZ(basePlate, simval.driveshaftAxisRadius*2);
    SCALEY(basePlate, simval.driveshaftJointDiameter + simval.driveshaftCaliber*2);
    SCALEX(basePlate, simval.driveshaftCaliber);
    TRANSLATEX(basePlate, simval.driveshaftJointDiameter + simval.driveshaftCaliber*0.5);
    basePlate.color = Vector3(1,0,0);
    parts.push_back(basePlate);

    Primitive leftPlate = Primitive::createCube(1);
    SCALEZ(leftPlate, simval.driveshaftAxisRadius*2);
    SCALEY(leftPlate, simval.driveshaftCaliber);
    SCALEX(leftPlate, simval.driveshaftJointDiameter + simval.driveshaftOverhang);
    TRANSLATEY(leftPlate, simval.driveshaftJointDiameter*0.5 + simval.driveshaftCaliber*0.5);
    TRANSLATEX(leftPlate, (simval.driveshaftJointDiameter + simval.driveshaftOverhang)*0.5 - simval.driveshaftOverhang);
    leftPlate.color = Vector3(1,0,0);
    parts.push_back(leftPlate);

    Primitive rightPlate = Primitive::createCube(1);
    SCALEZ(rightPlate, simval.driveshaftAxisRadius*2);
    SCALEY(rightPlate, simval.driveshaftCaliber);
    SCALEX(rightPlate, simval.driveshaftJointDiameter + simval.driveshaftOverhang);
    TRANSLATEY(rightPlate, -(simval.driveshaftJointDiameter*0.5 + simval.driveshaftCaliber*0.5));
    TRANSLATEX(rightPlate, (simval.driveshaftJointDiameter + simval.driveshaftOverhang)*0.5 - simval.driveshaftOverhang);
    rightPlate.color = Vector3(1,0,0);
    parts.push_back(rightPlate);

    return parts;
}

std::vector<Primitive> Simulator::createDriveShaftConnector() const {
    std::vector<Primitive> parts;

    float connHeight = 2;
    float bigConnRadius = simval.driveshaftJointDiameter*0.5*0.75;
    float smallConnRadiusPos = bigConnRadius + simval.driveshaftJointDiameter*0.25*0.25;

    Vector3 p1 = P3D::rotateX(Vector3(0, 0, simval.driveshaftJointDiameter*0.5), simval.crankshaftAngle);
    Vector3 p2 = P3D::rotateY(p1, simval.driveshaftAngle);

    float connAngle = p1.angle(p2);
    float crankDegree = fmod(simval.crankshaftAngle*RAD_TO_DEG, 360);
    if(crankDegree >= 90 && crankDegree < 270){
        connAngle = -connAngle;
    }

    Primitive middleCylinder = Primitive::createCylinder(15, connHeight, bigConnRadius);
    ROTATEY(middleCylinder, DEG_90);
    ROTATEX(middleCylinder, simval.crankshaftAngle);
    
    Vector3 rotationAxis = simval.crankshaftDirection.cross(Vector3(-1,0,0));
    middleCylinder.vertexList = P3D::rotateVectorAxis(middleCylinder.vertexList, rotationAxis, connAngle);
    TRANSLATEv(middleCylinder, simval.driveshaftMiddleJointPosition);
    middleCylinder.color = Vector3(0,1,0);
    parts.push_back(middleCylinder);

    Primitive conn1 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    TRANSLATEZ(conn1, smallConnRadiusPos);
    ROTATEX(conn1, simval.crankshaftAngle);
    ROTATEY(conn1, simval.driveshaftAngle);
    TRANSLATEv(conn1, simval.driveshaftMiddleJointPosition);
    conn1.color = Vector3(0,1,0);
    parts.push_back(conn1);

    Primitive conn2 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    TRANSLATEZ(conn2, smallConnRadiusPos);
    ROTATEX(conn2, simval.crankshaftAngle + DEG_90);
    TRANSLATEv(conn2, simval.driveshaftMiddleJointPosition);
    conn2.color = Vector3(0,1,0);
    parts.push_back(conn2);

    Primitive conn3 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    TRANSLATEZ(conn3, -smallConnRadiusPos);
    ROTATEX(conn3, simval.crankshaftAngle);
    ROTATEY(conn3, simval.driveshaftAngle);
    TRANSLATEv(conn3, simval.driveshaftMiddleJointPosition);
    conn3.color = Vector3(0,1,0);
    parts.push_back(conn3);

    Primitive conn4 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    TRANSLATEZ(conn4, -smallConnRadiusPos);
    ROTATEX(conn4, simval.crankshaftAngle + DEG_90);
    TRANSLATEv(conn4, simval.driveshaftMiddleJointPosition);
    conn4.color = Vector3(0,1,0);
    parts.push_back(conn4);

    return parts;
}