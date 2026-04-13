using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public interface INodeType
    {
        static abstract NodeTypes GetNodeType();
    }

    public enum NodeTypes
    {
        None = 0,
        Node,
        Scene,
        Camera,
        SpriteRenderer,
        Rigidbody,
        AudioStreamPlayer
    }
}
