#ifndef _FILES_H
#define _FILES_H

#include <string>

#include "../EntitySystem/TypeId.h"

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
}

#endif