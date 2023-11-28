#include "Particle.h"

/*
* - This constructor will be responsible for generating a randomized shape with numPoints vertices,
* centered on mouseClickPosition mapped to the Cartesian plane,
* which is centered at(0, 0) instead of(width / 2, height / 2).
*     - It will also give the particle initial vertical, horizontal, and angular / spin velocities
*     - Each vertex will be stored in the Matrix member variable m_A as a column vector
* 
* - The Matrix member variable m_A does not have a default constructor and thus must 
* be constructed in an initialization list before the code for the constructor begins:
*     - Particle(...) : m_A(2, numPoints)
*/
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{
    // Initialize m_ttl with the global constant TTL, which gives it a time to live of 5 seconds
    m_ttl = TTL;

    // Initialize m_numPoints with numPoints
    m_numPoints = numPoints;

    // Initialize m_radiansPerSec to a random angular velocity in the range [0:PI]
        // You can generate a random fraction between [0:1) by calling (float)rand() / (RAND_MAX)
        // Multiply this fraction by PI (this is a constant for pi, declared in Particle.h
    m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI;

    /* - m_cartesianPlane will be used to map between monitor pixel coordinates, that are 
    *  centered at (960, 540) for a 1080p monitor, to Cartesian coordinates that are centered 
    *  about the origin (0,0), so our Matrix algebra will work correctly
    */
    // Call setCenter(0,0)
    m_cartesianPlane.setCenter(0, 0);

    // Call setSize(target.getSize().x, (-1.0) * target.getSize().y)
        // This will initialize its width and height to the size of the RenderWindow stored in target and invert the y - axis
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);

    // Store the location of the center of this particle on the Cartesian plane in m_centerCoordinate 
        // Use mapPixelToCoords with m_cartesianPlane to map mouseClickPosition to the Cartesian plane and store it in m_centerCoordinate
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    // Assign m_vx and m_vy to random pixel velocities 
        // These will be the initial horizontal and vertical velocities of our particle
        // Somewhere between 100 and 500 worked for me, but you can experiment with this
        // If you want m_vx to be randomly positive or negative, use rand() % 2 and if it is not equal to zero, then multiply m_vx by -1
    m_vx = rand() % 2 ? rand() % 401 + 100 : -1 * (rand() % 401 + 100);
    m_vy = rand() % 401 + 100;

    // Assign m_color1 and m_color2 with Colors 
        // I made m_color1 whiteand m_color2 completely random, but you can experiment here

    // A random color from a selection for m_color1
    vector<Color> colors1{  Color::Red, Color::Yellow, Color::Magenta, };
    m_color1 = colors1[rand() % colors1.size()];

    // A random color from a selection for m_color2
    vector<Color> colors2{ Color::White, Color::Black, Color::Green, Color::Blue, Color::Cyan };
    m_color2 = colors2[rand() % colors2.size()];

    // Now we will generate numPoint vertices by sweeping a circular arc with randomized radii.

    // Initialize theta to an angle between [0: PI/ 2]
    float theta = (((float)rand() / (RAND_MAX)) * M_PI) / 2;
    // Initialize dTheta to 2 * PI / (numPoints - 1);
        // This is the amount we will rotate per vertex
        // We divide by numPoints - 1 because we want the last vertex to overlap with the first so we don't leave an open edge
    float dTheta = 2 * M_PI / (numPoints - 1);

    // Loop from j up to numPoints (I use i instead of j)
    for (int i = 0; i < numPoints; i++)
    {
        // Declare local variables r, dx, and dy
        int r, dx, dy;
        // Assign a random number between [20:80] to r (you can try a different range here)
        r = rand() % 61 + 20;
        // dx = r * cos(theta)
        dx = r * cos(theta);
        // dy = r * sin(theta)
        dy = r * sin(theta);

        // Assign the Cartesian coordinate of the newly generated vertex to m_A:
            // m_A(0, j) = m_centerCoordinate.x + dx;
            // m_A(1, j) = m_centerCoordinate.y + dy;
        m_A(0, i) = m_centerCoordinate.x + dx;
        m_A(1, i) = m_centerCoordinate.y + dy;

        // Increment theta by dTheta to move it to the next location for the next iteration of the loop
        theta += dTheta;
    }
}

