using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Nigozi;

namespace First
{
    internal class Player : RigidbodyNode
    {
        private Vector4 startColor = Vector4.Zero;
        private SpriteNode sprite;
        private float direction = -1.0f;

        public void OnStart()
        {
            Log.Info("Game on!");
            SpriteNode? child = GetChild<SpriteNode>(0);
            if (child != null)
            {
                Log.Info($"{child.GetType().Name} {child.ID}");
                sprite = child;
                startColor = child.Color;
                child.Color = new Vector4(0.2f, 0.2f, 0.9f, 1.0f);
            }
        }

        public void OnUpdate(float timestep)
        {
            float horizontalAxis = Input.GetAxis(Keys.A, Keys.D);
            Position += new Vector2(horizontalAxis * 5.0f * timestep, 0.0f);

            if (Input.IsKeyJustPressed(Keys.Space))
            {
                ApplyImpulse(Vector2.Up * 8.0f);
            }

            if (sprite.Color.X < 0.2f)
            {
                direction = 1;
            }
            else if (sprite.Color.X > 0.9f)
            {
                direction = -1;
            }
            sprite.Color += new Vector4(1.0f, 1.0f, 1.0f) * direction * 2.0f * timestep;
        }

        public void OnDestroy()
        {
            Log.Warn($"Destroyed {ID}");
        }
    }
}
