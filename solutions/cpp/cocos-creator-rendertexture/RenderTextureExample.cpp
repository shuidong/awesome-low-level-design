#include "cocos2d.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderCommand.h"
#include "renderer/CCCustomCommand.h"

USING_NS_CC;

/**
 * RenderTextureExample demonstrates the integration of RenderTexture with custom OpenGL commands
 * for a Win32 project exported from Cocos Creator 3.8.
 * 
 * This example covers:
 * 1. Creating and configuring a RenderTexture
 * 2. Implementing custom OpenGL commands to draw a triangle
 * 3. Rendering the RenderTexture content to a Sprite in the scene
 */
class RenderTextureExample : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    
    // Custom OpenGL command class for drawing a triangle
    class TriangleDrawCommand : public CustomCommand
    {
    public:
        TriangleDrawCommand();
        virtual ~TriangleDrawCommand();
        
        void init(float width, float height);
        virtual void execute() override;
        
    private:
        GLuint _vao;        // Vertex Array Object
        GLuint _vbo;        // Vertex Buffer Object
        GLuint _program;    // Shader program
        GLint _positionLoc; // Position attribute location
        GLint _colorLoc;    // Color uniform location
        
        // Vertex data for a simple triangle
        float _vertices[9] = {
            -0.5f, -0.5f, 0.0f,  // Bottom left
             0.5f, -0.5f, 0.0f,  // Bottom right
             0.0f,  0.5f, 0.0f   // Top center
        };
        
        void createShaderProgram();
        void setupVertexData();
    };
    
    CREATE_FUNC(RenderTextureExample);
    
private:
    RenderTexture* _renderTexture;      // The RenderTexture we'll draw to
    Sprite* _renderTextureSprite;       // Sprite to display the RenderTexture
    TriangleDrawCommand* _triangleCmd;  // Custom command for triangle drawing
    
    void createRenderTexture();
    void setupRenderTextureSprite();
    void drawTriangleToRenderTexture();
};

Scene* RenderTextureExample::createScene()
{
    return RenderTextureExample::create();
}

bool RenderTextureExample::init()
{
    // Step 1: Initialize the base Scene class
    if (!Scene::init())
    {
        return false;
    }
    
    // Step 2: Create and configure the RenderTexture
    createRenderTexture();
    
    // Step 3: Create a Sprite to display the RenderTexture content
    setupRenderTextureSprite();
    
    // Step 4: Draw custom OpenGL content (triangle) to the RenderTexture
    drawTriangleToRenderTexture();
    
    return true;
}

void RenderTextureExample::createRenderTexture()
{
    // Step 2.1: Create a RenderTexture with specified dimensions
    // Parameters: width, height, pixel format, depth stencil format
    _renderTexture = RenderTexture::create(512, 512, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    
    if (_renderTexture)
    {
        // Step 2.2: Retain the RenderTexture to prevent auto-release
        _renderTexture->retain();
        
        // Step 2.3: Set clear color for the RenderTexture (transparent background)
        _renderTexture->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
        
        // Step 2.4: Enable auto-draw mode for easier rendering
        _renderTexture->setAutoDraw(true);
        
        CCLOG("RenderTexture created successfully with dimensions: 512x512");
    }
    else
    {
        CCLOG("Failed to create RenderTexture");
    }
}

void RenderTextureExample::setupRenderTextureSprite()
{
    // Step 3.1: Create a Sprite using the RenderTexture as its texture
    if (_renderTexture)
    {
        _renderTextureSprite = Sprite::createWithTexture(_renderTexture->getSprite()->getTexture());
        
        if (_renderTextureSprite)
        {
            // Step 3.2: Position the Sprite at the center of the screen
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            _renderTextureSprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                                 origin.y + visibleSize.height / 2));
            
            // Step 3.3: Scale the Sprite to fit nicely on screen
            _renderTextureSprite->setScale(0.8f);
            
            // Step 3.4: Add the Sprite to the scene
            this->addChild(_renderTextureSprite, 1);
            
            CCLOG("RenderTexture Sprite created and added to scene");
        }
    }
}

