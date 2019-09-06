using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TritonEngine
{
    /// <summary>
    /// Enum containing all the possible severities for a Log message
    /// </summary>
    public enum LogSeverity
    {
        Trace,
        Info,
        Warn,
        Error
    };

    /// <summary>
    /// Class providing inbuilt functionality for debugging
    /// </summary>
    public class Debug
    {
        /// <summary>
        /// Outputs a message to the Engine console
        /// </summary>
        /// <param name="message">The message to output</param>
        /// <param name="severity">The severity of the message</param>
        public static void Log(String message, LogSeverity severity = LogSeverity.Trace)
        {
            switch(severity)
            {
                case LogSeverity.Trace:
                    Interop.send_trace_Message_Internal(message);
                    return;

                case LogSeverity.Info:
                    Interop.send_info_Message_Internal(message);
                    return;

                case LogSeverity.Warn:
                    Interop.send_warn_Message_Internal(message);
                    return;

                case LogSeverity.Error:
                    Interop.send_error_Message_Internal(message);
                    return;
            }
        }
    }
}
