#pragma once

namespace Triton
{
	namespace Manager
	{
		// Base class for all manager where T is the type that 
		// is managed
		template <typename T>
		class Manager
		{
		public:
			Manager() {}
			virtual ~Manager() 
			{ 
				cleanUp(); 
			}

			// Add object
			virtual size_t addObject(reference<T> object)
			{
				m_Objects[m_nextID] = object;
				return m_nextID++;
			}

			// Add object at id, should be used together with getNextID()
			virtual void addObject(size_t at, reference<T> object)
			{
				m_Objects[at] = object;
			}

			// Get the next possible id, used when the object that is being created uses the id 
			// of the manager itself
			virtual size_t getNextID()
			{
				return m_nextID++;
			}

			// Remove object however if the object is used elsewhere the object will still be valid
			virtual void removeObject(size_t id)
			{
				m_Objects.erase(id);
			}

			// Get object of specified id
			virtual reference<T> getObject(size_t id)
			{
				return m_Objects[id];
			}

			// Clean the memory however if the objects are used elsewhere they will still be valid
			virtual void cleanUp()
			{
				m_nextID = 0;
				m_Objects.clear();
			}
		protected:
			size_t m_nextID = 0;
			std::unordered_map<size_t, reference<T>> m_Objects;

		};
	}
}