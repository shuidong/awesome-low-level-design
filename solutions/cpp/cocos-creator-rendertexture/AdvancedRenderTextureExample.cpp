#include "cocos2d.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderCommand.h"
#include "renderer/CCCustomCommand.h"

USING_NS_CC;

/**
 * AdvancedRenderTextureExample - Extended implementation demonstrating:
 * 1. Multiple custom OpenGL shapes (triangle, quad, circle)
 * 2. Animation and color interpolation
 * 3. Multiple RenderTextures for layered effects
 * 4. Texture sampling and post-processing
 */
class AdvancedRenderTextureExample : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;
    
    // Multi-shape custom command supporting different primitives
    class MultiShapeCommand : public CustomCommand
    {
    public:
        enum ShapeType {
            TRIANGLE,
            QUAD,
            CIRCLE
        };
        
        MultiShapeCommand(ShapeType type);
        virtual ~MultiShapeCommand();
        
        void init(float width, float height);
        void setColor(const Color3F& color);
        void setTransform(const Mat4& transform);
        virtual void execute() override;
        
    private:
        ShapeType _shapeType;
        GLuint _vao, _vbo, _ebo;  // Element buffer for indexed drawing
        GLuint _program;
        GLint _positionLoc, _colorLoc, _mvpLoc;
        
        Color3F _color;
        Mat4 _transform;
        
        std::vector<float> _vertices;
        std::vector<unsigned int> _indices;
        
        void generateTriangleData();
        void generateQuadData();
        void generateCircleData();
        void createAdvancedShaderProgram();
        void setupVertexData();
    };
    
    CREATE_FUNC(AdvancedRenderTextureExample);
    
private:
    // Multiple render targets for layered effects
    RenderTexture* _backgroundRT;
    RenderTexture* _shapesRT;
    RenderTexture* _finalRT;
    
    // Display sprites
    Sprite* _finalSprite;
    
    // Custom commands for different shapes
    MultiShapeCommand* _triangleCmd;
    MultiShapeCommand* _quadCmd;
    MultiShapeCommand* _circleCmd;
    
    // Animation state
    float _animationTime;
    
    void createRenderTextures();
    void setupDisplaySprite();
    void createShapeCommands();
    void renderAnimatedShapes(float dt);
    void compositeRenderTextures();
};

Scene* AdvancedRenderTextureExample::createScene()
{
    return AdvancedRenderTextureExample::create();
}

bool AdvancedRenderTextureExample::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    // Initialize animation time
    _animationTime = 0.0f;
    
    // Create multiple render targets
    createRenderTextures();
    
    // Set up final display sprite
    setupDisplaySprite();
    
    // Create shape rendering commands
    createShapeCommands();
    
    // Enable per-frame updates for animation
    this->scheduleUpdate();
    
    return true;
}

void AdvancedRenderTextureExample::update(float dt)
{
    // Update animation time
    _animationTime += dt;
    
    // Render animated shapes to multiple render targets
    renderAnimatedShapes(dt);
    
    // Composite all render textures into final result
    compositeRenderTextures();
}

void AdvancedRenderTextureExample::createRenderTextures()
{
    // Background render texture for static elements
    _backgroundRT = RenderTexture::create(512, 512, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _backgroundRT->retain();
    _backgroundRT->setClearColor(Color4F(0.1f, 0.1f, 0.2f, 1.0f));  // Dark blue background
    
    // Shapes render texture for animated geometry
    _shapesRT = RenderTexture::create(512, 512, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _shapesRT->retain();
    _shapesRT->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 0.0f));  // Transparent
    
    // Final composite render texture
    _finalRT = RenderTexture::create(512, 512, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _finalRT->retain();
    _finalRT->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 1.0f));
    
    CCLOG("Advanced RenderTextures created successfully");
}

void AdvancedRenderTextureExample::setupDisplaySprite()
{
    if (_finalRT)
    {
        _finalSprite = Sprite::createWithTexture(_finalRT->getSprite()->getTexture());
        
        if (_finalSprite)
        {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            _finalSprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                         origin.y + visibleSize.height / 2));
            _finalSprite->setScale(0.9f);
            
            this->addChild(_finalSprite, 1);
            CCLOG("Final display sprite created");
        }
    }
}

