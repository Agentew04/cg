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
    auto endPoint = Primitive::createCylinder(15, 3*simval.crankshaftAxisHeight, simval.crankshaftAxisRadius);
    rotateY(endPoint, DEG_90); // deita o cilindro tbm
    rotateX(endPoint, simval.crankshaftAngle);
    translateX(endPoint, -simval.crankshaftAxisHeight);
    translatev(endPoint, simval.crankshaftEnd);
    endPoint.color = Vector3(0.2,0.2,0.2);
    crankShaft.push_back(endPoint);

    // outra maozinha pra engatar com o outro lado do virabrequim
    translateX(shaft, -2*simval.crankshaftAxisHeight);
    crankShaft.push_back(shaft);

    // outro lado do eixo
    translateX(axis, simval.crankshaftAxisHeight*0.5f);
    translatev(axis, simval.driveshaftJointPosition);
    crankShaft.push_back(axis);

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

std::vector<Primitive> Simulator::createDriveShaft() const{
    std::vector<Primitive> parts;

    std::vector<Primitive> p1 = createDriveShaftPart();
    std::vector<Primitive> p2 = createDriveShaftPart();
    for(auto &p:p1){
        rotateX(p, simval.crankshaftAngle);
        translatev(p, simval.driveshaftJointPosition - Vector3(simval.driveshaftLength, 0, 0));
    }

    for(auto &p:p2){
        scaleX(p, -1);
        rotateX(p, simval.crankshaftAngle + DEG_90);
        rotateY(p, simval.driveshaftAngle);
        translatev(p, simval.driveshaftMiddleJointPosition);
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
    rotateY(axis, DEG_90); // deita
    translateX(axis, 0.5f); // coloca lado de baixo na origem
    scaleX(axis, simval.driveshaftLength - simval.driveshaftCaliber - simval.driveshaftJointDiameter);
    translateX(axis, simval.driveshaftCaliber + simval.driveshaftJointDiameter);
    axis.color = Vector3(1,0,0);
    parts.push_back(axis);

    Primitive basePlate = Primitive::createCube(1);
    scaleZ(basePlate, simval.driveshaftAxisRadius*2);
    scaleY(basePlate, simval.driveshaftJointDiameter + simval.driveshaftCaliber*2);
    scaleX(basePlate, simval.driveshaftCaliber);
    translateX(basePlate, simval.driveshaftJointDiameter + simval.driveshaftCaliber*0.5);
    basePlate.color = Vector3(1,0,0);
    parts.push_back(basePlate);

    Primitive leftPlate = Primitive::createCube(1);
    scaleZ(leftPlate, simval.driveshaftAxisRadius*2);
    scaleY(leftPlate, simval.driveshaftCaliber);
    scaleX(leftPlate, simval.driveshaftJointDiameter + simval.driveshaftOverhang);
    translateY(leftPlate, simval.driveshaftJointDiameter*0.5 + simval.driveshaftCaliber*0.5);
    translateX(leftPlate, (simval.driveshaftJointDiameter + simval.driveshaftOverhang)*0.5 - simval.driveshaftOverhang);
    leftPlate.color = Vector3(1,0,0);
    parts.push_back(leftPlate);

    Primitive rightPlate = Primitive::createCube(1);
    scaleZ(rightPlate, simval.driveshaftAxisRadius*2);
    scaleY(rightPlate, simval.driveshaftCaliber);
    scaleX(rightPlate, simval.driveshaftJointDiameter + simval.driveshaftOverhang);
    translateY(rightPlate, -(simval.driveshaftJointDiameter*0.5 + simval.driveshaftCaliber*0.5));
    translateX(rightPlate, (simval.driveshaftJointDiameter + simval.driveshaftOverhang)*0.5 - simval.driveshaftOverhang);
    rightPlate.color = Vector3(1,0,0);
    parts.push_back(rightPlate);

    return parts;
}

std::vector<Primitive> Simulator::createDriveShaftConnector() const {
    std::vector<Primitive> parts;

    float connHeight = 2;
    float bigConnRadius = simval.driveshaftJointDiameter*0.5*0.75;
    float smallConnRadiusPos = bigConnRadius + simval.driveshaftJointDiameter*0.25*0.25;

    Primitive middleCylinder = Primitive::createCylinder(15, connHeight, bigConnRadius);
    rotateY(middleCylinder, DEG_90);
    rotateX(middleCylinder, simval.crankshaftAngle);
    Vector3 rotationAxis = simval.crankshaftDirection.cross(Vector3(-1,0,0));
    //std::cout << "crankshaftangle: " << fmod(simval.crankshaftAngle * RAD_TO_DEG, 360) << "; driveshaftangle: " << fmod(cos(CV::time()*0.125)*simval.driveshaftAngle * RAD_TO_DEG,360) << std::endl;
    float angle = fmod((sin(simval.crankshaftAngle)*0.4*cos(simval.driveshaftAngle*2.6)-(4/10.0f)),360);
    std::cout << "angle: " << angle * RAD_TO_DEG << std::endl;
    middleCylinder.vertexList = P3D::rotateVectorAxis(middleCylinder.vertexList, rotationAxis, angle);
    translatev(middleCylinder, simval.driveshaftMiddleJointPosition);
    middleCylinder.color = Vector3(0,1,0);
    parts.push_back(middleCylinder);

    Primitive conn1 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    translateZ(conn1, smallConnRadiusPos);
    rotateX(conn1, simval.crankshaftAngle);
    rotateY(conn1, simval.driveshaftAngle);
    translatev(conn1, simval.driveshaftMiddleJointPosition);
    conn1.color = Vector3(0,1,0);
    //parts.push_back(conn1);

    Primitive conn2 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    translateZ(conn2, smallConnRadiusPos);
    rotateX(conn2, simval.crankshaftAngle + DEG_90);
    rotateY(conn2, simval.driveshaftAngle);
    translatev(conn2, simval.driveshaftMiddleJointPosition);
    conn2.color = Vector3(0,1,0);
    //parts.push_back(conn2);

    Primitive conn3 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    translateZ(conn3, -smallConnRadiusPos);
    rotateX(conn3, simval.crankshaftAngle + 2*DEG_90);
    rotateY(conn3, simval.driveshaftAngle);
    translatev(conn3, simval.driveshaftMiddleJointPosition);
    conn3.color = Vector3(0,1,0);
    //parts.push_back(conn3);

    Primitive conn4 = Primitive::createCylinder(5, simval.driveshaftJointDiameter*0.5*0.25, connHeight);
    translateZ(conn4, -smallConnRadiusPos);
    rotateX(conn4, simval.crankshaftAngle + 3*DEG_90);
    rotateY(conn4, simval.driveshaftAngle);
    translatev(conn4, simval.driveshaftMiddleJointPosition);
    conn4.color = Vector3(0,1,0);
    //parts.push_back(conn4);

    return parts;
}