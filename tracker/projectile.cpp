#include <cmath>
#include "projectile.h"

Projectile::Projectile(const string& name) :
	TwoWayMultiSprite(name),
	startingPos(getPosition()),
	distance(0),
	maxDistance(Gamedata::getInstance().getXmlInt(name+"/maxDistance")),
	tooFar(false)
	{ }

Projectile::Projectile(const Projectile& s) :
	TwoWayMultiSprite(s),
	startingPos(s.startingPos),
	distance(s.distance),
	maxDistance(s.maxDistance),
	tooFar(s.tooFar)
	{ }