void AdvancedRenderTextureExample::createShapeCommands()
{
    // Create different shape commands
    _triangleCmd = new MultiShapeCommand(MultiShapeCommand::TRIANGLE);
    _triangleCmd->init(512.0f, 512.0f);
    
    _quadCmd = new MultiShapeCommand(MultiShapeCommand::QUAD);
    _quadCmd->init(512.0f, 512.0f);
    
    _circleCmd = new MultiShapeCommand(MultiShapeCommand::CIRCLE);
    _circleCmd->init(512.0f, 512.0f);
    
    CCLOG("Shape commands created");
}

void AdvancedRenderTextureExample::renderAnimatedShapes(float dt)
{
    // Render to shapes render texture
    _shapesRT->begin();
    _shapesRT->clear(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Animate triangle (rotating red triangle)
    Mat4 triangleTransform;
    Mat4::createRotationZ(_animationTime, &triangleTransform);
    Mat4::createTranslation(Vec3(-100, 100, 0), &triangleTransform);
    _triangleCmd->setTransform(triangleTransform);
    _triangleCmd->setColor(Color3F(1.0f, 0.0f, 0.0f));  // Red
    Director::getInstance()->getRenderer()->addCommand(_triangleCmd);
    
    // Animate quad (scaling green quad)
    float scale = 0.5f + 0.3f * sin(_animationTime * 2.0f);
    Mat4 quadTransform;
    Mat4::createScale(scale, scale, 1.0f, &quadTransform);
    Mat4::createTranslation(Vec3(100, 100, 0), &quadTransform);
    _quadCmd->setTransform(quadTransform);
    _quadCmd->setColor(Color3F(0.0f, 1.0f, 0.0f));  // Green
    Director::getInstance()->getRenderer()->addCommand(_quadCmd);
    
    // Animate circle (orbiting blue circle)
    float orbitRadius = 80.0f;
    float orbitX = orbitRadius * cos(_animationTime);
    float orbitY = orbitRadius * sin(_animationTime);
    Mat4 circleTransform;
    Mat4::createTranslation(Vec3(orbitX, orbitY - 100, 0), &circleTransform);
    _circleCmd->setTransform(circleTransform);
    
    // Color cycling for circle
    float colorPhase = _animationTime * 3.0f;
    Color3F circleColor(
        0.5f + 0.5f * sin(colorPhase),
        0.5f + 0.5f * sin(colorPhase + 2.0f),
        0.5f + 0.5f * sin(colorPhase + 4.0f)
    );
    _circleCmd->setColor(circleColor);
    Director::getInstance()->getRenderer()->addCommand(_circleCmd);
    
    _shapesRT->end();
}

void AdvancedRenderTextureExample::compositeRenderTextures()
{
    // Composite background and shapes into final render texture
    _finalRT->begin();
    _finalRT->clear(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Draw background
    auto bgSprite = Sprite::createWithTexture(_backgroundRT->getSprite()->getTexture());
    bgSprite->setPosition(Vec2(256, 256));
    bgSprite->visit();
    
    // Draw shapes with blend mode
    auto shapesSprite = Sprite::createWithTexture(_shapesRT->getSprite()->getTexture());
    shapesSprite->setPosition(Vec2(256, 256));
    shapesSprite->setBlendFunc(BlendFunc::ADDITIVE);
    shapesSprite->visit();
    
    _finalRT->end();
}

// MultiShapeCommand Implementation

AdvancedRenderTextureExample::MultiShapeCommand::MultiShapeCommand(ShapeType type)
    : _shapeType(type), _vao(0), _vbo(0), _ebo(0), _program(0)
    , _positionLoc(-1), _colorLoc(-1), _mvpLoc(-1)
    , _color(1.0f, 1.0f, 1.0f)
{
    setType(RenderCommand::Type::CUSTOM_COMMAND);
    Mat4::createIdentity(&_transform);
}

AdvancedRenderTextureExample::MultiShapeCommand::~MultiShapeCommand()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ebo != 0) glDeleteBuffers(1, &_ebo);
    if (_program != 0) glDeleteProgram(_program);
}

