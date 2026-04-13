using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Coral.Managed.Interop;

namespace Nigozi
{
    public class Test
    {
        public Test()
        {
            Console.WriteLine("Test created!");
            Node node = SceneTree.CreateNode<Node>();
            SceneTree.DestroyNode(node);
        }

        public void LogTest()
        {
            Console.WriteLine("Hello");
        }

        public void LogFromCpp()
        {
            Log.Info("Info from c#");
            Log.Warn("Warn from c#");
            Log.Error("Error from c#");
            Log.Critical("Critical from c#");
        }

        public static void StaticLogFromCpp(NativeString nativeMsg)
        {
            Log.Info($"Static Info from c# {nativeMsg}");
            Log.Warn($"Static Warn from c# {nativeMsg}");
            Log.Error($"Static Error from c# {nativeMsg}");
            Log.Critical($"Static Critical from c# {nativeMsg}");
        }
    }
}
