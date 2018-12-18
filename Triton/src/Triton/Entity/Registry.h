#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"
namespace Triton
{
	namespace ECS
	{
		class Component;
		class Event;
		class System;
		class Registry;

		namespace Internal
		{
			using ComponentID = size_t;
			ComponentID GetUniqueComponentID() noexcept;
			template<typename T>
			ComponentID GetComponentTypeID() noexcept
			{
				TR_STATIC_ASSERT(std::is_base_of<Component, T>::value, "T must inherit from Component");
				static ComponentID typeID{ GetUniqueComponentID() };
				return typeID;
			}

			using SystemID = size_t;
			SystemID GetUniqueSystemID() noexcept;
			template<typename T>
			SystemID GetSystemTypeID() noexcept
			{
				TR_STATIC_ASSERT(std::is_base_of<System, T>::value, "T must inherit from System");
				static SystemID typeID{ GetUniqueSystemID() };
				return typeID;
			}
		}

		struct TRITON_API Component
		{
			virtual ~Component() {}
		};

		class TRITON_API Entity
		{
		public:
			template<typename T, typename... TArgs>
			T& AddComponent(TArgs&&... mArgs)
			{
				if (HasComponent<T>())
				{
					return GetComponent<T>();
				}

				T* c(new T(std::forward<TArgs>(mArgs)...));
				CheckCapacity(Internal::GetComponentTypeID<T>());
				m_Components[Internal::GetComponentTypeID<T>()] = c;
				m_componentBitset[Internal::GetComponentTypeID<T>()] = true;

				return *c;
			}
			template<typename T>
			bool HasComponent() const
			{
				if(m_componentBitset.size() > Internal::GetComponentTypeID<T>())
				{
					return m_componentBitset[Internal::GetComponentTypeID<T>()];
				}
				else
				{
					return false;
				}
			}
			template<typename T>
			T& GetComponent() const
			{
				TR_CORE_ASSERT(HasComponent<T>());
				auto ptr(m_Components[Internal::GetComponentTypeID<T>()]);
				return *static_cast<T*>(ptr);
			}
			template<typename T>
			void RemoveComponent()
			{
				TR_CORE_ASSERT(HasComponent<T>());
				m_componentBitset[Internal::GetComponentTypeID<T>()] = false;
				delete m_Components[Internal::GetComponentTypeID<T>()];
			}

			virtual ~Entity()
			{
				for (unsigned int i = 0; i < m_Components.size(); i++)
				{
					delete m_Components[i];
				}
			}

			inline bool operator==(const Entity& rhs) const
			{
				return this->m_RegistryID == rhs.m_RegistryID;
			}
			inline bool operator!=(const Entity& rhs) const
			{
				return !(*this == rhs);
			}

			void Invalidate()
			{
				m_Valid = false;
			}
			bool IsValid() const
			{
				return m_Valid;
			}
		private:
			Entity(Registry* aRegistry, size_t aID)
			{
				m_Registry = aRegistry;
				m_RegistryID = aID;
			}

			void CheckCapacity(Internal::ComponentID aID)
			{
				if (m_Components.size() < aID - 1)
				{
					m_Components.resize(aID + 1);
					m_componentBitset.resize(aID + 1);
				}
			}
		private:
			size_t m_RegistryID;
			bool m_Valid = true;

			Registry* m_Registry;
			std::vector<Component*> m_Components;
			std::vector<bool> m_componentBitset;

			friend class Registry;
		};

		class TRITON_API System
		{
		public:
			virtual void Update(Registry* aRegistry) = 0;
			virtual void Configure(Registry* aRegistry) = 0;
			virtual void UnConfigure(Registry* aRegistry) = 0;
		};

		using EntityFilter = std::function<bool(const Entity* aEntity)>;

		class TRITON_API Registry
		{
		public:
			Entity* CreateEntity()
			{				
				Entity* entity = new Entity(this, ++m_LastEntityId);
				if (m_FreeSlots.size() != 0)
				{
					m_Entities.push_back(entity);
				}
				else
				{
					m_Entities[m_FreeSlots.front()] = entity;
					m_FreeSlots.pop();
				}
				return entity;
			}
			
			template<typename T, typename... TArgs>
			T& RegisterSystem(TArgs&&... mArgs)
			{
				T* system(new T(std::forward<TArgs>(mArgs)...));
				CheckCapacity(Internal::GetSystemTypeID<T>());
				m_Systems[Internal::GetSystemTypeID<T>()] = system;
				m_SystemBitSet[Internal::GetSystemTypeID<T>()] = true;
				system->Configure(this);
				return *system;
			}

			template<typename T>
			void UnregisterSystem()
			{
				m_Systems[Internal::GetSystemTypeID<T>()]->UnConfigure(this);
				m_SystemBitSet[Internal::GetSystemTypeID<T>()] = false;
				delete m_Systems[Internal::GetComponentTypeID<T>()];	
			}

			template<typename T>
			void EnableSystem()
			{
				TR_CORE_ASSERT(m_Systems[Internal::GetSystemTypeID<T>()] != nullptr, "Can't enable a non registered system");
				m_SystemBitSet[Internal::GetSystemTypeID<T>()] = true;
			}

			template<typename T>
			void DisableSystem()
			{
				TR_CORE_ASSERT(m_Systems[Internal::GetSystemTypeID<T>()] != nullptr, "Can't disable a non registered system");
				m_SystemBitSet[Internal::GetSystemTypeID<T>()] = false;
			}

			void UpdateSystems()
			{
				for (int i = 0; i < m_Entities.size(); i++)
				{
					if (m_Entities[i] != nullptr && !(m_Entities[i]->IsValid()))
					{
						m_FreeSlots.push(i);
						delete m_Entities[i];
					}
				}

				for (int i = 0; i < m_Systems.size(); i++)
				{
					if (m_SystemBitSet[i] = true)
					{
						m_Systems[i]->Update(this);
					}
				}
			}

			std::vector<Entity*> View(EntityFilter& aFilter)
			{
				std::vector<Entity*> result(m_Entities.size() / 4);
				for (Entity* entity : m_Entities)
				{
					if (aFilter(entity))
					{
						result.push_back(entity);
					}
				}
				return result;
			}

			virtual ~Registry()
			{
				for (unsigned int i = 0; i < m_Systems.size(); i++)
				{
					m_Systems[i]->UnConfigure(this);
					delete m_Systems[i];
				}
				for (unsigned int i = 0; i < m_Entities.size(); i++)
				{					
					delete m_Entities[i];
				}
			}
		private:
			void CheckCapacity(Internal::SystemID aID)
			{
				if (m_Systems.size() < aID - 1)
				{
					m_Systems.resize(aID + 1);
				}
				if (m_SystemBitSet.size() < aID - 1)
				{
					m_SystemBitSet.resize(aID + 1);
				}
			}
		private:
			size_t m_LastEntityId = 0;		
			std::queue<unsigned int> m_FreeSlots;
			std::vector<Entity*> m_Entities;
			std::vector<System*> m_Systems;
			std::vector<bool> m_SystemBitSet;
		};
	}
}