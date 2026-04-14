#pragma warning disable CS0108 // Member hides inherited member
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Nigozi;

namespace First
{
    public class Player : RigidbodyNode, IScriptBehaviour
    {
        public Player(ulong id) : base(id)
        {
        }

        public void OnStart()
        {
            Log.Info("Game on!");
        }   

        public void OnUpdate(float timestep)
        {
            float horizontalAxis = Input.GetAxis(Input.Keys.A, Input.Keys.D);
            Position += Vector2.Right * horizontalAxis * 10.0f * timestep;

            if (Input.IsKeyJustPressed(Input.Keys.Space))
            {
                Log.Info("Key just pressed!");
                ApplyImpulse(new Vector2(0.0f, 8.0f));
            }
        }

        public void OnDestroy()
        {
            Log.Warn($"I'm getting destroyed! {ID}");
        }
    }
}
#pragma warning restore CS0108