void AdvancedRenderTextureExample::MultiShapeCommand::init(float width, float height)
{
    // Generate shape-specific vertex data
    switch (_shapeType)
    {
        case TRIANGLE:
            generateTriangleData();
            break;
        case QUAD:
            generateQuadData();
            break;
        case CIRCLE:
            generateCircleData();
            break;
    }
    
    createAdvancedShaderProgram();
    setupVertexData();
    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void AdvancedRenderTextureExample::MultiShapeCommand::generateTriangleData()
{
    _vertices = {
        -30.0f, -30.0f, 0.0f,  // Bottom left
         30.0f, -30.0f, 0.0f,  // Bottom right
         0.0f,   30.0f, 0.0f   // Top center
    };
    
    _indices = { 0, 1, 2 };
}

void AdvancedRenderTextureExample::MultiShapeCommand::generateQuadData()
{
    _vertices = {
        -25.0f, -25.0f, 0.0f,  // Bottom left
         25.0f, -25.0f, 0.0f,  // Bottom right
         25.0f,  25.0f, 0.0f,  // Top right
        -25.0f,  25.0f, 0.0f   // Top left
    };
    
    _indices = { 0, 1, 2, 2, 3, 0 };
}

void AdvancedRenderTextureExample::MultiShapeCommand::generateCircleData()
{
    const int segments = 32;
    const float radius = 20.0f;
    
    // Center vertex
    _vertices.push_back(0.0f);
    _vertices.push_back(0.0f);
    _vertices.push_back(0.0f);
    
    // Circle vertices
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        _vertices.push_back(radius * cos(angle));
        _vertices.push_back(radius * sin(angle));
        _vertices.push_back(0.0f);
    }
    
    // Triangle indices for circle
    for (int i = 0; i < segments; ++i)
    {
        _indices.push_back(0);           // Center
        _indices.push_back(i + 1);       // Current vertex
        _indices.push_back(i + 2);       // Next vertex
    }
}

void AdvancedRenderTextureExample::MultiShapeCommand::createAdvancedShaderProgram()
{
    // Advanced vertex shader with MVP matrix
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 uMVP;
        
        void main()
        {
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )";
    
    // Fragment shader with uniform color
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        
        void main()
        {
            FragColor = vec4(uColor, 1.0);
        }
    )";
    
    // Compile and link shaders (implementation similar to basic example)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Get uniform locations
    _positionLoc = glGetAttribLocation(_program, "aPos");
    _colorLoc = glGetUniformLocation(_program, "uColor");
    _mvpLoc = glGetUniformLocation(_program, "uMVP");
}

void AdvancedRenderTextureExample::MultiShapeCommand::setupVertexData()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), 
                 _vertices.data(), GL_STATIC_DRAW);
    
    if (!_indices.empty())
    {
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
                     _indices.data(), GL_STATIC_DRAW);
    }
    
    glVertexAttribPointer(_positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(_positionLoc);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AdvancedRenderTextureExample::MultiShapeCommand::setColor(const Color3F& color)
{
    _color = color;
}

void AdvancedRenderTextureExample::MultiShapeCommand::setTransform(const Mat4& transform)
{
    _transform = transform;
}

void AdvancedRenderTextureExample::MultiShapeCommand::execute()
{
    glUseProgram(_program);
    
    // Set color uniform
    glUniform3f(_colorLoc, _color.r, _color.g, _color.b);
    
    // Set MVP matrix uniform
    glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, _transform.m);
    
    // Draw the shape
    glBindVertexArray(_vao);
    if (!_indices.empty())
    {
        glDrawElements(GL_TRIANGLES, (GLsizei)_indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(_vertices.size() / 3));
    }
    glBindVertexArray(0);
    
    glUseProgram(0);
}