// This function overrides the virtual function from sf::Drawable to allow our draw function to polymorph
// To draw, we will convert our Cartesian matrix coordinates from m_A to pixel coordinates in a VertexArray of primitive type TriangleFan 
void Particle::draw(RenderTarget & target, RenderStates states) const
{
    // Construct a VertexArray named lines 
        // Its arguments are TriangleFan, and the number of points numPoints + 1
        // The + 1 is to account for the center as shown above
    VertexArray lines(TriangleFan, m_numPoints + 1);

    // Declare a local Vector2f named center
        // This will be used to store the location on the monitor of the center of our particle
        // Assign it with the mapping of m_centerCoordinate from Cartesian to pixel / monitor coordinates using mapCoordsToPixel
    Vector2f center = static_cast<Vector2f>(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane)); // mapCoordsToPixel returns a Vector2i

    // Assign lines[0].position with center
    // Assign lines[0].color with m_color
        /* This will assign m_color to the center of our particle.If the outer colors are different,
           the engine will automatically create a cool looking smooth color gradient between the two colors */
    lines[0].position = center;
    lines[0].color = m_color1;

    // Loop i from 1 up to and including m_numPoints
        // Note that the index in lines is 1-off from the index in m_A because lines must contain the pixel coordinate for the center as its first element
    for (int i = 1; i <= m_numPoints; i++)
    {
        // Assign lines[i].position with the coordinate from column i - 1 in m_A, mapped from Cartesian to pixel coordinates using mapCoordsToPixel
        // Assign lines[i].color with m_Color2
        lines[i].position = static_cast<Vector2f>(target.mapCoordsToPixel(Vector2f(m_A(0, i - 1), m_A(1, i - 1)), m_cartesianPlane));
        lines[i].color = m_color2;
    }

    // When the loop is finished, draw the VertexArray :
        // target.draw(lines)
    target.draw(lines);
}

void Particle::update(float dt)
{
    // Subtract dt from m_ttl
    m_ttl -= dt;

    // Call rotate with an angle of dt * m_radiansPerSec
    rotate(dt * m_radiansPerSec);

    // Call scale using the global constant SCALE from Particle.h 
        // SCALE will effectively act as the percentage to scale per frame
        // 0.999 experimentally seemed to shrink the particle at a nice speed that wasn't too fast or too slow (you can change this)
    scale(SCALE);

    // Next we will calculate how far to shift / translate our particle, using distance (dx,dy) 
        
    // Declare local float variables dx and dy
    float dx, dy;
    // Assign m_vx * dt to dx
    dx = m_vx * dt;

    // The vertical velocity should change by some gravitational constant G, also experimentally determined and defined in Particle.h
        // This will allow the particle to travel up then fall down as if having an initial upward velocity and then getting pulled down by gravity

    // Subtract G * dt from m_vy
    m_vy -= G * dt;
    // Assign m_vy * dt to dy
    dy = m_vy * dt;

    // Call translate using dx,dy as arguments
    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift)
{
    // Construct a TranslationMatrix T with the specified shift values xShift and yShift
    TranslationMatrix T(xShift, yShift, m_numPoints);

    // Add it to m_A as m_A = T + m_A
    m_A = T + m_A;

    // Update the particle's center coordinate: 
        // m_centerCoordinate.x += xShift;
        // m_centerCoordinate.y += yShift;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

void Particle::rotate(double theta)
{
    /* Since the rotation matrix we will use is algebraically derived to rotate coordinates about the origin,
       we will temporarily shift our particle to the origin before rotating it */

    // Store the value of m_centerCoordinate in a Vector2f temp
    Vector2f temp = m_centerCoordinate;

    // Call translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
        // This will shift our particle's center, wherever it is, back to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    // Construct a RotationMatrix R with the specified angle of rotation theta
    RotationMatrix R(theta);

    // Multiply it by m_A as m_A = R * m_A
        /* Note: make sure to left-multiply r, as matrix multiplication is not commutative
           due to the fact that it multiplies the lvalue's rows into the rvalue's columns. */
    m_A = R * m_A;

    // Shift our particle back to its original center: 
        // translate(temp.x, temp.y);
    translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
    /* Scaling is also derived to scale coordinates relative to their distance from the origin.
       So we will also have to temporarily shift back to the origin here before scaling : */

    // Store the value of m_centerCoordinate in a Vector2f temp
    Vector2f temp = m_centerCoordinate;

    // Call translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
        // This will shift our particle's center, wherever it is, back to the origin
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    // Construct a ScalingMatrix S with the specified scaling multiplier c
    ScalingMatrix S(c);

    // Multiply it by m_A as m_A = S * m_A
    m_A = S * m_A;

    // Shift our particle back to its original center: 
        // translate(temp.x, temp.y);
    translate(temp.x, temp.y);
}

// ************************************************************
// ***********************barrior******************************
// ************************************************************

bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}
