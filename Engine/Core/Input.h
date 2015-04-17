#ifndef _INPUT_H
#define _INPUT_H

#include <SDL/SDL.h>
#include <string>
#include <map>

#include "../EntitySystem/System.h"

namespace Engine
{
	class BaseDevice
	{
	public:
		typedef std::string Button;
		typedef std::string Axis;

		// Virtual destructor
		virtual ~BaseDevice(){ }

		// Virtual update function - input polling should be done here
		virtual void update() { }

		// Virtual event handle function - SDL2 input events should be handled here
		virtual void handle(const SDL_Event &e) { }

		// Get device information
		const uint32_t &getId();
		const std::string &getName();

		// Get button state
		const bool getButton(const Button &button);
		const bool getButtonDown(const Button &button);
		const bool getButtonUp(const Button &button);

		// Get axis state
		const float getAxis(const Axis &axis);

	protected:
		uint32_t id;
		std::string name;

		// Button states
		std::map<Button, bool> buttons;
		std::map<Button, bool> buttonsOld;

		// Axis states
		std::map<Axis, float> axes;
	};

	template <typename T>
	class Device : public BaseDevice
	{
	public:
		static EntitySystem::TypeId getTypeId()
		{
			return EntitySystem::Type<BaseDevice>::getTypeId<T>();
		}
	};

	class Input : public EntitySystem::System<Input>
	{
	public:
		// Overload virtual functions
		virtual void initialise();
		virtual void uninitialise();
		virtual void update(EntitySystem::EntityManager &entities);

		// Check if quit event has been triggered
		const bool checkQuit() { return quit; }

		// Count devices of type T
		template <typename T>
		uint32_t count()
		{
			// Return the device list count for type T
			return devices[T::getTypeId()].size();
		}

		// Check if device of type T exists
		template <typename T>
		const bool exists(uint32_t device)
		{
			// Get device type
			EntitySystem::TypeId typeId = T::getTypeId();

			// Check if the device exists
			return devices[typeId].find(device) != devices[typeId].end();
		}

		// Get device of type T
		template <typename T>
		T &get(uint32_t device)
		{
			// Assert device exists
			assert(exists<T>(device));

			// Find device and return ref
			return dynamic_cast<T &>(*devices[T::getTypeId()][device]);
		}

	private:
		std::map<EntitySystem::TypeId, std::map<uint32_t, std::unique_ptr<BaseDevice>>> devices;
		bool quit = false;
	};
}

#endif