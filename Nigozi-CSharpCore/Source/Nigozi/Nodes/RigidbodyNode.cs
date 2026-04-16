using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public class RigidbodyNode : Node
    {
        public enum BodyType
        {
            Static = 0, Kinematic, Dynamic
        }

        public BodyType Type
        {
            get
            {
                unsafe { return (BodyType)InternalCalls.RigidbodyComponent_GetBodyType(ID); }
            }
            set
            {
                unsafe { InternalCalls.RigidbodyComponent_SetBodyType(ID, (ushort)value); }
            }
        }

        public bool FreezeRotation
        {
            get
            {
                unsafe { return InternalCalls.RigidbodyComponent_GetFreezeRotation(ID); }
            }
            set
            {
                unsafe { InternalCalls.RigidbodyComponent_SetFreezeRotation(ID, value); }
            }
        }

        public Vector2 ColliderSize
        {
            get
            {
                unsafe { return InternalCalls.BoxColliderComponent_GetSize(ID); }
            }
            set
            {
                unsafe { InternalCalls.BoxColliderComponent_SetSize(ID, value); }
            }
        }

        public Vector2 ColliderOffset
        {
            get
            {
                unsafe { return InternalCalls.BoxColliderComponent_GetOffset(ID); }
            }
            set
            {
                unsafe { InternalCalls.BoxColliderComponent_SetOffset(ID, value); }
            }
        }

        public RigidbodyNode(ulong id) : base(id)
        {
        }

        public RigidbodyNode() { }

        public void ApplyForce(Vector2 force)
        {
            unsafe { InternalCalls.RigidbodyComponent_ApplyForce(ID, force); }
        }

        public void ApplyImpulse(Vector2 force)
        {
            unsafe { InternalCalls.RigidbodyComponent_ApplyImpulse(ID, force); }
        }

        public static new NodeTypes GetNodeType()
        {
            return NodeTypes.Rigidbody;
        }
    }
}
