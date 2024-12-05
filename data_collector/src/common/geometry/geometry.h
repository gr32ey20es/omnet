#include <omnetpp.h>
using namespace omnetpp;

typedef cFigure::Point Point;
typedef cFigure::Transform Transform;
typedef cFigure::Rectangle Rectangle;

bool isGreaterOrEqualPoint(const Point&, const Point&);
void applyToTransform(Point&, const Transform&);
bool isInsideRectangle(const Point&, const Rectangle&);
