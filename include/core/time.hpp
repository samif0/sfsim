#pragma once

#include <SFML/System.hpp>

namespace SFSim {
namespace Core {

class Time {
public:
    static Time& getInstance();
    
    void initialize();
    void update();
    
    float getDeltaTime() const { return _deltaTime; }
    float getFixedDeltaTime() const { return _fixedDeltaTime; }
    float getTotalTime() const { return _totalTime; }
    
    int getFrameCount() const { return _frameCount; }
    float getFPS() const { return _fps; }
    
    void setTimeScale(float scale) { _timeScale = scale; }
    float getTimeScale() const { return _timeScale; }
    
    void setFixedTimeStep(float timeStep) { _fixedDeltaTime = timeStep; }
    
    bool shouldUpdateFixed();
    
    void pause() { _paused = true; }
    void resume() { _paused = false; }
    bool isPaused() const { return _paused; }
    
    void setMaxDeltaTime(float maxDelta) { _maxDeltaTime = maxDelta; }
    float getMaxDeltaTime() const { return _maxDeltaTime; }
    
    float getUnscaledDeltaTime() const { return _unscaledDeltaTime; }
    float getUnscaledTotalTime() const { return _unscaledTotalTime; }
    
private:
    Time();
    ~Time() = default;
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;
    
    sf::Clock _clock;
    sf::Clock _fpsClock;
    
    float _deltaTime;
    float _fixedDeltaTime;
    float _totalTime;
    float _unscaledDeltaTime;
    float _unscaledTotalTime;
    
    float _timeScale;
    float _maxDeltaTime;
    
    float _fixedAccumulator;
    
    int _frameCount;
    int _fpsFrameCount;
    float _fps;
    float _fpsUpdateTime;
    
    bool _paused;
    bool _initialized;
};

} // namespace Core
} // namespace SFSim