void RenderTextureExample::drawTriangleToRenderTexture()
{
    // Step 4.1: Begin drawing to the RenderTexture
    _renderTexture->begin();
    
    // Step 4.2: Clear the RenderTexture with transparent background
    _renderTexture->clear(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Step 4.3: Create and configure the custom triangle drawing command
    _triangleCmd = new TriangleDrawCommand();
    _triangleCmd->init(512.0f, 512.0f);
    
    // Step 4.4: Add the custom command to the renderer
    Director::getInstance()->getRenderer()->addCommand(_triangleCmd);
    
    // Step 4.5: End drawing to the RenderTexture
    _renderTexture->end();
    
    CCLOG("Triangle drawn to RenderTexture using custom OpenGL commands");
}

// TriangleDrawCommand Implementation

RenderTextureExample::TriangleDrawCommand::TriangleDrawCommand()
    : _vao(0), _vbo(0), _program(0), _positionLoc(-1), _colorLoc(-1)
{
    setType(RenderCommand::Type::CUSTOM_COMMAND);
}

RenderTextureExample::TriangleDrawCommand::~TriangleDrawCommand()
{
    // Clean up OpenGL resources
    if (_vao != 0)
    {
        glDeleteVertexArrays(1, &_vao);
    }
    if (_vbo != 0)
    {
        glDeleteBuffers(1, &_vbo);
    }
    if (_program != 0)
    {
        glDeleteProgram(_program);
    }
}

void RenderTextureExample::TriangleDrawCommand::init(float width, float height)
{
    // Initialize OpenGL resources for triangle rendering
    createShaderProgram();
    setupVertexData();
    
    // Set the viewport for rendering
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void RenderTextureExample::TriangleDrawCommand::createShaderProgram()
{
    // Vertex shader source code
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    
    // Fragment shader source code
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 ourColor;
        
        void main()
        {
            FragColor = vec4(ourColor, 1.0);
        }
    )";
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        CCLOG("Vertex shader compilation failed: %s", infoLog);
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        CCLOG("Fragment shader compilation failed: %s", infoLog);
    }
    
    // Create and link shader program
    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);
    
    // Check program linking
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        CCLOG("Shader program linking failed: %s", infoLog);
    }
    
    // Clean up individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Get attribute and uniform locations
    _positionLoc = glGetAttribLocation(_program, "aPos");
    _colorLoc = glGetUniformLocation(_program, "ourColor");
}

void RenderTextureExample::TriangleDrawCommand::setupVertexData()
{
    // Generate and bind Vertex Array Object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    // Generate and bind Vertex Buffer Object
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
    
    // Configure vertex attribute pointer
    glVertexAttribPointer(_positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(_positionLoc);
    
    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderTextureExample::TriangleDrawCommand::execute()
{
    // Custom OpenGL drawing execution
    
    // Step 1: Use our custom shader program
    glUseProgram(_program);
    
    // Step 2: Set triangle color (red)
    glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f);
    
    // Step 3: Bind vertex array and draw the triangle
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Step 4: Unbind vertex array
    glBindVertexArray(0);
    
    // Step 5: Reset to default shader program
    glUseProgram(0);
}

/**
 * Usage Instructions:
 * 
 * 1. Include this file in your Cocos Creator 3.8 Win32 project
 * 2. In your AppDelegate.cpp, replace the default scene creation with:
 *    auto scene = RenderTextureExample::createScene();
 *    director->runWithScene(scene);
 * 
 * 3. Ensure your project links with OpenGL32.lib and has proper Cocos2d-x headers
 * 
 * 4. The example will:
 *    - Create a 512x512 RenderTexture
 *    - Draw a red triangle using custom OpenGL commands
 *    - Display the result as a Sprite in the center of the screen
 * 
 * Key Features Demonstrated:
 * - RenderTexture creation and configuration
 * - Custom OpenGL command implementation
 * - Vertex buffer and shader program management
 * - Integration with Cocos2d-x rendering pipeline
 * - Proper resource cleanup and memory management
 */