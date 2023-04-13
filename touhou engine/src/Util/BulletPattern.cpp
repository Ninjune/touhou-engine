#include <fstream>
#include "BulletPattern.h"


BulletPattern::BulletPattern(std::string patternFolder, std::string fileName, sf::Texture& bulletTexture)
{
	std::fstream patternFile;
	patternFile.open(patternFolder + "/" + fileName + ".txt");

	if (patternFile.is_open())
	{
		patternFile >> origin.x >> origin.y >> frequency >> burstCount >>
			burstSize >> burstSizeChange >> direction >> directionChange >>
			spawnDirection >> spawnDirectionChange >> velocity >> velocityChange;
		name = fileName;
	}
	else
		throw "file not found?";
}


std::string BulletPattern::getName()
{
	return name;
}
/*
FILE FORMAT:
originX originY frequency burstCount burstSize burstSizeChange direction (just continued no \n)
directionChange spawnDirection spawnDirectionChange velocity velocityChange
bulletType

The file name will be the name. Exclude .txt

For origin: if either is a negative number then require Enemy to give it's origin as it will be relative to enemy.
*/