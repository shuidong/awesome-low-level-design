#ifndef __RENDER_TEXTURE_EXAMPLE_H__
#define __RENDER_TEXTURE_EXAMPLE_H__

#include "cocos2d.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderCommand.h"
#include "renderer/CCCustomCommand.h"

USING_NS_CC;

/**
 * RenderTextureExample - Header file
 * 
 * Demonstrates integration of RenderTexture with custom OpenGL commands
 * for Cocos Creator 3.8 Win32 projects.
 * 
 * Features:
 * - RenderTexture creation and management
 * - Custom OpenGL triangle rendering
 * - Shader program compilation and linking
 * - Vertex buffer object management
 * - Integration with Cocos2d-x scene graph
 */
class RenderTextureExample : public Scene
{
public:
    /**
     * Factory method to create the scene
     * @return Scene* pointer to the created scene
     */
    static Scene* createScene();
    
    /**
     * Initialize the scene and all its components
     * @return bool true if initialization succeeded, false otherwise
     */
    virtual bool init() override;
    
    /**
     * Custom OpenGL command class for drawing geometric shapes
     * Encapsulates OpenGL state and rendering operations
     */
    class TriangleDrawCommand : public CustomCommand
    {
    public:
        TriangleDrawCommand();
        virtual ~TriangleDrawCommand();
        
        /**
         * Initialize the OpenGL resources for triangle rendering
         * @param width The render target width
         * @param height The render target height
         */
        void init(float width, float height);
        
        /**
         * Execute the custom OpenGL drawing commands
         * Called by the Cocos2d-x renderer during the render pass
         */
        virtual void execute() override;
        
    private:
        // OpenGL resource handles
        GLuint _vao;        ///< Vertex Array Object handle
        GLuint _vbo;        ///< Vertex Buffer Object handle
        GLuint _program;    ///< Shader program handle
        GLint _positionLoc; ///< Position attribute location
        GLint _colorLoc;    ///< Color uniform location
        
        /// Vertex data for triangle (3 vertices, 3 components each)
        float _vertices[9];
        
        /**
         * Create and compile the shader program
         * Includes vertex and fragment shader compilation and linking
         */
        void createShaderProgram();
        
        /**
         * Set up vertex array and buffer objects
         * Configures vertex attribute pointers
         */
        void setupVertexData();
    };
    
    // Cocos2d-x macro for automatic create() method generation
    CREATE_FUNC(RenderTextureExample);
    
private:
    // Core components
    RenderTexture* _renderTexture;      ///< Render target for custom drawing
    Sprite* _renderTextureSprite;       ///< Sprite to display RenderTexture content
    TriangleDrawCommand* _triangleCmd;  ///< Custom OpenGL drawing command
    
    /**
     * Create and configure the RenderTexture
     * Sets up dimensions, pixel format, and clear color
     */
    void createRenderTexture();
    
    /**
     * Create a Sprite to display the RenderTexture content
     * Positions and scales the sprite for optimal display
     */
    void setupRenderTextureSprite();
    
    /**
     * Execute custom OpenGL drawing to the RenderTexture
     * Demonstrates the integration of custom commands with RenderTexture
     */
    void drawTriangleToRenderTexture();
};

#endif // __RENDER_TEXTURE_EXAMPLE_H__