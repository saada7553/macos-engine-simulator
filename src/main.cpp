#include "../include/transmission.h"
#include "../scripting/include/compiler.h"
#include "../include/s_audio_file.h"

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

    ImpulseResponse *response = engine->getExhaustSystem(0)->getImpulseResponse();
    std::cout << response->getFilename() << std::endl; 

    Simulator* sim = engine->createSimulator(vehicle, transmission); 
    sim->getEngine()->getIgnitionModule()->m_enabled = true; 
    engine->setSpeedControl(0.04); 
    sim->setSimulationSpeed(1.0); 
    sim->m_starterMotor.m_enabled = true;

    engine->calculateDisplacement();
    sim->setSimulationFrequency(engine->getSimulationFrequency());

    // Synthesizer::AudioParameters audioParams = sim->synthesizer().getAudioParameters();
    // audioParams.inputSampleNoise = static_cast<float>(engine->getInitialJitter());
    // audioParams.airNoise = static_cast<float>(engine->getInitialNoise());
    // audioParams.dF_F_mix = static_cast<float>(engine->getInitialHighFrequencyGain());
    // sim->synthesizer().setAudioParameters(audioParams);

    // for (int i = 0; i < engine->getExhaustSystemCount(); ++i) {
    //     ImpulseResponse *response = engine->getExhaustSystem(i)->getImpulseResponse();

    //     sAudioFile waveFile;
    //     waveFile.OpenFile(response->getFilename().c_str());
    //     waveFile.InitializeInternalBuffer(waveFile.GetSampleCount());
    //     waveFile.FillBuffer(0);
    //     waveFile.CloseFile();

    //     sim->synthesizer().initializeImpulseResponse(
    //         reinterpret_cast<const int16_t *>(waveFile.GetBuffer()),
    //         waveFile.GetSampleCount(),
    //         response->getVolume(),
    //         i
    //     );

    //     waveFile.DestroyInternalBuffer();
    // }
    // sim->startAudioRenderingThread();

    size_t cnt = 0; 
    size_t iteration = 0; 

    while(1) {
        double rpm = engine->getRpm();
        std::cout << "\rITERATION: " << iteration << " RPM: " << rpm << "    " << std::flush;

        sim->startFrame(1.0 / 120.0);

        const int iterationCount = sim->getFrameIterationCount();
        while (sim->simulateStep()) { }

        sim->endFrame(); 
        iteration++; 
    }

    std::cout << "made sim" << std::endl; 
    return 0; 
}
