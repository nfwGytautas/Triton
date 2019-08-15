using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace TritonEngine
{
    /// <summary>
    /// Class inherited by all scripts
    /// </summary>
    public class ObjectScript : GameObject
    {
        /// <summary>
        /// Update is called every frame, with the delta between this and previous frame
        /// </summary>
        /// <param name="Delta">Time elapsed between the previous frame and this frame</param>
        public virtual void OnUpdate(float Delta) { }
    }

    /// <summary>
    /// The game object class allows provides functionality for manipulating
    /// an entity from scripts
    /// </summary>
    public class GameObject
    {
        /// <summary>
        /// Returns a component of type T
        /// </summary>
        /// <typeparam name="T">The component you want to get</typeparam>
        /// <returns>Component</returns>
        public T GetComponent<T>() where T : class
        {
            Type serviceInterface = typeof(T);
            if (serviceInterface.Equals(typeof(ECS.Transform)))
            {
                return (T)(object)new ECS.Transform(_object_handle);
            }

            return null;
        }

        /// <summary>
        /// Checks if the GameObject has the specified component
        /// </summary>
        /// <typeparam name="T">The component you want to check for</typeparam>
        /// <returns>True if the object has the specified component</returns>
        public bool HasComponent<T>()
        {
            Type serviceInterface = typeof(T);
            if (serviceInterface.Equals(typeof(ECS.Transform)))
            {
                return has_transform(_object_handle);
            }

            return false;
        }

        /// <summary>
        /// Native handle to the game object
        /// </summary>
        private IntPtr _object_handle;

        /// <summary>
        /// Returns a boolean that shows if the object has the transform component
        /// </summary>
        /// <param name="_handle">Handle to game object</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static bool has_transform(IntPtr _handle);
    }
}
