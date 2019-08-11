using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace TritonEngine
{
    class Interop
    {
        /// <summary>
        /// Send a trace(white) message to the Triton engine 
        /// </summary>
        /// <param name="payload">The message</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void send_trace_Message_Internal(String payload);

        /// <summary>
        /// Send a info(green) message to the Triton engine 
        /// </summary>
        /// <param name="payload">The message</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void send_info_Message_Internal(String payload);

        /// <summary>
        /// Send a warning(yellow) message to the Triton engine 
        /// </summary>
        /// <param name="payload">The message</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void send_warn_Message_Internal(String payload);

        /// <summary>
        /// Send a error(red) message to the Triton engine 
        /// </summary>
        /// <param name="payload">The message</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void send_error_Message_Internal(String payload);
    }
}