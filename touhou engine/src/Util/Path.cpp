#include "Path.h"


void Path::setPathSpeed(int speedSeconds)
{
	pathSpeed = speedSeconds;
}


int Path::getPathSpeed()
{
	return pathSpeed;
}

// goal: make each point equal distance from eachother
bool Path::smoothPath()
{
    double ratio, x, y, segmentLength,
           totalLength = 0, segmentDistance, 
           distanceSinceLastPoint = 0,
           a, b;
    std::vector<sf::Vector2f> newPath;

    if (size() <= 2 || pathSpeed < 0)
        return true;
    
    for (int i = 0; i < size() - 1; i++) 
        totalLength += sqrt(pow(at(i + 1).x - at(i).x, 2) +
            pow(at(i + 1).y - at(i).y, 2));

    segmentLength = totalLength/ size()-1;

    newPath.push_back(at(0));
    for (int i = 0; i < size() - 1; i++) 
    {
        segmentDistance = sqrt(pow(at(i + 1).x - at(i).x, 2) + pow(at(i + 1).y - at(i).y, 2));
        while (distanceSinceLastPoint + segmentDistance >= segmentLength) {
            if (newPath.size() > size() * 2)
                return false;
            ratio = (segmentLength - distanceSinceLastPoint) / segmentDistance;
            a = at(i + 1).x - at(i).x;
            b = at(i + 1).y - at(i).y;

            x = at(i).x + ratio * a;
            y = at(i).y + ratio * b;

            newPath.push_back(sf::Vector2f(x, y));

            segmentDistance = sqrt(pow(at(i + 1).x - x, 2) +
                pow(at(i + 1).y - y, 2));
            distanceSinceLastPoint = 0;
        }
        distanceSinceLastPoint += segmentDistance;
    }
    if (newPath[newPath.size() - 1] != at(size() - 1))
        newPath.push_back(at(size() - 1));

    clear();
    insert(begin(), newPath.begin(), newPath.end());
    return true;
}