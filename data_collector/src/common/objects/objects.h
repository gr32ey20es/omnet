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
        virtual void displayString() const;

    public:
        virtual const Point& getLocation() const;
        virtual const Rectangle& getBoundRect() const;

    protected:
        virtual double distanceTo(const Point&) const;
        virtual double wrapIfOutside(const Point& vector);
};
