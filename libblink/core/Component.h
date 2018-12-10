#pragma once

class Entity;

class Component
{
public:
	friend class Entity;

public:
	Entity* getEntity() const;

private:
	Entity* m_entity{};

};
