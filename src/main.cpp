// #include "../include/engine_sim_application.h"
// #include "../include/vehicle.h"
// #include "../include/engine.h"
#include "../include/transmission.h"
#include "../scripting/include/compiler.h"

#include <iostream>

int main() {
    std::cout << "hello world" << std::endl; 

    Vehicle *vehicle = nullptr; 
    Transmission *transmission = nullptr;
    Engine *engine = nullptr; 

    es_script::Compiler compiler;
    compiler.initialize();
    const bool compiled = compiler.compile("../assets/main.mr");
    if (compiled) {
        const es_script::Compiler::Output output = compiler.execute();

        engine = output.engine;
        vehicle = output.vehicle;
        transmission = output.transmission;
    }
    else {
        std::cout << "failed compilation" << std::endl; 
    }
    compiler.destroy(); 

    std::cout << engine->getName() << std::endl; 

    // Engine::Parameters engineParams;
    // engineParams.cylinderBanks = 1; 
    // engineParams.cylinderCount = 1; 
    // engineParams.crankshaftCount = 1; 
    // engineParams.exhaustSystemCount = 1; 
    // engineParams.intakeCount = 1; 
    // engineParams.name = "new engine"; 
    // engineParams.throttle = nullptr; 
    // engineParams.initialSimulationFrequency = 0; 
    // engineParams.initialHighFrequencyGain = 0; 
    // engineParams.initialNoise = 0; 
    // engineParams.initialJitter = 0; 
    // engine->initialize(engineParams); 

    // Vehicle::Parameters vehParams;
    // vehParams.mass = units::mass(1597, units::kg);
    // vehParams.diffRatio = 3.42;
    // vehParams.tireRadius = units::distance(10, units::inch);
    // vehParams.dragCoefficient = 0.25;
    // vehParams.crossSectionArea = units::distance(6.0, units::foot) * units::distance(6.0, units::foot);
    // vehParams.rollingResistance = 2000.0;
    // vehicle = new Vehicle;
    // vehicle->initialize(vehParams);

    // const double gearRatios[] = { 2.97, 2.07, 1.43, 1.00, 0.84, 0.56 };
    // Transmission::Parameters tParams;
    // tParams.GearCount = 6;
    // tParams.GearRatios = gearRatios;
    // tParams.MaxClutchTorque = units::torque(1000.0, units::ft_lb);
    // transmission = new Transmission;
    // transmission->initialize(tParams);

    Simulator* sim = engine->createSimulator(vehicle, transmission); 
    sim->getEngine()->getIgnitionModule()->m_enabled = true; 
    // engine->setSpeedControl(0.1); 
    sim->setSimulationSpeed(1.0); 
    sim->m_starterMotor.m_enabled = true;

    size_t cnt = 0; 

    while(1) {
        std::cout << engine->getRpm() << std::endl; 

        sim->startFrame(1 / 30);

        if(engine->getRpm() > 363)
            cnt++; 
        
        if(cnt > 10000) {
            sim->m_starterMotor.m_enabled = false; 
            // std::cout << "starter off" << std::endl; 
        }

        // if(cnt > 999999) {
        //     engine->getIgnitionModule()->m_enabled = false; 
        // }

        const int iterationCount = sim->getFrameIterationCount();
        while (sim->simulateStep()) { }

        sim->endFrame(); 
    }

    std::cout << "made sim" << std::endl; 
    return 0; 
}
