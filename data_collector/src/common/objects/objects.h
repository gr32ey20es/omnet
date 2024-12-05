#include <omnetpp.h>
#include "../geometry/geometry.h"

using namespace omnetpp;

class Object : public cSimpleModule
{
    private:
        Point location;
        Rectangle boundRect;

    protected:
        virtual void initialize() override;

    public:
        const Point& getLocation() const;
        const Rectangle& getBoundRect() const;

    protected:
        double distanceTo(const Point&) const;
        double wrapIfOutside(const Point& vector);
};
