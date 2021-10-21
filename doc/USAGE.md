# CSUGL Usage

[TOC]

## Core Example

### Use Intelligent pointer

Actually ``` csugl::Ref ``` is ``` std::shared_ptr``` and ```csugl::Scope``` is ```std::unique_pre``` in implementation

``` cpp
// create an referenceable pointer 
csugl::Ref<Object> obj = csugl::MakeRef<Object>(...);
```

``` cpp
// create an scope pointer
csugl::Scope<Object> obj = csugl::MakeScope<Object>(...);
```
``` cpp
// or with custom deleter
auto deleter = [](Object* data) {
    // release data with custom method
    // ...
};
csugl::Scope<Object, decltype(deleter)> obj(new Object(...), deleter);
```

### Create Window

``` cpp
// initialize the application and get instance
csugl::Ref<csugl::Application> app = csugl::singleton<csugl::Application>::getInstance();

// get window from app as a pointer
csugl::Window* window = &app->GetWindow();

while (app->isOpen()) {
    // use defualt frame buffer
    glBindFrameBuffer(0);
    glClear(...);
    
    // ...
    
    // glfw swap buffer
    window->Display();
}

// don't forget to destroy it
csugl::singleton<csugl::Application>::destroy();

```

### Add Event Callback

Use lambda expression as callback

``` cpp
window->addEventCallback([&](csugl::Event &ev) {
    csugl::EventDispatcher dispatcher(ev);
    // dispatch resize event
    dispatcher.Dispatch<csugl::WindowResizeEvent>([&](csugl::WindowResizeEvent &ev){
        // csugl::Event overload << operator to print info
        LOG(INFO) << ev;
        winSize = glm::ivec2{ev.width, ev.height};
        // true or false is both OK
        return false;
    });
    // then dispatch orther event
    dispatcher.Dispatch<csugl::KeyPressedEvent>([&](csugl::KeyPressedEvent &ev){
        LOG(INFO) << ev;
        // check the key pressed
        if(ev.keyCode == csugl::csugl::Key::A){
            // ...
        }
        return false;
    });
});
```

### Get Input

``` cpp
// if button0 is being pressed
if (csugl::Input::IsMouseButtonPressed(csugl::Mouse::Button0)) {
    // ...
}
// if A is being pressed
if(csugl::Input::IsKeyPressed(csugl::Key::A)) {
    // ...
}
// get mouse position
std::pair<int, int> mousePos = csugl::Input::GetMousePos();
```

## Render Example

### Create a Shader and use it

``` cpp
// load shader from file, of which name without space
auto shader = csugl::MakeRef<csugl::Shader>("myshader.glsl");

// use this shader program
shader->Use();
shader->SetVector3("_value", glm::vec3{1.0f, 1.0f, 1.0f});
```

### Create Vertex Buffer

TODO