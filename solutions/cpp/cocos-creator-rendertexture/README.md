# Cocos Creator 3.8 RenderTexture with Custom OpenGL Commands

This example demonstrates the integration of RenderTexture and custom OpenGL commands for a Win32 project exported from Cocos Creator 3.8. The implementation showcases how to:

1. Create and configure a RenderTexture
2. Implement custom OpenGL commands for drawing geometric shapes
3. Integrate custom rendering with the Cocos2d-x rendering pipeline
4. Display RenderTexture content in the scene

## Features

- **RenderTexture Management**: Create, configure, and manage RenderTexture objects
- **Custom OpenGL Commands**: Implement CustomCommand subclass for direct OpenGL rendering
- **Shader Programming**: Vertex and fragment shader compilation and linking
- **Vertex Buffer Objects**: Efficient vertex data management with VAO/VBO
- **Scene Integration**: Seamless integration with Cocos2d-x scene graph

## Prerequisites

- Cocos Creator 3.8 or later
- Visual Studio 2019/2022 (for Win32 development)
- CMake 3.16 or later
- Windows 10 SDK
- Cocos2d-x framework (included with Cocos Creator)

## Project Structure

```
cocos-creator-rendertexture/
├── RenderTextureExample.h      # Header file with class declarations
├── RenderTextureExample.cpp    # Implementation with detailed comments
├── CMakeLists.txt             # CMake build configuration
└── README.md                  # This documentation
```

## Implementation Details

### 1. RenderTexture Creation

```cpp
// Create a 512x512 RenderTexture with RGBA8888 format and depth/stencil buffer
_renderTexture = RenderTexture::create(512, 512, 
    Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);

// Configure clear color and auto-draw mode
_renderTexture->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
_renderTexture->setAutoDraw(true);
```

### 2. Custom OpenGL Command

The `TriangleDrawCommand` class demonstrates:

- Shader program creation and compilation
- Vertex buffer object setup
- Custom rendering execution

```cpp
class TriangleDrawCommand : public CustomCommand
{
private:
    GLuint _vao;        // Vertex Array Object
    GLuint _vbo;        // Vertex Buffer Object  
    GLuint _program;    // Shader program
    
public:
    virtual void execute() override
    {
        // Custom OpenGL rendering code
        glUseProgram(_program);
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};
```

### 3. Shader Implementation

The example includes complete vertex and fragment shaders:

**Vertex Shader:**
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

**Fragment Shader:**
```glsl
#version 330 core
out vec4 FragColor;
uniform vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
```

### 4. Rendering Pipeline Integration

```cpp
// Begin rendering to RenderTexture
_renderTexture->begin();

// Clear the render target
_renderTexture->clear(0.0f, 0.0f, 0.0f, 0.0f);

// Add custom command to renderer
Director::getInstance()->getRenderer()->addCommand(_triangleCmd);

// End rendering to RenderTexture
_renderTexture->end();
```

## Building the Project

### Option 1: Using CMake (Recommended)

1. **Set up environment variables:**
   ```bash
   set COCOS2DX_ROOT=C:\path\to\cocos2d-x
   ```

2. **Generate Visual Studio project:**
   ```bash
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" -A x64
   ```

3. **Build the project:**
   ```bash
   cmake --build . --config Release
   ```

### Option 2: Manual Integration

1. **Copy files to your Cocos Creator project:**
   - Copy `RenderTextureExample.h` and `RenderTextureExample.cpp` to your project's `Classes` folder

2. **Update your AppDelegate.cpp:**
   ```cpp
   #include "RenderTextureExample.h"
   
   bool AppDelegate::applicationDidFinishLaunching()
   {
       // ... initialization code ...
       
       // Create and run the RenderTexture example scene
       auto scene = RenderTextureExample::createScene();
       director->runWithScene(scene);
       
       return true;
   }
   ```

3. **Add to your project's build system:**
   - Include the source files in your Visual Studio project
   - Ensure OpenGL libraries are linked

## Usage

Once built and running, the application will:

1. **Initialize** a 512x512 RenderTexture
2. **Draw** a red triangle using custom OpenGL commands
3. **Display** the result as a Sprite in the center of the screen

The triangle is rendered directly to the RenderTexture using raw OpenGL calls, demonstrating the integration between Cocos2d-x's high-level rendering system and low-level OpenGL commands.

## Key Learning Points

### RenderTexture Benefits
- **Off-screen rendering**: Draw complex graphics without affecting the main framebuffer
- **Texture reuse**: Render once, use multiple times for performance optimization
- **Post-processing**: Apply effects to rendered content
- **Dynamic textures**: Create textures at runtime based on game state

### Custom OpenGL Commands
- **Direct control**: Access to full OpenGL functionality
- **Performance**: Optimize rendering for specific use cases
- **Integration**: Seamlessly work with Cocos2d-x rendering pipeline
- **Flexibility**: Implement any OpenGL rendering technique

### Memory Management
- **Resource cleanup**: Proper deletion of OpenGL objects
- **Retention**: Manage Cocos2d-x object lifetimes
- **RAII**: Use constructor/destructor patterns for resource management

## Troubleshooting

### Common Issues

1. **Shader compilation errors:**
   - Check OpenGL version compatibility
   - Verify shader syntax
   - Enable debug output for detailed error messages

2. **Black screen or no rendering:**
   - Ensure RenderTexture is properly created
   - Verify custom command is added to renderer
   - Check viewport and projection matrix settings

3. **Build errors:**
   - Confirm Cocos2d-x paths are correct
   - Verify all required libraries are linked
   - Check include directories

### Debug Tips

- Enable OpenGL debug output
- Use graphics debuggers (RenderDoc, Visual Studio Graphics Debugger)
- Add logging to track command execution
- Verify OpenGL state before and after custom commands

## Extensions

This example can be extended to include:

- **Multiple shapes**: Implement different geometric primitives
- **Texturing**: Apply textures to custom-drawn geometry
- **Animation**: Animate custom-drawn content over time
- **Post-processing**: Apply effects to RenderTexture content
- **UI integration**: Combine with Cocos Creator UI elements

## References

- [Cocos Creator 3.8 Documentation](https://docs.cocos.com/creator/3.8/)
- [Cocos2d-x Programming Guide](https://docs.cocos2d-x.org/cocos2d-x/v4/)
- [OpenGL Programming Guide](https://www.opengl.org/documentation/)
- [Learn OpenGL](https://learnopengl.com/)

## License

This example is provided for educational purposes and follows the same license as the awesome-low-level-design repository.