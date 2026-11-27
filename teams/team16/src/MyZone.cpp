
#include <citylab/CityLab.h>
#include <cmath>

namespace team16 {
using namespace citylab;

class MyZone : public Zone {
public:
    void init(Context& ctx) override {
        scenery_ = glGenLists(1);

        glNewList(scenery_, GL_COMPILE);
        drawScenery(ctx.plot());
        glEndList();

        ctx.log("Cable car station layout initialized on plot " +
                ctx.plot().id);
    }

    void update(const Frame&) override {
        // Static layout for now.
    }

    void draw(const Context&) const override {
        if (scenery_ != 0) {
            glCallList(scenery_);
        }
    }

    void reset() override {}

    void onKey(int) override {}

private:

    // Draw a horizontal rectangular surface.
    void drawRect(float x, float z,
                  float width, float depth,
                  float y = 0.02f) const {
        glPushMatrix();
        glTranslatef(x, y, z);
        shapes::plane(width, depth);
        glPopMatrix();
    }

    // Draw a curved pedestrian path (circular arc).
void drawCurvedPath(float cx, float cz,
                    float radius, float width,
                    float startAngle, float endAngle) const {

    const int segments = 24;

    glBegin(GL_QUAD_STRIP);

    for (int i = 0; i <= segments; ++i) {
        float t = static_cast<float>(i) / segments;
        float angle = (startAngle +
                       t * (endAngle - startAngle))
                      * 3.14159265f / 180.0f;

        float cosA = cosf(angle);
        float sinA = sinf(angle);

        float outerRadius = radius + width / 2.0f;
        float innerRadius = radius - width / 2.0f;

        glVertex3f(
            cx + outerRadius * cosA,
            0.045f,
            cz + outerRadius * sinA
        );

        glVertex3f(
            cx + innerRadius * cosA,
            0.045f,
            cz + innerRadius * sinA
        );
    }

    glEnd();
}

    // Draw all building footprints.
    void drawStructures() const {

        // ---------------------------------
        // LOWER STATION
        // Center: (0, 23)
        // Size: 20 x 13
        // ---------------------------------
        glColor3f(0.48f, 0.51f, 0.56f);
        drawRect(0.0f, 23.0f, 20.0f, 13.0f, 0.05f);

        // Lower station - right: Control and maintenance
        // Center: (18, 23)
        // Size: 12 x 10
        glColor3f(0.25f, 0.43f, 0.62f);
        drawRect(18.0f, 23.0f, 12.0f, 10.0f, 0.06f);

        // Lower station - left: Restrooms
        // Center: (-17, 23)
        // Size: 8 x 8
        glColor3f(0.35f, 0.59f, 0.68f);
        drawRect(-17.0f, 23.0f, 8.0f, 8.0f, 0.06f);


        // ---------------------------------
        // UPPER STATION
        // Center: (0, -30)
        // Size: 20 x 13
        // ---------------------------------
        glColor3f(0.48f, 0.51f, 0.56f);
        drawRect(0.0f, -30.0f, 20.0f, 13.0f, 0.05f);

        // Upper station - left: Control and maintenance
        // Center: (-18, -30)
        // Size: 12 x 10
        glColor3f(0.25f, 0.43f, 0.62f);
        drawRect(-18.0f, -30.0f, 12.0f, 10.0f, 0.06f);

        // Upper station - right: Restaurant
        // Center: (18, -30)
        // Size: 12 x 10
        glColor3f(0.72f, 0.48f, 0.25f);
        drawRect(18.0f, -30.0f, 12.0f, 10.0f, 0.06f);
    }

    // Draw tower base markers.
    void drawTowerBases() const {
        glColor3f(0.78f, 0.22f, 0.18f);

        // Tower 1
        drawRect(0.0f, 7.0f, 3.0f, 3.0f, 0.07f);

        // Tower 2
        drawRect(0.0f, -12.0f, 3.0f, 3.0f, 0.07f);
    }

    // Draw waiting area and entrance marker.
    void drawPublicAreas() const {

        // Waiting area
        glColor3f(0.82f, 0.76f, 0.62f);
        drawRect(0.0f, 37.0f, 24.0f, 10.0f, 0.03f);

        // Entrance landmark marker
        glColor3f(0.95f, 0.75f, 0.15f);
        drawRect(12.0f, 42.0f, 3.0f, 3.0f, 0.07f);
    }

    // Draw pedestrian paths.
    void drawPaths() const {

    glColor3f(0.68f, 0.68f, 0.65f);

    // =====================================
    // 1. MAIN ENTRANCE PATH
    // =====================================

    // Entrance to waiting area
    drawRect(0.0f, 43.0f, 4.0f, 14.0f, 0.04f);

    // Waiting area to lower station
    drawRect(0.0f, 30.75f, 3.0f, 3.5f, 0.04f);


    // =====================================
    // 2. LOWER STATION CONNECTIONS
    // =====================================

    // Lower station to right control building
    drawRect(11.0f, 23.0f, 2.0f, 3.0f, 0.04f);

    // Lower station to left restroom
    drawRect(-11.5f, 23.0f, 3.0f, 2.0f, 0.04f);


    // =====================================
    // 3. UPPER STATION CONNECTIONS
    // =====================================

    // Upper station to left control building
    drawRect(-11.0f, -30.0f, 2.0f, 3.0f, 0.04f);

    // Upper station to right restaurant
    drawRect(11.0f, -30.0f, 2.0f, 3.0f, 0.04f);


    // =====================================
    // 4. SIDE WALKWAYS
    // =====================================

    // Left perimeter walkway
    drawRect(-30.0f, 2.5f, 2.0f, 65.0f, 0.04f);

    // Right perimeter walkway
    drawRect(30.0f, 2.5f, 2.0f, 65.0f, 0.04f);


    // =====================================
    // 5. WAITING AREA SIDE CONNECTIONS
    // =====================================

    // Horizontal connections
    drawRect(-20.0f, 37.0f, 16.0f, 2.0f, 0.04f);
    drawRect(20.0f, 37.0f, 16.0f, 2.0f, 0.04f);

    // Curved corners
    drawCurvedPath(-28.0f, 35.0f, 2.0f, 2.0f,
                   90.0f, 180.0f);

    drawCurvedPath(28.0f, 35.0f, 2.0f, 2.0f,
                   90.0f, 0.0f);


// =====================================
// 6. UPPER STATION SIDE CONNECTIONS
// =====================================

// Left connection to upper control building
drawRect(-26.5f, -30.0f, 5.0f, 2.0f, 0.04f);

// Right connection to upper restaurant
drawRect(26.5f, -30.0f, 5.0f, 2.0f, 0.04f);


// =====================================
// 7. CURVED UPPER CORNERS
// =====================================

// Upper-left curved corner
drawCurvedPath(-28.0f, -28.0f, 2.0f, 2.0f,
               180.0f, 270.0f);

// Upper-right curved corner
drawCurvedPath(28.0f, -28.0f, 2.0f, 2.0f,
               270.0f, 360.0f);
}

    void drawScenery(const PlotInfo& plot) const {

        // Main terrain
        glColor3f(0.58f, 0.66f, 0.48f);
        drawRect(0.0f, 0.0f,
                 plot.width, plot.depth, 0.01f);

        // Public areas
        drawPublicAreas();

        // Buildings
        drawStructures();

        // Tower bases
        drawTowerBases();

        // Pedestrian paths
        drawPaths();
    }

    GLuint scenery_ = 0;
};

} // namespace team16

CITYLAB_REGISTER_ZONE(team16::MyZone);
