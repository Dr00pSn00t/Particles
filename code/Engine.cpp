#include "Engine.h"

// The Engine constructor
Engine::Engine()
{
    //create the window
    int pixelWidth = VideoMode::getDesktopMode().width / 2;
    int pixelHeight = VideoMode::getDesktopMode().height / 2;
    VideoMode vm(pixelWidth, pixelHeight);
    m_Window.create(vm, "Particles", Style::Default);
}

// Run will call all the private functions
void Engine::run()
{
    // Construct a local Clock object to track time per frame
    Clock clock;

    //Construct a local Particle to be used for the unit tests.
    //The tests will be given to you, and you can use them to check your progress as you go.
    //Use the following code exactly:
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;

    // Loop while m_Window is open
    while (m_Window.isOpen())
    {
        // Restart the clock (this will return the time elapsed since the last frame)
        clock.restart();

        // Call input, update, draw
        input();
        update(clock.getElapsedTime().asSeconds());
        draw();
    }
}

void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) m_Window.close();
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // construct a number of (5) particles
                for (int i = 0; i < 5; i++)
                {
                    m_particles.push_back(Particle(m_Window, rand() % 26 + 25, Vector2i(event.mouseButton.x, event.mouseButton.y)));
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {

            }
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        m_Window.close();
    }
}

void Engine::update(float dtAsSeconds)
{
    for (vector<Particle>::iterator iterator = m_particles.begin(); iterator != m_particles.end();)
    {
        if (iterator->getTTL() > 0.0)
        {
            iterator->update(dtAsSeconds);
            iterator++;
        }
        else
        {
            iterator = m_particles.erase(iterator);
        }
    }
}

void Engine::draw()
{
    m_Window.clear();
    for (Particle particle : m_particles)
    {
        m_Window.draw(particle);
    }
    m_Window.display();
}