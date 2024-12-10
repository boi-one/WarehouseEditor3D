#include "Conveyor.h"

void ConveyorManager::SelectConveyor(glm::vec3& mousePosition)
{
    for (Conveyor& c : allConveyors)
    {

        Conveyor::ClosestPoint(c.path, mousePosition);
    }

}

Point* Conveyor::ClosestPoint(std::vector<Point> list, glm::vec3 origin, float range)
{
    Point* point = 0;

    for (Point& p : list)
    {
        float distance = glm::distance(p.position, origin);
        if (distance < range)
        {
            point = &p;
        }
    }
    return point;
}
