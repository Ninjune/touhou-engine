#include "PatternChanger.h"


PatternChanger::PatternChanger()
{

}


void PatternChanger::update(sf::RenderWindow& window,
	int& frame,
	BulletPatternMenu& menu,
	std::map<std::string, BulletPattern>& patterns
)
{
	for (std::pair<std::string, BulletPattern> const& it : patterns)
	{
		//textStr += it.first + ": " + std::to_string(it.second.);
	}
}


void PatternChanger::poll(sf::Event)
{

}