#include "shape.h"
#include <vector>
#include <algorithm>

Shape::Shape(const Vector &c, Texture* t, double ya, double pi, double ro)
    : center(c), texture(t), yaw(ya), pitch(pi), roll(ro) {}

void Shape::setAngles(double a, double b, double c) {
    yaw = a;
    pitch = b;
    roll = c;
    xcos = cos(yaw);
    xsin = sin(yaw);
    ycos = cos(pitch);
    ysin = sin(pitch);
    zcos = cos(roll);
    zsin = sin(roll);
}

void Shape::setYaw(double a) {
    yaw = a;
    xcos = cos(yaw);
    xsin = sin(yaw);
}

void Shape::setPitch(double b) {
    pitch = b;
    ycos = cos(pitch);
    ysin = sin(pitch);
}

void Shape::setRoll(double c) {
    roll = c;
    zcos = cos(roll);
    zsin = sin(roll);
}

struct TimeAndShape {
    double time;
    Shape* shape;
};

void calcColor(unsigned char* toFill, Autonoma* c, Ray ray, unsigned int depth) {
    std::vector<TimeAndShape> times;

    for (Shape* shape : c->shapes) {
        double time = shape->getIntersection(ray);
        times.push_back({ time, shape });
    }

    std::sort(times.begin(), times.end(), [](const TimeAndShape& a, const TimeAndShape& b) {
        return a.time < b.time;
    });

    if (times.empty() || times[0].time == inf) {
        double opacity, reflection, ambient;
        Vector temp = ray.vector.normalize();
        const double x = temp.x;
        const double z = temp.z;
        const double me = std::abs(temp.y);
        const double angle = atan2(z, x);
        c->skybox->getColor(toFill, &ambient, &opacity, &reflection, fix(angle / M_TWO_PI), fix(me));
        return;
    }

    double curTime = times[0].time;
    Shape* curShape = times[0].shape;
    Vector intersect = curTime * ray.vector + ray.point;
    double opacity, reflection, ambient;
    curShape->getColor(toFill, &ambient, &opacity, &reflection, c, Ray(intersect, ray.vector), depth);

    double lightData[3] = { 0, 0, 0 };
    getLight(lightData, c, intersect, curShape->getNormal(intersect), curShape->reversible());
    
    toFill[0] = static_cast<unsigned char>(toFill[0] * (ambient + lightData[0] * (1 - ambient)));
    toFill[1] = static_cast<unsigned char>(toFill[1] * (ambient + lightData[1] * (1 - ambient)));
    toFill[2] = static_cast<unsigned char>(toFill[2] * (ambient + lightData[2] * (1 - ambient)));
    
    if (depth < c->depth && (opacity < 1 - 1e-6 || reflection > 1e-6)) {
        unsigned char col[4];
        if (opacity < 1 - 1e-6) {
            Ray nextRay(intersect + ray.vector * 1E-4, ray.vector);
            calcColor(col, c, nextRay, depth + 1);
            toFill[0] = static_cast<unsigned char>(toFill[0] * opacity + col[0] * (1 - opacity));
            toFill[1] = static_cast<unsigned char>(toFill[1] * opacity + col[1] * (1 - opacity));
            toFill[2] = static_cast<unsigned char>(toFill[2] * opacity + col[2] * (1 - opacity));
        }
        if (reflection > 1e-6) {
            Vector norm = curShape->getNormal(intersect).normalize();
            Vector vec = ray.vector - 2 * norm * (norm.dot(ray.vector));
            Ray nextRay(intersect + vec * 1E-4, vec);
            calcColor(col, c, nextRay, depth + 1);
            toFill[0] = static_cast<unsigned char>(toFill[0] * (1 - reflection) + col[0] * reflection);
            toFill[1] = static_cast<unsigned char>(toFill[1] * (1 - reflection) + col[1] * reflection);
            toFill[2] = static_cast<unsigned char>(toFill[2] * (1 - reflection) + col[2] * reflection);
        }
    }
}
