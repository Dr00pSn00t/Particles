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
        // Call input, update, draw
        input();
        update(clock.restart().asSeconds());
        draw();
    }
}

// Poll the Windows event queue 
void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {

        // Handle the Escape key pressed and closed events so your program can exit
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            m_Window.close();
        }
        if (event.type == Event::Closed) m_Window.close();
        if (event.type == Event::MouseButtonPressed)
        {
            // Handle the left mouse button pressed event 
            if (event.mouseButton.button == Mouse::Left)
            {
                // construct a number of (5) particles
                for (int i = 0; i < rand() % 10 + 8; i++)
                {
                    // numPoints is a random number in the range [25:50] (you can experiment with this too)
                    // Pass the position of the mouse click into the constructor
                    m_particles.push_back(Particle(m_Window, rand() % 40 + 45, Vector2i(event.mouseButton.x, event.mouseButton.y)));
                }
            }
        }
    }
}

// The general idea here is to loop through m_particles and call update on each Particle in the vector whose ttl (time to live) has not expired
    // If a particle's ttl has expired, it must be erased from the vector
void Engine::update(float dtAsSeconds)
{
    // This is best done with an iterator - based for - loop
        // Don't automatically increment the iterator for each iteration
    for (vector<Particle>::iterator iterator = m_particles.begin(); iterator != m_particles.end();)
    {
        // if getTTL() > 0.0 
        if (iterator->getTTL() > 0.0)
        {   
            // Call update on that Particle
            iterator->update(dtAsSeconds);
            // increment the iterator
            iterator++;
        }
        else // else
        {
            // erase the element the iterator points to
            // erase returns an iterator that points to the next element after deletion, or end if it is the end of the vector
                // Assign the iterator to this return value
            iterator = m_particles.erase(iterator);

            // Do not increment the iterator (if you do you might increment past the end of the vector after you delete the last element)
        }
    }
}

void Engine::draw()
{
    // clear the window
    m_Window.clear();

    // Loop through each Particle in m_Particles 
    for (Particle particle : m_particles)
    {
        // Pass each element into m_Window.draw()
        // Note:  This will use polymorphism to call your Particle::draw() function
        m_Window.draw(particle);
    }

    // display the window
    m_Window.display();
}