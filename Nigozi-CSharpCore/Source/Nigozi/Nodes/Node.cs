using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Coral.Managed.Interop;

namespace Nigozi
{
    public class Node : INodeType
    {
        private ulong id;
        public ulong ID
        {
            get => id;
        }
        public Node? Parent
        {
            get
            {
                ulong parentId = 0;
                unsafe { parentId = InternalCalls.SceneTree_GetParent(id); }

                if (parentId == 0)
                {
                    return null;
                }

                return new Node(parentId);
            }
            set
            {
                ulong newParentId = 0;
                if (value != null)
                {
                    newParentId = value.ID;
                }
                unsafe
                {
                    if (InternalCalls.SceneTree_SetParent(id, newParentId) == 1)
                    {
                        id = newParentId;
                    }
                }
            }
        }

        private Transform Transform
        {
            get
            {
                Transform _transform;
                unsafe { InternalCalls.TransformComponent_GetTransform(ID, &_transform); }
                return _transform;
            }
            set
            {
                Transform _transform = value;
                unsafe { InternalCalls.TransformComponent_SetTransform(ID, &_transform); }
            }
        }
        private Transform GlobalTransform
        {
            get
            {
                Transform _transform;
                unsafe { InternalCalls.TransformComponent_GetWorldTransform(ID, &_transform); }
                return _transform;
            }
            set
            {
                Transform _transform = value;
                unsafe { InternalCalls.TransformComponent_SetWorldTransform(ID, &_transform); }
            }
        }

        public string Name
        {
            get
            {
                NativeString nativeName;
                unsafe { nativeName = InternalCalls.NameComponent_GetName(ID); }
                string name = nativeName!;
                return name;
            }
            set
            {
                NativeString nativeName = value;
                unsafe { InternalCalls.NameComponent_SetName(ID, nativeName); }
            }
        }

        public string Tag
        {
            get
            {
                NativeString nativeTag;
                unsafe { nativeTag = InternalCalls.TagComponent_GetTag(ID); }
                string tag = nativeTag!;
                return tag;
            }
            set
            {
                NativeString nativeTag = value;
                unsafe { InternalCalls.TagComponent_SetTag(ID, nativeTag); }
            }
        }

        public Vector2 Position
        {
            get => Transform.Position;
            set
            {
                Transform temp = Transform;
                temp.Position = value;
                Transform = temp;
            }
        }

        public Vector2 Scale
        {
            get => Transform.Scale;
            set
            {
                Transform temp = Transform;
                temp.Scale = value;
                Transform = temp;
            }
        }

        public float Rotation
        {
            get => Transform.Rotation;
            set
            {
                Transform temp = Transform;
                temp.Rotation = value;
                Transform = temp;
            }
        }

        public Vector2 GlobalPosition
        {
            get => GlobalTransform.Position;
            set
            {
                Transform temp = GlobalTransform;
                temp.Position = value;
                GlobalTransform = temp;
            }
        }

        public Vector2 GlobalScale
        {
            get => GlobalTransform.Scale;
            set
            {
                Transform temp = GlobalTransform;
                temp.Scale = value;
                GlobalTransform = temp;
            }
        }

        public float GlobalRotation
        {
            get => GlobalTransform.Rotation;
            set
            {
                Transform temp = GlobalTransform;
                temp.Rotation = value;
                GlobalTransform = temp;
            }
        }

        public Node(ulong id)
        {
            this.id = id;
        }

        public Node() { }

        public T As<T>() where T : Node
        {
            return (this as T)!;
        }
        public void OnStart() { }
        public void OnUpdate(float timestep) { }
        public void OnRender() { }
        public void OnImGuiRender() { }
        public void OnDestroy() { }
        private void SetID(ulong id)
        {
            this.id = id;
        }

        public static NodeTypes GetNodeType()
        {
            return NodeTypes.Node;
        }

        private static Type NodeTypeToType(NodeTypes type)
        {
            switch(type)
            {
                case NodeTypes.Node: return typeof(Node);
                case NodeTypes.Scene: return typeof(SceneNode);
                case NodeTypes.Camera: return typeof(CameraNode);
                case NodeTypes.SpriteRenderer: return typeof(SpriteNode);
                case NodeTypes.Rigidbody: return typeof(RigidbodyNode);
                case NodeTypes.AudioStreamPlayer: return typeof(AudioStreamPlayerNode);
            }
            return typeof(Node);
        }
    }
}
