using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public struct Transform
    {
        public Vector2 Position = Vector2.Zero;
        public Vector2 Scale = Vector2.Zero;
        public float Rotation = 0.0f;

        public Transform(Vector2 position, Vector2 scale, float rotation = 0.0f)
        {
            this.Position = position;
            this.Scale = scale;
            this.Rotation = rotation;
        }

        public Transform(Vector2 position)
        {
            this.Position = position;
        }

        public Transform() { }
    }
}
