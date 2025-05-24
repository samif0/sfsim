#include "core/input.hpp"
#include <algorithm>

namespace SFSim {
namespace Core {

Input& Input::getInstance() {
    static Input instance;
    return instance;
}

Input::Input()
    : _mousePosition(0, 0)
    , _lastMousePosition(0, 0)
    , _mouseDelta(0, 0)
    , _mouseWheelDelta(0.0f)
    , _mouseCapture(false)
    , _mouseSensitivity(1.0f)
{
}

void Input::initialize() {
    _keyStates.clear();
    _mouseStates.clear();
    _mousePosition = Vector2f(0, 0);
    _lastMousePosition = Vector2f(0, 0);
    _mouseDelta = Vector2f(0, 0);
    _mouseWheelDelta = 0.0f;
}

void Input::update() {
    _lastMousePosition = _mousePosition;
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    _mousePosition = Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    _mouseDelta = (_mousePosition - _lastMousePosition) * _mouseSensitivity;
    
    for (auto& pair : _keyStates) {
        if (pair.second == InputState::Pressed) {
            pair.second = InputState::Held;
        } else if (pair.second == InputState::Released) {
            _keyStates.erase(pair.first);
        }
    }
    
    for (auto& pair : _mouseStates) {
        if (pair.second == InputState::Pressed) {
            pair.second = InputState::Held;
        } else if (pair.second == InputState::Released) {
            _mouseStates.erase(pair.first);
        }
    }
    
    _mouseWheelDelta = 0.0f;
    
    processActionCallbacks();
}

void Input::handleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            updateInputState(event.key.code, true);
            break;
            
        case sf::Event::KeyReleased:
            updateInputState(event.key.code, false);
            break;
            
        case sf::Event::MouseButtonPressed:
            updateInputState(static_cast<MouseButton>(event.mouseButton.button), true);
            break;
            
        case sf::Event::MouseButtonReleased:
            updateInputState(static_cast<MouseButton>(event.mouseButton.button), false);
            break;
            
        case sf::Event::MouseWheelScrolled:
            _mouseWheelDelta += event.mouseWheelScroll.delta;
            break;
            
        case sf::Event::MouseMoved:
            if (!_mouseCapture) {
                _mousePosition = Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            }
            break;
            
        default:
            break;
    }
}

bool Input::isKeyPressed(KeyCode key) const {
    InputState state = getInputState(key);
    return state == InputState::Pressed || state == InputState::Held;
}

bool Input::isKeyJustPressed(KeyCode key) const {
    return getInputState(key) == InputState::Pressed;
}

bool Input::isKeyJustReleased(KeyCode key) const {
    return getInputState(key) == InputState::Released;
}

bool Input::isMouseButtonPressed(MouseButton button) const {
    InputState state = getInputState(button);
    return state == InputState::Pressed || state == InputState::Held;
}

bool Input::isMouseButtonJustPressed(MouseButton button) const {
    return getInputState(button) == InputState::Pressed;
}

bool Input::isMouseButtonJustReleased(MouseButton button) const {
    return getInputState(button) == InputState::Released;
}

Vector2f Input::getMousePosition() const {
    return _mousePosition;
}

Vector2f Input::getMouseDelta() const {
    return _mouseDelta;
}

float Input::getMouseWheelDelta() const {
    return _mouseWheelDelta;
}

void Input::setMousePosition(const Vector2f& position) {
    _mousePosition = position;
    sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(position.x), static_cast<int>(position.y)));
}

void Input::centerMouse(const sf::Window& window) {
    sf::Vector2u windowSize = window.getSize();
    Vector2f center(windowSize.x * 0.5f, windowSize.y * 0.5f);
    setMousePosition(center);
}

void Input::bindKeyAction(const std::string& actionName, KeyCode key) {
    _actionBindings[actionName] = Action(key);
}

void Input::bindMouseAction(const std::string& actionName, MouseButton button) {
    _actionBindings[actionName] = Action(button);
}

void Input::unbindAction(const std::string& actionName) {
    _actionBindings.erase(actionName);
}

void Input::registerActionCallback(const std::string& actionName, ActionCallback callback) {
    _actionCallbacks[actionName] = callback;
}

void Input::unregisterActionCallback(const std::string& actionName) {
    _actionCallbacks.erase(actionName);
}

bool Input::isActionPressed(const std::string& actionName) const {
    auto it = _actionBindings.find(actionName);
    if (it == _actionBindings.end()) return false;
    
    const Action& action = it->second;
    if (action.type == Action::Key) {
        return isKeyPressed(action.key);
    } else {
        return isMouseButtonPressed(action.mouse);
    }
}

