#ifndef GAMEAPP_H
#define GAMEAPP_H

/////////////////////////////////
// STL and C/C++ lib includes  //
/////////////////////////////////
#include <string>
#include <unordered_map>
#include <list>
#include <memory>

/////////////////////////////////
//    3rd-party lib includes   //
/////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
  Class:    GameApp

  Summary:  Abstract class that serves as a simple game engine for
	    simple games. All games using this engine must be derived
	    classes and implement OnInitialize and OnUpdate. Uses
	    SFML to facilitate windowing and drawing.
 C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GameApp {
public:
    // Enums for common FPS locks
    enum class FPSControl {
	NONE = 0,
	Locked30 = 30,
	Locked60 = 60
    };

protected:
    // Provides a way to easily store keyboard presses
    enum class KeyStatus {
	NotPressed = 0,
	Pressed = 1,
	Released = 2
    };

private:
    // SFML specific objects to facilitate visuals, input, and timing
    sf::RenderWindow* m_pWindow = nullptr;
    sf::Event m_WindowEvent = sf::Event();
    sf::VideoMode m_WindowSize = sf::VideoMode(800, 600);
    sf::Clock m_AppClock;

    // Meta-data data structures
    std::string m_szWindowTitle;
    std::unordered_map<sf::Keyboard::Key, KeyStatus> m_hmKeys;
    std::unordered_map<int, std::pair<std::shared_ptr<sf::Sound>, bool>> m_hmSoundEffects;
    std::list<sf::Drawable*> m_llstDrawableObjects;
    std::list<const sf::Vertex*> m_llstLineVertices;

    FPSControl m_ctrlMode;

    // Subroutines that handle different aspects of the engine
    void ResetReleasedKeys();
    void HandleInput();
    void RefreshAndDisplay();
    void PlaySoundEffects();

    void LockFrameRate();

    // For managing time
    unsigned long long m_ullTimePoint1 = 0;
    unsigned long long m_ullTimePoint2 = 0;

    // This will hold the target frametime as required by the FPS control mode
    float m_fFrametime = 0.0f;

protected:
    // As stated earlier, derived class must implement these
    virtual bool OnInitialize() = 0;
    virtual bool OnUpdate(float fFrameTime) = 0;

    // Helpful utliliy functions that the derived class may 
    // want to use
    sf::VideoMode GetWindowSize();
    KeyStatus GetKeyStatus(sf::Keyboard::Key sfTestedKey);

    // This function is what the derived class will use when
    // it wants the engine to draw something
    void PushDrawableObject(sf::Drawable* psfDrawableObj);

    void MarkSoundForPlay(int nSfxNo);

    void RegisterSoundEffect(int nSfxNo, sf::SoundBuffer* psfSoundBuffer);

    void PushLines(const sf::Vertex* pVertices);

public:
    // Constructors, default title and FPS control is specified.
    GameApp(std::string szWindowTitle = "Default Title",
	FPSControl ctrlFpsControl = FPSControl::NONE);

    GameApp(sf::VideoMode sfWindowSize,
	std::string szWindowTitle = "Default Title",
	FPSControl ctrlFpsControl = FPSControl::NONE);

    ~GameApp();

    // Main function should call this to start the game
    int RunGame();
};

#endif
