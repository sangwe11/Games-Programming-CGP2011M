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
		virtual bool load(){ std::cout << "load() had not been overridden or overloaded." << std::endl; return false; }

		// Optional setup function
		virtual void setup() { }

		// Optional cleanup function
		virtual void cleanup() { }

	protected:
		std::string name;
		Files *files;
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

		template <typename T, typename... Args>
		T &loadFile(const std::string &name, Args &&... args)
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

				T &file = dynamic_cast<T &>(*files[typeId][name]);

				// Store file name
				file.name = name;
				file.files = this;

				// Assert the file loaded
				assert(file.load(std::forward<Args>(args)...));

				// Print debug message
				std::cout << "Loaded file: " << name << std::endl;

				// Setup
				file.setup();

				// Return loaded file
				return file;
			}
		}

	private:
		std::map<EntitySystem::TypeId, std::map<std::string, std::unique_ptr<BaseFile>>> files;
	};
}

#endif