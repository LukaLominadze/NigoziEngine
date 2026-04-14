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
            Position = Position + new Vector2(timestep, 0);
            Log.Info($"Moving! {timestep}");
        }

        public void OnRender()
        {

            
        }

        public void OnImGuiRender()
        {
            
        }

        public void OnDestroy()
        {
            
        }
    }
}
