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
        public void OnStart()
        {
            Log.Info("Game on!");
        }

        public void OnUpdate(float timestep)
        {
            float horizontalAxis = Input.GetAxis(Keys.A, Keys.D);
            Position += new Vector2(horizontalAxis * 5.0f * timestep, 0.0f);

            if (Input.IsKeyJustPressed(Keys.Space))
            {
                ApplyImpulse(Vector2.Up * 8.0f);
            }
        }

        public void OnDestroy()
        {
            Log.Warn($"Destroyed {ID}");
        }
    }
}
