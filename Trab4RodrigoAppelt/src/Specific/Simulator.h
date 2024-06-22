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

    float rpm;
};

/// @brief Classe que faz todos os calculos da simulacao.
/// Incluindo o pistao, virabrequim, engrenagens e possivelmente eixo cardan
class Simulator {
public:

    Simulator();

    /// @brief Faz todos os calculos e atualiza pontos/vetores.
    void update(float delta);

    SimulationValues& getValues();

    std::vector<Primitive> createCrankShaft(SimulationValues& simval);
    std::vector<Primitive> createPiston(SimulationValues& simval);
private:
    SimulationValues simval;
};

#endif