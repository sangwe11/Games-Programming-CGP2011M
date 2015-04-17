#ifndef _INSTANCE_H
#define _INSTANCE_H

namespace Engine
{
	class Instance
	{
	public:
		Instance();
		~Instance();

	private:
		void setup();
		void cleanup();
	};
}

#endif