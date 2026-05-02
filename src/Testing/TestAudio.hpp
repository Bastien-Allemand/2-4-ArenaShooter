#ifndef TEST_AUDIO_HPP_INCLUDED
#define TEST_AUDIO_HPP_INCLUDED

#include <Audio.h>
#include <string>
#include <chrono>
#include <iostream>
#include <thread>
#include "define.h"
using namespace gce;


BEGIN_SUBTEST( Audio, Load )

    std::wcout << L"[TestLoad] Start\n";
    const std::wstring flutePath = WRES_PATH L"res/Sounds/music1.mp3";
    AudioUse::LoadSound("War", flutePath.c_str());
    std::wcout << L"[TestLoad] Loading Sound : War\n";
    std::wcout << L"[TestLoad] End\n";

END_SUBTEST( Load )


BEGIN_SUBTEST( Audio, Unload )

    std::wcout << L"[TestUnload] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/music1.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str());
    std::wcout << L"[TestUnload] Loading Sound : War\n";
    AudioUse::UnloadSound("Effect");
    std::wcout << L"[TestUnload] Unloading Sound : War\n";

END_SUBTEST( Unload )


BEGIN_SUBTEST( Audio, Play )

    std::wcout << L"[TestPlay] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/TheFool.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str());
    AudioUse::Play("Effect");
    std::wcout << L"[TestPlay] Playing Sound : War\n";
    while (true) AudioUse::Update(0);
    std::wcout << L"[TestPlay] End\n";

END_SUBTEST( Play )


BEGIN_SUBTEST( Audio, PlayBackground )

    std::wcout << L"[TestPlayBackgroundSong] Start\n";
    const std::wstring musicPath = WRES_PATH L"res/Sounds/music3.mp3";
    AudioUse::LoadSound("Background", musicPath.c_str());
    AudioUse::Play("Background", true);
    std::wcout << L"[TestPlayBackgroundSong] Loop Listening\n";
    while (true) AudioUse::Update(0);
    std::wcout << L"[TestPlayBackgroundSong] End\n";

END_SUBTEST( PlayBackground )


BEGIN_SUBTEST( Audio, MultipleSongs )

    std::wcout << L"[TestMultipleSong] Start\n";
    const std::wstring sound1 = WRES_PATH L"res/Sounds/music3.mp3";
    const std::wstring sound2 = WRES_PATH L"res/Sounds/NewJeans.mp3";

    AudioUse::LoadSound("Effect1", sound1.c_str(), 1, false);
    AudioUse::LoadSound("Effect2", sound2.c_str(), 2, false);

    auto startTime = std::chrono::steady_clock::now();
    bool played1 = false, played2 = false;

    while (true)
    {
        auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

        if (elapsed >= 300 && !played1)
        {
            AudioUse::Play("Effect1");
            std::wcout << L"[TestMultipleSong] Sound 1 playing\n";
            played1 = true;
        }

        if (elapsed >= 600 && !played2)
        {
            AudioUse::Play("Effect2");
            std::wcout << L"[TestMultipleSong] Sound 2 playing\n";
            played2 = true;
        }

        if (elapsed >= 3000 && played2)
        {
            played2 = false;
            startTime = std::chrono::steady_clock::now();
        }

        AudioUse::Update(0);
    }
    std::wcout << L"[TestMultipleSong] End\n";

END_SUBTEST( MultipleSongs )


BEGIN_SUBTEST( Audio, PauseAndResume )

    std::wcout << L"[TestPauseAndResume] Start\n";
    const std::wstring musicPath = WRES_PATH L"res/Sounds/NewJeans.mp3";
    AudioUse::LoadSound("Music", musicPath.c_str());
    AudioUse::Play("Music");

    auto startTime = std::chrono::steady_clock::now();
    bool paused = false, resumed = false;

    while (true)
    {
        auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

        if (elapsed >= 4000 && !paused)
        {
            AudioUse::Pause("Music");
            std::wcout << L"[TestPauseAndResume] Pause\n";
            paused = true;
        }

        if (elapsed >= 6000 && !resumed)
        {
            AudioUse::Resume("Music");
            std::wcout << L"[TestPauseAndResume] Resume\n";
            resumed = true;
        }

        AudioUse::Update(0);
    }
    std::wcout << L"[TestPauseAndResume] End\n";

END_SUBTEST( PauseAndResume )


BEGIN_SUBTEST( Audio, Stop )

    std::wcout << L"[TestStop] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/music1.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str());
    AudioUse::Play("Effect");

    auto startTime = std::chrono::steady_clock::now();
    bool stopped = false;

    while (!stopped)
    {
        auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

        if (elapsed >= 4000 && !stopped)
        {
            AudioUse::Stop("Effect");
            std::wcout << L"[TestStop] Stop Sound\n";
            stopped = true;
        }

        AudioUse::Update(0);
    }
    std::wcout << L"[TestStop] End\n";

END_SUBTEST( Stop )


BEGIN_SUBTEST( Audio, MasterVolume )

    std::wcout << L"[TestMasterVolume] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/music1.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str());
    AudioUse::Play("Effect");

    auto startTime = std::chrono::steady_clock::now();
    int volumeStep = 0;

    while (true)
    {
        auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

        if (elapsed >= 4000 && volumeStep == 0)
        {
            AudioUse::SetMasterVolume(10);
            std::wcout << L"[TestMasterVolume] Master volume at 30%\n";
            volumeStep++;
        }
        else if (elapsed >= 6000 && volumeStep == 1)
        {
            AudioUse::SetMasterVolume(80);
            std::wcout << L"[TestMasterVolume] Master volume at 70%\n";
            volumeStep++;
        }

        AudioUse::Update(0);
    }
    std::wcout << L"[TestMasterVolume] End\n";

