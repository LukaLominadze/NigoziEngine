using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public class SpriteNode : Node
    {
        public IVector2 Slot
        {
            get
            {
                unsafe { return InternalCalls.SpriteRendererComponent_GetSlot(ID); }
            }
            set
            {
                unsafe { InternalCalls.SpriteRendererComponent_SetSlot(ID, value); }
            }
        }

        public IVector2 Seperator
        {
            get
            {
                unsafe { return InternalCalls.SpriteRendererComponent_GetSeperator(ID); }
            }
            set
            {
                unsafe { InternalCalls.SpriteRendererComponent_SetSeperator(ID, value); }
            }
        }

        public SpriteNode(ulong id) : base(id)
        {
        }

        public SpriteNode() { }

        public static new NodeTypes GetNodeType()
        {
            return NodeTypes.SpriteRenderer;
        }
    }
}
