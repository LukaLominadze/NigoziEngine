using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public class CameraNode : Node
    {
        public float Zoom
        {
            get
            {
                unsafe { return InternalCalls.CameraComponent_GetZoom(ID); }
            }
            set
            {
                unsafe { InternalCalls.CameraComponent_SetZoom(ID, value); }
                
            }
        }

        public float Aspect
        {
            get
            {
                unsafe { return InternalCalls.CameraComponent_GetAspect(ID); }
            }
            set
            {
                unsafe { InternalCalls.CameraComponent_SetAspect(ID, value); }
            }
        }

        public bool Current
        {
            get 
            {
                unsafe { return InternalCalls.CameraComponent_GetCurrent(ID); }
            }
            set
            {
                unsafe { InternalCalls.CameraComponent_SetCurrent(ID, value); }
            }
        }

        public CameraNode(ulong id) : base(id)
        {
        }

        public static new NodeTypes GetNodeType()
        {
            return NodeTypes.Camera;
        }
    }
}
