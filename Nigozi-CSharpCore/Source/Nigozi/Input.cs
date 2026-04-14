using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public static class Input
    {
        public static bool IsKeyPressed(Keys key)
        {
            unsafe { return InternalCalls.Input_IsKeyPressed((int)key) == 1; }
        }

        public static bool IsKeyJustPressed(Keys key)
        {
            unsafe { return InternalCalls.Input_IsKeyJustPressed((int)key) == 1; }
        }

        public static bool IsKeyReleased(Keys key)
        {
            unsafe { return InternalCalls.Input_IsKeyReleased((int)key) == 1; }
        }

        public static bool IsKeyJustReleased(Keys key)
        {
            unsafe { return InternalCalls.Input_IsKeyJustReleased((int)key) == 1; }
        }

        public static bool IsMouseButtonPressed(Keys key)
        {
            unsafe { return InternalCalls.Input_IsMouseButtonPressed((int)key) == 1; }
        }

        public static bool IsMouseButtonJustPressed(Keys key)
        {
            unsafe { return InternalCalls.Input_IsMouseButtonJustPressed((int)key) == 1; }
        }

        public static bool IsMouseButtonReleased(Keys key)
        {
            unsafe { return InternalCalls.Input_IsMouseButtonReleased((int)key) == 1; }
        }

        public static bool IsMouseButtonJustReleased(Keys key)
        {
            unsafe { return InternalCalls.Input_IsMouseButtonJustReleased((int)key) == 1; }
        }

        public static float GetAxis(Keys negativeKey, Keys positiveKey)
        {
            unsafe { return InternalCalls.Input_GetAxis((int)negativeKey, (int)positiveKey); }
        }

        public static Vector2 GetAxis(Keys leftKey, Keys rightKey, Keys downKey, Keys upKey)
        {
            IVector2 horizontalKeys = new IVector2((int)leftKey, (int)rightKey);
            IVector2 verticalKeys = new IVector2((int)downKey, (int)upKey);

            unsafe { return InternalCalls.Input_GetVec2Axis(horizontalKeys, verticalKeys); }
        }

        public enum Keys
        {
                /* Printable = keys */
            Space = 32,
            Apostrophe = 39,  /* ' */
            Comma = 44,  /* , */
            Minus = 45,  /* - */
            Period = 46,  /* . */
            Slash = 47,  /* / */
            Key0 = 48,
            Key1 = 49,
            Key2 = 50,
            Key3 = 51,
            Key4 = 52,
            Key5 = 53,
            Key6 = 54,
            Key7 = 55,
            Key8 = 56,
            Key9 = 57,
            Semicolon = 59,  /* ; */
            Equal = 61,  /* = */
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LeftBracket = 91,  /* [ */
            Backslash = 92,  /* \ */
            RightBracket = 93,  /* ] */
            GraveAccent = 96,  /* ` */
            World1 = 161, /* non-US #1 */
            World2 = 162, /* non-US #2 */

                        /* Function = keys */
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,
            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,
            Page_UP = 266,
            Page_DOWN = 267,
            Home = 268,
            End = 269,
            Caps_LOCK = 280,
            Scroll_LOCK = 281,
            Num_LOCK = 282,
            Print_SCREEN = 283,
            Pause = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,
            Kp0 = 320,
            Kp1 = 321,
            Kp2 = 322,
            Kp3 = 323,
            Kp4 = 324,
            Kp5 = 325,
            Kp6 = 326,
            Kp7 = 327,
            Kp8 = 328,
            Kp9 = 329,
            KpDecimal = 330,
            KpDivide = 331,
            KpMultiply = 332,
            KpSubtract = 333,
            KpAdd = 334,
            KpEnter = 335,
            KpEqual = 336,
            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,
            LeftSuper = 343,
            RightShift = 344,
            RightControl = 345,
            RightAlt = 346,
            RightSuper = 347,
            Menu = 348,

            Last = Menu,

                        /*! @} */

                        /*! @defgroup = mods = Modifier = key = flags
                         *  @brief = Modifier = key = flags.
                         *
                         *  See [key = input](@ref = input_key) for = how = these = are = used.
                         *
                         *  @ingroup = input
                         *  @{ */

                        /*! @brief = If = this = bit = is = set = one = or = more = Shift = keys = were = held = down.
                         *
                         *  If = this = bit = is = set = one = or = more = Shift = keys = were = held = down.
                         */
            ModShift = 0x0001,
                        /*! @brief = If = this = bit = is = set = one = or = more = Control = keys = were = held = down.
                         *
                         *  If = this = bit = is = set = one = or = more = Control = keys = were = held = down.
                         */
            ModControl = 0x0002,
                        /*! @brief = If = this = bit = is = set = one = or = more = Alt = keys = were = held = down.
                         *
                         *  If = this = bit = is = set = one = or = more = Alt = keys = were = held = down.
                         */
            ModAlt = 0x0004,
                        /*! @brief = If = this = bit = is = set = one = or = more = Super = keys = were = held = down.
                         *
                         *  If = this = bit = is = set = one = or = more = Super = keys = were = held = down.
                         */
            ModSuper = 0x0008,
                        /*! @brief = If = this = bit = is = set = the = Caps = Lock = key = is = enabled.
                         *
                         *  If = this = bit = is = set = the = Caps = Lock = key = is = enabled = and = the @ref
                         *  LOCK_KeyMods = input = mode = is = set.
                         */
            ModCapsLock = 0x0010,
                        /*! @brief = If = this = bit = is = set = the = Num = Lock = key = is = enabled.
                         *
                         *  If = this = bit = is = set = the = Num = Lock = key = is = enabled = and = the @ref
                         *  LOCK_KeyMods = input = mode = is = set.
                         */
            ModNumLock = 0x0020,

            MouseButton1 = 0,
            MouseButton2 = 1,
            MouseButton3 = 2,
            MouseButton4 = 3,
            MouseButton5 = 4,
            MouseButton6 = 5,
            MouseButton7 = 6,
            MouseButton8 = 7,
            MouseButtonLast = MouseButton8,
            MouseButtonLeft = MouseButton1,
            MouseButtonRight = MouseButton2,
            MouseButtonMiddle = MouseButton3,

            Joystick1 = 0,
            Joystick2 = 1,
            Joystick3 = 2,
            Joystick4 = 3,
            Joystick5 = 4,
            Joystick6 = 5,
            Joystick7 = 6,
            Joystick8 = 7,
            Joystick9 = 8,
            Joystick10 = 9,
            Joystick11 = 10,
            Joystick12 = 11,
            Joystick13 = 12,
            Joystick14 = 13,
            Joystick15 = 14,
            Joystick16 = 15,
            JOYSTICK_LAST = Joystick16,

            GamepadButtonA = 0,
            GamepadButtonB = 1,
            GamepadButtonX = 2,
            GamepadButtonY = 3,
            GamepadButtonLeftBumper = 4,
            GamepadButtonRightBumper = 5,
            GamepadButtonBack = 6,
            GamepadButtonStart = 7,
            GamepadButtonGuide = 8,
            GamepadButtonLeftThumb = 9,
            GamepadButtonRightThumb = 10,
            GamepadButtonDpadUp = 11,
            GamepadButtonDpadRight = 12,
            GamepadButtonDpadDown = 13,
            GamepadButtonDpadLeft = 14,
            GamepadButtonLast = GamepadButtonDpadLeft,

            GamepadButtonCross = GamepadButtonA,
            GamepadButtonCircle = GamepadButtonB,
            GamepadButtonSquare = GamepadButtonX,
            GamepadButtonTriangle = GamepadButtonY,

            GamepadAxisLeftX = 0,
            GamepadAxisLeftY = 1,
            GamepadAxisRightX = 2,
            GamepadAxisRightY = 3,
            GamepadAxisLeftTrigger = 4,
            GamepadAxisRightTrigger = 5,
            GamepadAxisLast = GamepadAxisRightTrigger
                        /*! @} */
        }
    }
}
