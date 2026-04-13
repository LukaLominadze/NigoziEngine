using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public class SceneNode : Node
    {
        public SceneNode(ulong id) : base(id) { }

        public static new NodeTypes GetNodeType()
        {
            return NodeTypes.Scene;
        }
    }
}
