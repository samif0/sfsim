#include "core/time.hpp"
#include <algorithm>

namespace SFSim {
namespace Core {

Time& Time::getInstance() {
    static Time instance;
    return instance;
}

Time::Time()
    : _deltaTime(0.0f)
    , _fixedDeltaTime(1.0f / 60.0f)
    , _totalTime(0.0f)
    , _unscaledDeltaTime(0.0f)
    , _unscaledTotalTime(0.0f)
    , _timeScale(1.0f)
    , _maxDeltaTime(1.0f / 15.0f)
    , _fixedAccumulator(0.0f)
    , _frameCount(0)
    , _fpsFrameCount(0)
    , _fps(0.0f)
    , _fpsUpdateTime(0.0f)
    , _paused(false)
    , _initialized(false)
{
}

void Time::initialize() {
    _clock.restart();
    _fpsClock.restart();
    _initialized = true;
    
    _deltaTime = 0.0f;
    _totalTime = 0.0f;
    _unscaledDeltaTime = 0.0f;
    _unscaledTotalTime = 0.0f;
    _fixedAccumulator = 0.0f;
    _frameCount = 0;
    _fpsFrameCount = 0;
    _fps = 0.0f;
    _fpsUpdateTime = 0.0f;
}

void Time::update() {
    if (!_initialized) {
        initialize();
        return;
    }
    
    float rawDeltaTime = _clock.restart().asSeconds();
    
    _unscaledDeltaTime = std::min(rawDeltaTime, _maxDeltaTime);
    _unscaledTotalTime += _unscaledDeltaTime;
    
    if (_paused) {
        _deltaTime = 0.0f;
    } else {
        _deltaTime = _unscaledDeltaTime * _timeScale;
        _totalTime += _deltaTime;
    }
    
    _fixedAccumulator += _deltaTime;
    
    _frameCount++;
    _fpsFrameCount++;
    
    _fpsUpdateTime += _unscaledDeltaTime;
    if (_fpsUpdateTime >= 1.0f) {
        _fps = _fpsFrameCount / _fpsUpdateTime;
        _fpsFrameCount = 0;
        _fpsUpdateTime = 0.0f;
    }
}

bool Time::shouldUpdateFixed() {
    if (_fixedAccumulator >= _fixedDeltaTime) {
        _fixedAccumulator -= _fixedDeltaTime;
        return true;
    }
    return false;
}

} // namespace Core
} // namespace SFSim