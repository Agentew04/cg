#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <vector>

#include "../Math/Vector3.h"
#include "../3D/Primitive.h"

struct SimulationValues {
    Vector3 pistonOrigin;
    float pistonBaseLength;
    float pistonArmLength;
    Vector3 pistonDirection;
    Vector3 pistonBaseEnd;

    float spacePistonCrankshaft;

    Vector3 crankshaftOrigin;
    Vector3 crankshaftDirection;
    Vector3 crankshaftEnd;
    float crankshaftLength;
    float crankshaftAngle;
    float crankshaftAxisHeight;
    float crankshaftAxisRadius;

    float crankshaftGearLength;
    Vector3 gearJointPosition;
    float gearRadius;

    float rpm;

    float driveshaftAngle; // 0 a 45 graus em radianos
    Vector3 driveshaftJointPosition;
    Vector3 driveshaftMiddleJointPosition;
    float driveshaftJointDiameter; // controlam o formato da peca
    float driveshaftAxisRadius;
    float driveshaftLength;
    float driveshaftOverhang;
    float driveshaftCaliber;
    float driveshaftConnectorAngle;
};

/// @brief Classe que faz todos os calculos da simulacao.
/// Incluindo o pistao, virabrequim, engrenagens e possivelmente eixo cardan
class Simulator {
public:

    Simulator();

    /// @brief Faz todos os calculos e atualiza pontos/vetores.
    void update(float delta);

    SimulationValues& getValues();

    std::vector<Primitive> createCrankShaft() const;
    std::vector<Primitive> createPiston() const;
    std::vector<Primitive> createGears() const;
    std::vector<Primitive> createDriveShaft() const;
    std::vector<Primitive> createDriveShaftConnector() const;
private:
    SimulationValues simval;

    std::vector<Primitive> createDriveShaftPart() const;
};

#endif
