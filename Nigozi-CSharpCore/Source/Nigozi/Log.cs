using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Coral.Managed.Interop;

namespace Nigozi
{
    public static class Log
    {
        public static void Info(string message)
        {
            NativeString nativeMessage = message;
            unsafe { InternalCalls.Log_Info(nativeMessage); }
        }
        public static void Warn(string message)
        {
            NativeString nativeMessage = message;
            unsafe { InternalCalls.Log_Warn(nativeMessage); }
        }
        public static void Error(string message)
        {
            NativeString nativeMessage = message;
            unsafe { InternalCalls.Log_Error(nativeMessage); }
        }
        public static void Critical(string message)
        {
            NativeString nativeMessage = message;
            unsafe { InternalCalls.Log_Critical(nativeMessage); }
        }
    }
}