bool Input::isActionJustPressed(const std::string& actionName) const {
    auto it = _actionBindings.find(actionName);
    if (it == _actionBindings.end()) return false;
    
    const Action& action = it->second;
    if (action.type == Action::Key) {
        return isKeyJustPressed(action.key);
    } else {
        return isMouseButtonJustPressed(action.mouse);
    }
}

bool Input::isActionJustReleased(const std::string& actionName) const {
    auto it = _actionBindings.find(actionName);
    if (it == _actionBindings.end()) return false;
    
    const Action& action = it->second;
    if (action.type == Action::Key) {
        return isKeyJustReleased(action.key);
    } else {
        return isMouseButtonJustReleased(action.mouse);
    }
}

std::string Input::getKeyName(KeyCode key) const {
    switch (key) {
        case sf::Keyboard::A: return "A";
        case sf::Keyboard::B: return "B";
        case sf::Keyboard::C: return "C";
        case sf::Keyboard::D: return "D";
        case sf::Keyboard::E: return "E";
        case sf::Keyboard::F: return "F";
        case sf::Keyboard::G: return "G";
        case sf::Keyboard::H: return "H";
        case sf::Keyboard::I: return "I";
        case sf::Keyboard::J: return "J";
        case sf::Keyboard::K: return "K";
        case sf::Keyboard::L: return "L";
        case sf::Keyboard::M: return "M";
        case sf::Keyboard::N: return "N";
        case sf::Keyboard::O: return "O";
        case sf::Keyboard::P: return "P";
        case sf::Keyboard::Q: return "Q";
        case sf::Keyboard::R: return "R";
        case sf::Keyboard::S: return "S";
        case sf::Keyboard::T: return "T";
        case sf::Keyboard::U: return "U";
        case sf::Keyboard::V: return "V";
        case sf::Keyboard::W: return "W";
        case sf::Keyboard::X: return "X";
        case sf::Keyboard::Y: return "Y";
        case sf::Keyboard::Z: return "Z";
        case sf::Keyboard::Space: return "Space";
        case sf::Keyboard::Enter: return "Enter";
        case sf::Keyboard::Escape: return "Escape";
        case sf::Keyboard::LShift: return "Left Shift";
        case sf::Keyboard::RShift: return "Right Shift";
        case sf::Keyboard::LControl: return "Left Ctrl";
        case sf::Keyboard::RControl: return "Right Ctrl";
        case sf::Keyboard::LAlt: return "Left Alt";
        case sf::Keyboard::RAlt: return "Right Alt";
        default: return "Unknown";
    }
}

std::string Input::getMouseButtonName(MouseButton button) const {
    switch (button) {
        case MouseButton::Left: return "Left Mouse";
        case MouseButton::Right: return "Right Mouse";
        case MouseButton::Middle: return "Middle Mouse";
        case MouseButton::XButton1: return "Mouse Button 4";
        case MouseButton::XButton2: return "Mouse Button 5";
        default: return "Unknown Mouse Button";
    }
}

std::vector<KeyCode> Input::getPressedKeys() const {
    std::vector<KeyCode> keys;
    for (const auto& pair : _keyStates) {
        if (pair.second == InputState::Pressed || pair.second == InputState::Held) {
            keys.push_back(pair.first);
        }
    }
    return keys;
}

std::vector<MouseButton> Input::getPressedMouseButtons() const {
    std::vector<MouseButton> buttons;
    for (const auto& pair : _mouseStates) {
        if (pair.second == InputState::Pressed || pair.second == InputState::Held) {
            buttons.push_back(pair.first);
        }
    }
    return buttons;
}

void Input::updateInputState(KeyCode key, bool pressed) {
    if (pressed) {
        if (_keyStates.find(key) == _keyStates.end() || _keyStates[key] == InputState::Released) {
            _keyStates[key] = InputState::Pressed;
        }
    } else {
        _keyStates[key] = InputState::Released;
    }
}

void Input::updateInputState(MouseButton button, bool pressed) {
    if (pressed) {
        if (_mouseStates.find(button) == _mouseStates.end() || _mouseStates[button] == InputState::Released) {
            _mouseStates[button] = InputState::Pressed;
        }
    } else {
        _mouseStates[button] = InputState::Released;
    }
}

void Input::processActionCallbacks() {
    for (const auto& pair : _actionCallbacks) {
        if (isActionJustPressed(pair.first)) {
            pair.second();
        }
    }
}

InputState Input::getInputState(KeyCode key) const {
    auto it = _keyStates.find(key);
    return it != _keyStates.end() ? it->second : InputState::Released;
}

InputState Input::getInputState(MouseButton button) const {
    auto it = _mouseStates.find(button);
    return it != _mouseStates.end() ? it->second : InputState::Released;
}

} // namespace Core
} // namespace SFSim