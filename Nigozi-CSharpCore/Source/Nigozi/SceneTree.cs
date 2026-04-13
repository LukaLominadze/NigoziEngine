using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    internal static class SceneTree
    {
        public static T CreateNode<T>() where T : Node, INodeType
        {
            ulong id = 0;
            unsafe { id = InternalCalls.SceneTree_CreateNode((ushort)T.GetNodeType()); }
            unsafe { return (T)typeof(T).GetConstructor(new[] { typeof(ulong) })!.Invoke(new object[] { id }); }
        }

        public static SceneNode CreateNode()
        {
            throw new ArgumentException("Cannot load a scene from CreateNode function!");
        }

        public static void DestroyNode(Node node)
        {
            unsafe
            {
                InternalCalls.SceneTree_DestroyNode(node.ID);
            }
        }
    }
}
