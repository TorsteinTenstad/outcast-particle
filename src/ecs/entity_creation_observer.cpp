#include "ecs/entity_creation_observer.hpp"
#include <iostream>

EntityCreationObserver::EntityCreationObserver(ECSScene& subject, std::function<void(ECSScene&, int)> f) :
	subject_(subject),
	f_(f)
{
	subject_.AddEntityCreationObserver(this);
}
EntityCreationObserver::~EntityCreationObserver()
{
	subject_.RemoveEntityCreationObserver(this);
}
void EntityCreationObserver::Notify(int created_id)
{
	f_(subject_, created_id);
}