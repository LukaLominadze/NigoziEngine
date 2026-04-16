using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public class AudioStreamPlayerNode : Node
    {
        public float Volume
        {
            get
            {
                unsafe { return InternalCalls.AudioStreamPlayerComponent_GetVolume(ID); }
            }
            set
            {
                unsafe { InternalCalls.AudioStreamPlayerComponent_SetVolume(ID, value); }
            }
        }

        public void Play()
        {
            unsafe { InternalCalls.AudioStreamPlayerComponent_SetPlaying(ID, true); }
        }

        public void Pause()
        {
            unsafe { InternalCalls.AudioStreamPlayerComponent_SetIsPaused(ID, true); }
        }

        public void Resume()
        {
            unsafe { InternalCalls.AudioStreamPlayerComponent_SetIsPaused(ID, false); }
        }

        public void Stop()
        {
            unsafe { InternalCalls.AudioStreamPlayerComponent_Stop(ID); }
        }

        public bool IsPlaying()
        {
            unsafe { return InternalCalls.AudioStreamPlayerComponent_GetPlaying(ID); }
        }

        public bool IsPaused()
        {
            unsafe { return InternalCalls.AudioStreamPlayerComponent_GetIsPaused(ID); }
        }

        public AudioStreamPlayerNode(ulong id) : base(id)
        {
        }

        public AudioStreamPlayerNode() { }

        public static new NodeTypes GetNodeType()
        {
            return NodeTypes.AudioStreamPlayer;
        }
    }
}
