#pragma once

#include "math/vector.hpp"
#include <SFML/Window.hpp>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

namespace SFSim {
namespace Core {

using namespace Math;

enum class InputState {
    Released,
    Pressed,
    Held
};

enum class MouseButton {
    Left = sf::Mouse::Left,
    Right = sf::Mouse::Right,
    Middle = sf::Mouse::Middle,
    XButton1 = sf::Mouse::XButton1,
    XButton2 = sf::Mouse::XButton2
};

using KeyCode = sf::Keyboard::Key;
using ActionCallback = std::function<void()>;

class Input {
public:
    static Input& getInstance();
    
    void initialize();
    void update();
    void handleEvent(const sf::Event& event);
    
    bool isKeyPressed(KeyCode key) const;
    bool isKeyJustPressed(KeyCode key) const;
    bool isKeyJustReleased(KeyCode key) const;
    
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonJustPressed(MouseButton button) const;
    bool isMouseButtonJustReleased(MouseButton button) const;
    
    Vector2f getMousePosition() const;
    Vector2f getMouseDelta() const;
    float getMouseWheelDelta() const;
    
    void setMousePosition(const Vector2f& position);
    void centerMouse(const sf::Window& window);
    
    void bindKeyAction(const std::string& actionName, KeyCode key);
    void bindMouseAction(const std::string& actionName, MouseButton button);
    void unbindAction(const std::string& actionName);
    
    void registerActionCallback(const std::string& actionName, ActionCallback callback);
    void unregisterActionCallback(const std::string& actionName);
    
    bool isActionPressed(const std::string& actionName) const;
    bool isActionJustPressed(const std::string& actionName) const;
    bool isActionJustReleased(const std::string& actionName) const;
    
    void enableMouseCapture(bool enable) { _mouseCapture = enable; }
    bool isMouseCaptureEnabled() const { return _mouseCapture; }
    
    void setMouseSensitivity(float sensitivity) { _mouseSensitivity = sensitivity; }
    float getMouseSensitivity() const { return _mouseSensitivity; }
    
    std::string getKeyName(KeyCode key) const;
    std::string getMouseButtonName(MouseButton button) const;
    
    std::vector<KeyCode> getPressedKeys() const;
    std::vector<MouseButton> getPressedMouseButtons() const;
    
private:
    Input();
    ~Input() = default;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    
    std::unordered_map<KeyCode, InputState> _keyStates;
    std::unordered_map<MouseButton, InputState> _mouseStates;
    
    Vector2f _mousePosition;
    Vector2f _lastMousePosition;
    Vector2f _mouseDelta;
    float _mouseWheelDelta;
    
    bool _mouseCapture;
    float _mouseSensitivity;
    
    struct Action {
        enum Type { Key, Mouse } type;
        union {
            KeyCode key;
            MouseButton mouse;
        };
        
        Action(KeyCode k) : type(Key), key(k) {}
        Action(MouseButton m) : type(Mouse), mouse(m) {}
    };
    
    std::unordered_map<std::string, Action> _actionBindings;
    std::unordered_map<std::string, ActionCallback> _actionCallbacks;
    
    void updateInputState(KeyCode key, bool pressed);
    void updateInputState(MouseButton button, bool pressed);
    void processActionCallbacks();
    
    InputState getInputState(KeyCode key) const;
    InputState getInputState(MouseButton button) const;
};

} // namespace Core
} // namespace SFSim