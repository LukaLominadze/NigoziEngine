using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Nigozi;

namespace MyProject
{
    internal class MyScript : RigidbodyNode
    {
        public void OnStart()
        {
            Log.Info("This is the beggining of the game!");
        }

        public void OnUpdate(float timestep)
        {
            float horizontalAxis = Input.GetAxis(Keys.A, Keys.D);
            Position += Vector2.Right * horizontalAxis * 5.0f * timestep;

            if (Input.IsKeyJustPressed(Keys.Space))
            {
                ApplyImpulse(Vector2.Up * 8.0f);
            }
        }

        public void OnRender()
        {
            // This is for rendering
        }

        public void OnImGuiRender()
        {
            // This is for gizmo rendering
        }

        public void OnDestroy()
        {
            Log.Warn($"I'm getting destroyed! {ID}");
        }
    }
}
