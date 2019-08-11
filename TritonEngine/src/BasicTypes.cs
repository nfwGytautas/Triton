using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace TritonEngine.Types
{
    /// <summary>
    /// Vector of 3 Floats
    /// </summary>
    public class Vector3
    {
        /// <summary>
        /// Initializes an empty Vector3
        /// </summary>
        public Vector3() { }

        /// <summary>
        /// {C++}
        /// Initializes Vetor3 from native handle
        /// </summary>
        /// <param name="_nativeHandle">The pointer to the Vector3 in Triton</param>
        public Vector3(IntPtr _nativeHandle)
        {
            _handle = _nativeHandle;
        }

        /// <summary>
        /// Initializes an Vector3 with specified parameters
        /// </summary>
        /// <param name="x">The X component of the Vector</param>
        /// <param name="y">The Y component of the Vector</param>
        /// <param name="z">The Z component of the Vector</param>
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// The X component of the Vector class
        /// </summary>
        public float X { get { return GetComponent(VecComponent.X); } set { SetComponent(VecComponent.X, value); } }

        /// <summary>
        /// The Y component of the Vector class
        /// </summary>
        public float Y { get { return GetComponent(VecComponent.Y); } set { SetComponent(VecComponent.Y, value); } }

        /// <summary>
        /// The Z component of the Vector class
        /// </summary>
        public float Z { get { return GetComponent(VecComponent.Z); } set { SetComponent(VecComponent.Z, value); } }

        /// <summary>
        /// Enum specifying all the available components for the vector 
        /// </summary>
        private enum VecComponent
        {
            X = 0,
            Y = 1,
            Z = 2
        }

        private float _x;
        private float _y;
        private float _z;

        /// <summary>
        /// Native handle
        /// </summary>
        private IntPtr _handle;

        /// <summary>
        /// Get component value of specified type
        /// </summary>
        /// <param name="component">The component to get</param>
        /// <returns>Value of component</returns>
        private float GetComponent(VecComponent component)
        {
            // Vector was created by the user
            if(_handle == IntPtr.Zero)
            {
                switch (component)
                {
                    case VecComponent.X:
                        return _x;
                    case VecComponent.Y:
                        return _y;
                    case VecComponent.Z:
                        return _z;
                }

                return 0.0f;
            }
            else
            {
                return get_vector3_component(_handle, component);
            }
        }

        /// <summary>
        /// Set component value of specified type
        /// </summary>
        /// <param name="component">The component to set</param>
        /// <param name="value">The new value of the component</param>
        private void SetComponent(VecComponent component, float value)
        {
            // Vector was created by the user
            if (_handle == IntPtr.Zero)
            {
                switch (component)
                {
                    case VecComponent.X:
                        _x = value;
                        return;
                    case VecComponent.Y:
                        _y = value;
                        return;
                    case VecComponent.Z:
                        _z = value;
                        return;
                }
            }
            else
            {
                set_vector3_component(_handle, component, value);
            }
        }

        /// <summary>
        /// Returns a float component of _component type from _handle object
        /// </summary>
        /// <param name="_handle">Handle to object</param>
        /// <param name="_component">Object component</param>
        /// <returns>The value of component</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static float get_vector3_component(IntPtr _handle, object _component);

        /// <summary>
        /// Returns a float component of _component type from _handle object
        /// </summary>
        /// <param name="_handle">Handle to object</param>
        /// <param name="_component">Object component</param>
        /// <param name="_value">Component value</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void set_vector3_component(IntPtr _handle, object _component, float _value);
    }
}
