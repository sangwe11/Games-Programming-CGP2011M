#ifndef _FILES_H
#define _FILES_H

#include <string>
#include <map>
#include <memory>
#include <assert.h>

#include "../EntitySystem/TypeId.h"
#include "../EntitySystem/System.h"

namespace Engine
{
	class BaseFile
	{
		friend class Files;

	public:
		// Virtual destructor
		virtual ~BaseFile() { }

		// All file types must implement a load function and return true if the file was loaded successfully
		virtual bool load(){ return false; }

		// Optional setup function
		virtual void setup() { }

		// Optional cleanup function
		virtual void cleanup() { }

	protected:
		std::string name;
	};

	template <typename T>
	class File : public BaseFile
	{
	public:
		// Virtual destructor
		virtual ~File() { }

		// Returns the TypeId for this file type
		static EntitySystem::TypeId getTypeId()
		{
			return EntitySystem::Type<BaseFile>::getTypeId<T>();
		}

	};

	class Files : public EntitySystem::System<Files>
	{
	public:
		virtual void uninitialise();

		template <typename T>
		T &loadFile(const std::string &name)
		{
			// Get file type id
			EntitySystem::TypeId typeId = T::getTypeId();

			// Check if file has already been loaded
			if (files[typeId][name] != nullptr)
			{
				return dynamic_cast<T &>(*files[typeId][name]);
			}
			else
			{
				// Create a new file of type
				files[typeId][name] = std::make_unique<T>();

				// Store file name
				files[typeId][name]->name = name;

				// Assert the file loaded
				assert(files[typeId][name]->load());

				// Setup
				files[typeId][name]->setup();

				// Return loaded file
				return dynamic_cast<T &>(*files[typeId][name]);
			}
		}

	private:
		std::map<EntitySystem::TypeId, std::map<std::string, std::unique_ptr<BaseFile>>> files;
	};
}

#endif