using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Nigozi;

namespace First
{
    public class FloatingNode : Node, IScriptBehaviour
    {
        public FloatingNode(ulong id) : base(id)
        {
        }

        public void OnStart()
        {
            Log.Info("FGame on!");
        }

        public void OnUpdate(float timestep)
        {
            Position = Position + new Vector2(timestep, 0);
            Log.Info($"FMoving! {timestep}");
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
