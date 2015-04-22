#ifndef _TYPEID_H
#define _TYPEID_H

#include <cstddef>

namespace EntitySystem
{

	typedef std::size_t TypeId;

	template <typename TBase>
	class Type
	{
	public:
		template <typename T>
		static TypeId getTypeId()
		{
			static const TypeId id = nextId++;
			return id;
		}

	private:
		static TypeId nextId;
	};

	template <typename TBase>
	TypeId Type<TBase>::nextId = 0;

}

#endif