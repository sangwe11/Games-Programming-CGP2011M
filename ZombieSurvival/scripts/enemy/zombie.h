#ifndef _ZOMBIE_H
#define _ZOMBIE_H

#include <string>

#include <Scripting/Script.h>

namespace Engine
{
	class Zombie : public Script<Zombie>
	{
	public:
		Zombie(const std::string &name) : name(name) { }

		virtual void update()
		{
			std::cout << name << std::endl;
		}

	private:
		std::string name;
	};
}
#endif
