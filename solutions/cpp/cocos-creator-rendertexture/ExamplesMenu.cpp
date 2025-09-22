#include "RenderTextureExample.h"
#include "AdvancedRenderTextureExample.h"

/**
 * ExamplesMenu - A simple menu scene to choose between different RenderTexture examples
 * 
 * This file demonstrates how to integrate the RenderTexture examples into a Cocos Creator 3.8 project.
 * It provides a menu system to switch between:
 * 1. Basic RenderTexture Example - Simple triangle rendering
 * 2. Advanced RenderTexture Example - Multiple shapes with animation
 */
class ExamplesMenu : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    
    // Menu callbacks
    void onBasicExampleClicked(Ref* sender);
    void onAdvancedExampleClicked(Ref* sender);
    void onBackToMenuClicked(Ref* sender);
    
    CREATE_FUNC(ExamplesMenu);
    
private:
    void createMenu();
    void createTitle();
    void createInstructions();
};

Scene* ExamplesMenu::createScene()
{
    return ExamplesMenu::create();
}

bool ExamplesMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    // Create title
    createTitle();
    
    // Create menu system
    createMenu();
    
    // Create instructions
    createInstructions();
    
    return true;
}

void ExamplesMenu::createTitle()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Main title
    auto titleLabel = Label::createWithTTF("Cocos Creator 3.8\nRenderTexture Examples", 
                                          "fonts/arial.ttf", 36);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height - 100));
    titleLabel->setTextColor(Color4B::WHITE);
    titleLabel->setAlignment(TextHAlignment::CENTER);
    this->addChild(titleLabel, 1);
    
    // Subtitle
    auto subtitleLabel = Label::createWithTTF("Custom OpenGL Commands Integration", 
                                            "fonts/arial.ttf", 24);
    subtitleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                  origin.y + visibleSize.height - 150));
    subtitleLabel->setTextColor(Color4B(200, 200, 200, 255));
    subtitleLabel->setAlignment(TextHAlignment::CENTER);
    this->addChild(subtitleLabel, 1);
}

void ExamplesMenu::createMenu()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Basic Example Button
    auto basicExampleLabel = Label::createWithTTF("Basic RenderTexture Example", 
                                                "fonts/arial.ttf", 28);
    auto basicExampleItem = MenuItemLabel::create(basicExampleLabel,
                                                CC_CALLBACK_1(ExamplesMenu::onBasicExampleClicked, this));
    basicExampleItem->setPosition(Vec2(0, 50));
    
    // Advanced Example Button
    auto advancedExampleLabel = Label::createWithTTF("Advanced RenderTexture Example", 
                                                   "fonts/arial.ttf", 28);
    auto advancedExampleItem = MenuItemLabel::create(advancedExampleLabel,
                                                   CC_CALLBACK_1(ExamplesMenu::onAdvancedExampleClicked, this));
    advancedExampleItem->setPosition(Vec2(0, -20));
    
    // Create menu
    auto menu = Menu::create(basicExampleItem, advancedExampleItem, nullptr);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 2));
    this->addChild(menu, 1);
}

void ExamplesMenu::createInstructions()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    std::string instructions = 
        "Features Demonstrated:\n\n"
        "Basic Example:\n"
        "• RenderTexture creation and configuration\n"
        "• Custom OpenGL command implementation\n"
        "• Simple triangle rendering with shaders\n"
        "• Resource management and cleanup\n\n"
        "Advanced Example:\n"
        "• Multiple RenderTextures for layered effects\n"
        "• Different shape primitives (triangle, quad, circle)\n"
        "• Real-time animation and color interpolation\n"
        "• Texture compositing and blending\n\n"
        "Press ESC to return to menu from examples";
    
    auto instructionsLabel = Label::createWithTTF(instructions, "fonts/arial.ttf", 16);
    instructionsLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                      origin.y + 200));
    instructionsLabel->setTextColor(Color4B(180, 180, 180, 255));
    instructionsLabel->setAlignment(TextHAlignment::CENTER);
    instructionsLabel->setMaxLineWidth(visibleSize.width * 0.8f);
    this->addChild(instructionsLabel, 1);
}

