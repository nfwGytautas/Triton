using System;
using System.Collections.Generic;
using System.Linq;
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
    /// The main class of the TritonEngine
    /// </summary>
    public class TritonEngine
    {
        /// <summary>
        /// {C++}
        /// Initializes the engine
        /// </summary>
        public void Setup()
        {
        }
    }
}