using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using TritonEngine.Types;

namespace TritonEngine.ECS
{
    /// <summary>
    /// GameObject component that specifies the objects transformation in the application
    /// </summary>
    public class Transform
    {
        private Vector3 _position;
        /// <summary>
        /// A 3-Component vector that specifies the objects position
        /// </summary>
        public Vector3 Position
        {
            get => _position;
            set
            {
                _position.X = value.X;
                _position.Y = value.Y;
                _position.Z = value.Z;
            }
        }

        private Vector3 _rotation;
        /// <summary>
        /// A 3-Component vector that specifies the objects rotation
        /// </summary>
        public Vector3 Rotation
        {
            get => _rotation;
            set
            {
                _rotation.X = value.X;
                _rotation.Y = value.Y;
                _rotation.Z = value.Z;
            }
        }

        private Vector3 _scale;
        /// <summary>
        /// A 3-Component vector that specifies the objects scale
        /// </summary>
        public Vector3 Scale
        {
            get => _scale;
            set
            {
                _scale.X = value.X;
                _scale.Y = value.Y;
                _scale.Z = value.Z;
            }
        }

        /// <summary>
        /// {C++}
        /// Handle to the underlying object
        /// </summary>
        private IntPtr _handle;

        /// <summary>
        /// {C++}
        /// Creates the transform object from GameObject handle
        /// </summary>
        /// <param name="handle"></param>
        public Transform(IntPtr handle)
        {
            _handle = get_transform_handle(handle);
            _position = new Vector3(get_position_handle(_handle));
            _rotation = new Vector3(get_rotation_handle(_handle));
            _scale = new Vector3(get_scale_handle(_handle));
        }

        /// <summary>
        /// Returns the handle to the transform component from game handle
        /// </summary>
        /// <param name="_handle">Handle to game object</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static IntPtr get_transform_handle(IntPtr _handle);

        /// <summary>
        /// Returns the handle to the position of the transform
        /// </summary>
        /// <param name="_handle">Handle to game object</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static IntPtr get_position_handle(IntPtr _handle);

        /// <summary>
        /// Returns the handle to the rotation of the transform
        /// </summary>
        /// <param name="_handle">Handle to game object</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static IntPtr get_rotation_handle(IntPtr _handle);

        /// <summary>
        /// Returns the handle to the scale of the transform
        /// </summary>
        /// <param name="_handle">Handle to game object</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static IntPtr get_scale_handle(IntPtr _handle);
    }
}