void ExamplesMenu::onBasicExampleClicked(Ref* sender)
{
    CCLOG("Switching to Basic RenderTexture Example");
    
    // Create and run basic example scene
    auto scene = RenderTextureExample::createScene();
    
    // Add back button to basic example
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto backLabel = Label::createWithTTF("Back to Menu (ESC)", "fonts/arial.ttf", 20);
    auto backItem = MenuItemLabel::create(backLabel,
                                        CC_CALLBACK_1(ExamplesMenu::onBackToMenuClicked, this));
    backItem->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 50));
    
    auto backMenu = Menu::create(backItem, nullptr);
    backMenu->setPosition(Vec2::ZERO);
    scene->addChild(backMenu, 100);
    
    // Transition to scene
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void ExamplesMenu::onAdvancedExampleClicked(Ref* sender)
{
    CCLOG("Switching to Advanced RenderTexture Example");
    
    // Create and run advanced example scene
    auto scene = AdvancedRenderTextureExample::createScene();
    
    // Add back button to advanced example
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto backLabel = Label::createWithTTF("Back to Menu (ESC)", "fonts/arial.ttf", 20);
    auto backItem = MenuItemLabel::create(backLabel,
                                        CC_CALLBACK_1(ExamplesMenu::onBackToMenuClicked, this));
    backItem->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 50));
    
    auto backMenu = Menu::create(backItem, nullptr);
    backMenu->setPosition(Vec2::ZERO);
    scene->addChild(backMenu, 100);
    
    // Transition to scene
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void ExamplesMenu::onBackToMenuClicked(Ref* sender)
{
    CCLOG("Returning to Examples Menu");
    
    // Return to menu scene
    auto scene = ExamplesMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

/**
 * Integration Instructions for AppDelegate.cpp:
 * 
 * 1. Include this file in your AppDelegate.cpp:
 *    #include "ExamplesMenu.h"
 * 
 * 2. In applicationDidFinishLaunching(), replace scene creation with:
 * 
 *    bool AppDelegate::applicationDidFinishLaunching()
 *    {
 *        auto director = Director::getInstance();
 *        auto glview = director->getOpenGLView();
 *        
 *        if (!glview) {
 *            glview = GLViewImpl::create("Cocos RenderTexture Examples");
 *            director->setOpenGLView(glview);
 *        }
 *        
 *        // Set design resolution
 *        glview->setDesignResolutionSize(1024, 768, ResolutionPolicy::NO_BORDER);
 *        
 *        // Enable OpenGL debug context (optional, for development)
 *        #ifdef COCOS2D_DEBUG
 *        director->setDisplayStats(true);
 *        director->setAnimationInterval(1.0f / 60);
 *        #endif
 *        
 *        // Create and run the examples menu scene
 *        auto scene = ExamplesMenu::createScene();
 *        director->runWithScene(scene);
 *        
 *        return true;
 *    }
 * 
 * 3. Make sure to include necessary headers in your project:
 *    - cocos2d.h
 *    - All OpenGL headers (GL/gl.h, GL/glext.h)
 *    - Platform-specific OpenGL loader (if needed)
 * 
 * 4. Link required libraries:
 *    - cocos2d.lib
 *    - opengl32.lib
 *    - glu32.lib (optional)
 *    - Platform-specific libraries
 * 
 * Key Benefits of This Integration:
 * 
 * • Educational Value: Clear separation of basic and advanced concepts
 * • Reusability: Easy to extract individual examples for other projects
 * • Extensibility: Simple to add new RenderTexture examples
 * • User Experience: Interactive menu system for exploring features
 * • Documentation: Comprehensive inline comments and instructions
 */