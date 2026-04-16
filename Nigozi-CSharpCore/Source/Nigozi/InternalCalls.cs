using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Coral.Managed.Interop;

namespace Nigozi
{
#pragma warning disable 0649
    internal static unsafe class InternalCalls
    {
        #region Log
        internal static delegate* unmanaged<NativeString, void> Log_Info;
        internal static delegate* unmanaged<NativeString, void> Log_Warn;
        internal static delegate* unmanaged<NativeString, void> Log_Error;
        internal static delegate* unmanaged<NativeString, void> Log_Critical;
        #endregion

        #region SceneTree
        internal static delegate* unmanaged<ushort, ulong> SceneTree_CreateNode;
        internal static delegate* unmanaged<ulong, ulong> SceneTree_GetParent;
        internal static delegate* unmanaged<ulong, ulong, byte> SceneTree_SetParent;
        internal static delegate* unmanaged<ulong, void> SceneTree_DestroyNode;
        #endregion

        #region Input
        internal static delegate* unmanaged<int, byte> Input_IsKeyPressed;
        internal static delegate* unmanaged<int, byte> Input_IsKeyJustPressed;
        internal static delegate* unmanaged<int, byte> Input_IsKeyReleased;
        internal static delegate* unmanaged<int, byte> Input_IsKeyJustReleased;

        internal static delegate* unmanaged<int, byte> Input_IsMouseButtonPressed;
        internal static delegate* unmanaged<int, byte> Input_IsMouseButtonJustPressed;
        internal static delegate* unmanaged<int, byte> Input_IsMouseButtonReleased;
        internal static delegate* unmanaged<int, byte> Input_IsMouseButtonJustReleased;

        internal static delegate* unmanaged<int, int, float> Input_GetAxis;
        internal static delegate* unmanaged<IVector2, IVector2, Vector2> Input_GetVec2Axis;
        #endregion

        #region NameComponent
        internal static delegate* unmanaged<ulong, NativeString> NameComponent_GetName;
        internal static delegate* unmanaged<ulong, NativeString, void> NameComponent_SetName;
        #endregion

        #region TagComponent
        internal static delegate* unmanaged<ulong, NativeString> TagComponent_GetTag;
        internal static delegate* unmanaged<ulong, NativeString, void> TagComponent_SetTag;
        #endregion

        #region TransformComponent
        internal static delegate* unmanaged<ulong, Transform*, void> TransformComponent_GetTransform;
        internal static delegate* unmanaged<ulong, Transform*, void> TransformComponent_SetTransform;
        internal static delegate* unmanaged<ulong, Transform*, void> TransformComponent_GetWorldTransform;
        internal static delegate* unmanaged<ulong, Transform*, void> TransformComponent_SetWorldTransform;
        #endregion

        #region CameraComponent
        internal static delegate* unmanaged<ulong, float> CameraComponent_GetZoom;
        internal static delegate* unmanaged<ulong, float> CameraComponent_GetAspect;
        internal static delegate* unmanaged<ulong, bool> CameraComponent_GetCurrent;
        internal static delegate* unmanaged<ulong, float, void> CameraComponent_SetZoom;
        internal static delegate* unmanaged<ulong, float, void> CameraComponent_SetAspect;
        internal static delegate* unmanaged<ulong, bool, void> CameraComponent_SetCurrent;
        #endregion

        #region SpriteRendererComponent
        internal static delegate* unmanaged<ulong, IVector2> SpriteRendererComponent_GetSlot;
        internal static delegate* unmanaged<ulong, IVector2, void> SpriteRendererComponent_SetSlot;
        internal static delegate* unmanaged<ulong, IVector2> SpriteRendererComponent_GetSeperator;
        internal static delegate* unmanaged<ulong, IVector2, void> SpriteRendererComponent_SetSeperator;
        #endregion

        #region RigidbodyComponent
        internal static delegate* unmanaged<ulong, ushort> RigidbodyComponent_GetBodyType;
        internal static delegate* unmanaged<ulong, ushort, void> RigidbodyComponent_SetBodyType;
        internal static delegate* unmanaged<ulong, bool> RigidbodyComponent_GetFreezeRotation;
        internal static delegate* unmanaged<ulong, bool, void> RigidbodyComponent_SetFreezeRotation;
        internal static delegate* unmanaged<ulong, Vector2, void> RigidbodyComponent_ApplyForce;
        internal static delegate* unmanaged<ulong, Vector2, void> RigidbodyComponent_ApplyImpulse;
        #endregion

        #region BoxColliderComponent
        internal static delegate* unmanaged<ulong, Vector2> BoxColliderComponent_GetSize;
        internal static delegate* unmanaged<ulong, Vector2, void> BoxColliderComponent_SetSize;
        internal static delegate* unmanaged<ulong, Vector2> BoxColliderComponent_GetOffset;
        internal static delegate* unmanaged<ulong, Vector2, void> BoxColliderComponent_SetOffset;
        #endregion

        #region AudioStreamPlayerComponent
        internal static delegate* unmanaged<ulong, float> AudioStreamPlayerComponent_GetVolume;
        internal static delegate* unmanaged<ulong, float, void> AudioStreamPlayerComponent_SetVolume;
        internal static delegate* unmanaged<ulong, bool> AudioStreamPlayerComponent_GetPlaying;
        internal static delegate* unmanaged<ulong, bool, void> AudioStreamPlayerComponent_SetPlaying;
        internal static delegate* unmanaged<ulong, bool> AudioStreamPlayerComponent_GetIsPaused;
        internal static delegate* unmanaged<ulong, bool, void> AudioStreamPlayerComponent_SetIsPaused;
        internal static delegate* unmanaged<ulong, void> AudioStreamPlayerComponent_Stop;
        #endregion
    }
#pragma warning restore 0649
}
