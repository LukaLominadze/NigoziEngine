using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public interface IScriptBehaviour
    {
        public void OnStart();
        public void OnUpdate(float timestep);
        public void OnRender();
        public void OnImGuiRender();
        public void OnDestroy();
    }
}