END_SUBTEST( MasterVolume )


BEGIN_SUBTEST( Audio, Volume )

    std::wcout << L"[TestVolume] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/music1.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str());
    AudioUse::Play("Effect");

    auto startTime = std::chrono::steady_clock::now();
    int currentVolume = 0;

    while (true)
    {
        auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

        if (elapsed % 5000 == 0 && currentVolume <= 100)
        {
            AudioUse::SetVolumeOfSound("Effect", currentVolume);
            std::wcout << L"[TestVolume] Sound volume : " << currentVolume << L"%\n";
            currentVolume += 10;
        }

        AudioUse::Update(0);
    }
    std::wcout << L"[TestVolume] End\n";

END_SUBTEST( Volume )


BEGIN_SUBTEST( Audio, Effect )

    std::wcout << L"[TestEffect] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/NewJeans.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str());
    AudioUse::SetAudioCategory("Effect", Category::MUSIC);
    AudioUse::Play("Effect");

    auto startTime = std::chrono::steady_clock::now();
    bool effectApplied1 = false;
    bool effectApplied2 = false;
    bool effectApplied3 = false;

    while (true)
    {
        auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

        if (elapsed >= 20000 && !effectApplied1)
        {
            AudioUse::ApplyGainCategory(Category::MUSIC, 1.5f);
            std::wcout << L"[TestEffect] Gain Effect1 applied\n";
            effectApplied1 = true;
        }
                
        if (elapsed >= 30000 && !effectApplied2)
        {
            AudioUse::ApplyGainCategory(Category::MUSIC, 0.5f);
            std::wcout << L"[TestEffect] Gain Effect2 applied\n";
            effectApplied2 = true;
        }

        if (elapsed >= 40000 && !effectApplied3)
        {
            AudioUse::ApplyGainCategory(Category::MUSIC, 0.5f);
            std::wcout << L"[TestEffect] Gain Effect3 applied\n";
            effectApplied3 = true;
        }

        AudioUse::Update(0);
    }
    std::wcout << L"[TestEffect] End\n";

END_SUBTEST( Effect )


BEGIN_SUBTEST( Audio, SpacialCircle )

    std::wcout << L"[TestEffect] Start\n";
    const std::wstring soundPath = WRES_PATH L"res/Sounds/NewJeans.mp3";
    AudioUse::LoadSound("Effect", soundPath.c_str(), 0 , true);
    //SetVolume("Effect", 20);

    AudioUse::Play("Effect", (0.0f, 0.0f, 0.0f));

    auto lastTime = std::chrono::steady_clock::now();
    float accumulatedTime = 0.0f;
    const float fixedTimeStep = 1.0f / 60.0f; // 60fps physics

    while (true)
    {
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
        lastTime = currentTime;

        // Fixed timestep update for audio
        accumulatedTime += deltaTime;
        while (accumulatedTime >= fixedTimeStep)
        {
            AudioUse::Update(fixedTimeStep);
            accumulatedTime -= fixedTimeStep;
        }

        // Small sleep to prevent CPU overuse
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::wcout << L"[TestEffect] End\n";

END_SUBTEST( SpacialCircle )


BEGIN_SUBTEST( Audio, Normalization )

    std::wcout << L"[TestNormalization] D�but\n";

    const std::wstring loudTrack = WRES_PATH L"res/Sounds/Asap.mp3";
    const std::wstring quietTrack = WRES_PATH L"res/Sounds/NewJeans.mp3";

    AudioUse::LoadSound("Loud", loudTrack.c_str());
    AudioUse::LoadSound("Quiet", quietTrack.c_str());

    auto t0 = std::chrono::steady_clock::now();
    bool playedLoud = false, playedQuiet = false;

    while (true)
    {
        auto ms = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();

        if (ms > 500 && !playedLoud) { AudioUse::Play("Loud");  playedLoud = true; }
        if (ms > 5500 && !playedQuiet) { AudioUse::Stop("Loud"); AudioUse::Play("Quiet");  playedQuiet = true; }

        if (ms > 10500) { AudioUse::Stop("Quiet"); break; }
        AudioUse::Update(0);
    }

    std::wcout << L"[TestNormalization] Fin\n";

END_SUBTEST( Normalization )


BEGIN_SUBTEST( Audio, Categories )

    std::wcout << L"[TestNormalization] D�but\n";

    const std::wstring loudTrack = WRES_PATH L"res/Sounds/Asap.mp3";
    const std::wstring quietTrack = WRES_PATH L"res/Sounds/NewJeans.mp3";

    AudioUse::LoadSound("Loud", loudTrack.c_str());
    AudioUse::LoadSound("Quiet", quietTrack.c_str());
    
    AudioUse::SetAudioCategory("Loud", Category::MUSIC);
    AudioUse::SetAudioCategory("Quiet", Category::MUSIC);

    auto t0 = std::chrono::steady_clock::now();
    bool playedLoud = false, playedQuiet = false;

    AudioUse::Play("Loud");
    AudioUse::Play("Quiet");

    while (true)
    {
        auto ms = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();

        if (ms > 10000 && !playedLoud) 
        { 
            AudioUse::SetVolumeOfCategory(Category::MUSIC, 50);
            std::cout << "Change volume of category";
            playedLoud = true; 
        }

        if (ms > 15000 && !playedQuiet) 
        {  
           AudioUse:: SetVolumeOfSound("Loud", 25);
            std::cout << "Change second music volume";
            playedQuiet = true; 
        }

        AudioUse::Update(0);
    }

    std::wcout << L"[TestNormalization] Fin\n";

END_SUBTEST( Categories )


#endif