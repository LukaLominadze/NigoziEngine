#include <GLFW/glfw3.h>

namespace Nigozi
{
namespace Input
{
	/* Printable keys */
#define NG_KEY_SPACE               GLFW_KEY_SPACE
#define NG_KEY_APOSTROPHE               GLFW_KEY_APOSTROPHE  /* ' */
#define NG_KEY_COMMA               GLFW_KEY_COMMA  /* , */
#define NG_KEY_MINUS               GLFW_KEY_MINUS  /* - */
#define NG_KEY_PERIOD               GLFW_KEY_PERIOD  /* . */
#define NG_KEY_SLASH               GLFW_KEY_SLASH  /* / */
#define NG_KEY_0               GLFW_KEY_0
#define NG_KEY_1               GLFW_KEY_1
#define NG_KEY_2               GLFW_KEY_2
#define NG_KEY_3               GLFW_KEY_3
#define NG_KEY_4               GLFW_KEY_4
#define NG_KEY_5               GLFW_KEY_5
#define NG_KEY_6               GLFW_KEY_6
#define NG_KEY_7               GLFW_KEY_7
#define NG_KEY_8               GLFW_KEY_8
#define NG_KEY_9               GLFW_KEY_9
#define NG_KEY_SEMICOLON               GLFW_KEY_SEMICOLON  /* ; */
#define NG_KEY_EQUAL               GLFW_KEY_EQUAL  /* = */
#define NG_KEY_A               GLFW_KEY_A
#define NG_KEY_B               GLFW_KEY_B
#define NG_KEY_C               GLFW_KEY_C
#define NG_KEY_D               GLFW_KEY_D
#define NG_KEY_E               GLFW_KEY_E
#define NG_KEY_F               GLFW_KEY_F
#define NG_KEY_G               GLFW_KEY_G
#define NG_KEY_H               GLFW_KEY_H
#define NG_KEY_I               GLFW_KEY_I
#define NG_KEY_J               GLFW_KEY_J
#define NG_KEY_K               GLFW_KEY_K
#define NG_KEY_L               GLFW_KEY_L
#define NG_KEY_M               GLFW_KEY_M
#define NG_KEY_N               GLFW_KEY_N
#define NG_KEY_O               GLFW_KEY_O
#define NG_KEY_P               GLFW_KEY_P
#define NG_KEY_Q               GLFW_KEY_Q
#define NG_KEY_R               GLFW_KEY_R
#define NG_KEY_S               GLFW_KEY_S
#define NG_KEY_T               GLFW_KEY_T
#define NG_KEY_U               GLFW_KEY_U
#define NG_KEY_V               GLFW_KEY_V
#define NG_KEY_W               GLFW_KEY_W
#define NG_KEY_X               GLFW_KEY_X
#define NG_KEY_Y               GLFW_KEY_Y
#define NG_KEY_Z               GLFW_KEY_Z
#define NG_KEY_LEFT_BRACKET               GLFW_KEY_LEFT_BRACKET  /* [ */
#define NG_KEY_BACKSLASH               GLFW_KEY_BACKSLASH  /* \ */
#define NG_KEY_RIGHT_BRACKET               GLFW_KEY_RIGHT_BRACKET  /* ] */
#define NG_KEY_GRAVE_ACCENT               GLFW_KEY_GRAVE_ACCENT  /* ` */
#define NG_KEY_WORLD_1               GLFW_KEY_WORLD_1 /* non-US #1 */
#define NG_KEY_WORLD_2               GLFW_KEY_WORLD_2 /* non-US #2 */

/* Function keys */
#define NG_KEY_ESCAPE               GLFW_KEY_ESCAPE
#define NG_KEY_ENTER               GLFW_KEY_ENTER
#define NG_KEY_TAB               GLFW_KEY_TAB
#define NG_KEY_BACKSPACE               GLFW_KEY_BACKSPACE
#define NG_KEY_INSERT               GLFW_KEY_INSERT
#define NG_KEY_DELETE               GLFW_KEY_DELETE
#define NG_KEY_RIGHT               GLFW_KEY_RIGHT
#define NG_KEY_LEFT               GLFW_KEY_LEFT
#define NG_KEY_DOWN               GLFW_KEY_DOWN
#define NG_KEY_UP               GLFW_KEY_UP
#define NG_KEY_PAGE_UP               GLFW_KEY_PAGE_UP
#define NG_KEY_PAGE_DOWN               GLFW_KEY_PAGE_DOWN
#define NG_KEY_HOME               GLFW_KEY_HOME
#define NG_KEY_END               GLFW_KEY_END
#define NG_KEY_CAPS_LOCK               GLFW_KEY_CAPS_LOCK
#define NG_KEY_SCROLL_LOCK               GLFW_KEY_SCROLL_LOCK
#define NG_KEY_NUM_LOCK               GLFW_KEY_NUM_LOCK
#define NG_KEY_PRINT_SCREEN               GLFW_KEY_PRINT_SCREEN
#define NG_KEY_PAUSE               GLFW_KEY_PAUSE
#define NG_KEY_F1               GLFW_KEY_F1
#define NG_KEY_F2               GLFW_KEY_F2
#define NG_KEY_F3               GLFW_KEY_F3
#define NG_KEY_F4               GLFW_KEY_F4
#define NG_KEY_F5               GLFW_KEY_F5
#define NG_KEY_F6               GLFW_KEY_F6
#define NG_KEY_F7               GLFW_KEY_F7
#define NG_KEY_F8               GLFW_KEY_F8
#define NG_KEY_F9               GLFW_KEY_F9
#define NG_KEY_F10               GLFW_KEY_F10
#define NG_KEY_F11               GLFW_KEY_F11
#define NG_KEY_F12               GLFW_KEY_F12
#define NG_KEY_F13               GLFW_KEY_F13
#define NG_KEY_F14               GLFW_KEY_F14
#define NG_KEY_F15               GLFW_KEY_F15
#define NG_KEY_F16               GLFW_KEY_F16
#define NG_KEY_F17               GLFW_KEY_F17
#define NG_KEY_F18               GLFW_KEY_F18
#define NG_KEY_F19               GLFW_KEY_F19
#define NG_KEY_F20               GLFW_KEY_F20
#define NG_KEY_F21               GLFW_KEY_F21
#define NG_KEY_F22               GLFW_KEY_F22
#define NG_KEY_F23               GLFW_KEY_F23
#define NG_KEY_F24               GLFW_KEY_F24
#define NG_KEY_F25               GLFW_KEY_F25
#define NG_KEY_KP_0               GLFW_KEY_KP_0
#define NG_KEY_KP_1               GLFW_KEY_KP_1
#define NG_KEY_KP_2               GLFW_KEY_KP_2
#define NG_KEY_KP_3               GLFW_KEY_KP_3
#define NG_KEY_KP_4               GLFW_KEY_KP_4
#define NG_KEY_KP_5               GLFW_KEY_KP_5
#define NG_KEY_KP_6               GLFW_KEY_KP_6
#define NG_KEY_KP_7               GLFW_KEY_KP_7
#define NG_KEY_KP_8               GLFW_KEY_KP_8
#define NG_KEY_KP_9               GLFW_KEY_KP_9
#define NG_KEY_KP_DECIMAL               GLFW_KEY_KP_DECIMAL
#define NG_KEY_KP_DIVIDE               GLFW_KEY_KP_DIVIDE
#define NG_KEY_KP_MULTIPLY               GLFW_KEY_KP_MULTIPLY
#define NG_KEY_KP_SUBTRACT               GLFW_KEY_KP_SUBTRACT
#define NG_KEY_KP_ADD               GLFW_KEY_KP_ADD
#define NG_KEY_KP_ENTER               GLFW_KEY_KP_ENTER
#define NG_KEY_KP_EQUAL               GLFW_KEY_KP_EQUAL
#define NG_KEY_LEFT_SHIFT               GLFW_KEY_LEFT_SHIFT
#define NG_KEY_LEFT_CONTROL               GLFW_KEY_LEFT_CONTROL
#define NG_KEY_LEFT_ALT               GLFW_KEY_LEFT_ALT
#define NG_KEY_LEFT_SUPER               GLFW_KEY_LEFT_SUPER
#define NG_KEY_RIGHT_SHIFT               GLFW_KEY_RIGHT_SHIFT
#define NG_KEY_RIGHT_CONTROL               GLFW_KEY_RIGHT_CONTROL
#define NG_KEY_RIGHT_ALT               GLFW_KEY_RIGHT_ALT
#define NG_KEY_RIGHT_SUPER               GLFW_KEY_RIGHT_SUPER
#define NG_KEY_MENU               GLFW_KEY_MENU

#define NG_KEY_LAST               GLFW_KEY_LAST               GLFW_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define NG_MOD_SHIFT               GLFW_MOD_SHIFT
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define NG_MOD_CONTROL               GLFW_MOD_CONTROL
   /*! @brief If this bit is set one or more Alt keys were held down.
	*
	*  If this bit is set one or more Alt keys were held down.
	*/
#define NG_MOD_ALT               GLFW_MOD_ALT
	/*! @brief If this bit is set one or more Super keys were held down.
	 *
	 *  If this bit is set one or more Super keys were held down.
	 */
#define NG_MOD_SUPER               GLFW_MOD_SUPER
	 /*! @brief If this bit is set the Caps Lock key is enabled.
	  *
	  *  If this bit is set the Caps Lock key is enabled and the @ref
	  *  GLFW_LOCK_KEY_MODS input mode is set.
	  */
#define NG_MOD_CAPS_LOCK               GLFW_MOD_CAPS_LOCK
	  /*! @brief If this bit is set the Num Lock key is enabled.
	   *
	   *  If this bit is set the Num Lock key is enabled and the @ref
	   *  GLFW_LOCK_KEY_MODS input mode is set.
	   */
#define NG_MOD_NUM_LOCK               GLFW_MOD_NUM_LOCK

	   /*! @} */

	   /*! @defgroup buttons Mouse buttons
		*  @brief Mouse button IDs.
		*
		*  See [mouse button input](@ref input_mouse_button) for how these are used.
		*
		*  @ingroup input
		*  @{ */
#define NG_MOUSE_BUTTON_1               GLFW_MOUSE_BUTTON_1
#define NG_MOUSE_BUTTON_2               GLFW_MOUSE_BUTTON_2
#define NG_MOUSE_BUTTON_3               GLFW_MOUSE_BUTTON_3
#define NG_MOUSE_BUTTON_4               GLFW_MOUSE_BUTTON_4
#define NG_MOUSE_BUTTON_5               GLFW_MOUSE_BUTTON_5
#define NG_MOUSE_BUTTON_6               GLFW_MOUSE_BUTTON_6
#define NG_MOUSE_BUTTON_7               GLFW_MOUSE_BUTTON_7
#define NG_MOUSE_BUTTON_8               GLFW_MOUSE_BUTTON_8
#define NG_MOUSE_BUTTON_LAST               GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define NG_MOUSE_BUTTON_LEFT               GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define NG_MOUSE_BUTTON_RIGHT               GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define NG_MOUSE_BUTTON_MIDDLE               GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
		/*! @} */

		/*! @defgroup joysticks Joysticks
		 *  @brief Joystick IDs.
		 *
		 *  See [joystick input](@ref joystick) for how these are used.
		 *
		 *  @ingroup input
		 *  @{ */
#define NG_JOYSTICK_1               GLFW_JOYSTICK_1
#define NG_JOYSTICK_2               GLFW_JOYSTICK_2
#define NG_JOYSTICK_3               GLFW_JOYSTICK_3
#define NG_JOYSTICK_4               GLFW_JOYSTICK_4
#define NG_JOYSTICK_5               GLFW_JOYSTICK_5
#define NG_JOYSTICK_6               GLFW_JOYSTICK_6
#define NG_JOYSTICK_7               GLFW_JOYSTICK_7
#define NG_JOYSTICK_8               GLFW_JOYSTICK_8
#define NG_JOYSTICK_9               GLFW_JOYSTICK_9
#define NG_JOYSTICK_10               GLFW_JOYSTICK_10
#define NG_JOYSTICK_11               GLFW_JOYSTICK_11
#define NG_JOYSTICK_12               GLFW_JOYSTICK_12
#define NG_JOYSTICK_13               GLFW_JOYSTICK_13
#define NG_JOYSTICK_14               GLFW_JOYSTICK_14
#define NG_JOYSTICK_15               GLFW_JOYSTICK_15
#define NG_JOYSTICK_16               GLFW_JOYSTICK_16
#define NG_JOYSTICK_LAST               GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16
		 /*! @} */

		 /*! @defgroup gamepad_buttons Gamepad buttons
		  *  @brief Gamepad buttons.
		  *
		  *  See @ref gamepad for how these are used.
		  *
		  *  @ingroup input
		  *  @{ */
#define NG_GAMEPAD_BUTTON_A               GLFW_GAMEPAD_BUTTON_A
#define NG_GAMEPAD_BUTTON_B               GLFW_GAMEPAD_BUTTON_B
#define NG_GAMEPAD_BUTTON_X               GLFW_GAMEPAD_BUTTON_X
#define NG_GAMEPAD_BUTTON_Y               GLFW_GAMEPAD_BUTTON_Y
#define NG_GAMEPAD_BUTTON_LEFT_BUMPER               GLFW_GAMEPAD_BUTTON_LEFT_BUMPER
#define NG_GAMEPAD_BUTTON_RIGHT_BUMPER               GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER
#define NG_GAMEPAD_BUTTON_BACK               GLFW_GAMEPAD_BUTTON_BACK
#define NG_GAMEPAD_BUTTON_START               GLFW_GAMEPAD_BUTTON_START
#define NG_GAMEPAD_BUTTON_GUIDE               GLFW_GAMEPAD_BUTTON_GUIDE
#define NG_GAMEPAD_BUTTON_LEFT_THUMB               GLFW_GAMEPAD_BUTTON_LEFT_THUMB
#define NG_GAMEPAD_BUTTON_RIGHT_THUMB               GLFW_GAMEPAD_BUTTON_RIGHT_THUMB
#define NG_GAMEPAD_BUTTON_DPAD_UP               GLFW_GAMEPAD_BUTTON_DPAD_UP
#define NG_GAMEPAD_BUTTON_DPAD_RIGHT               GLFW_GAMEPAD_BUTTON_DPAD_RIGHT
#define NG_GAMEPAD_BUTTON_DPAD_DOWN               GLFW_GAMEPAD_BUTTON_DPAD_DOWN
#define NG_GAMEPAD_BUTTON_DPAD_LEFT               GLFW_GAMEPAD_BUTTON_DPAD_LEFT
#define NG_GAMEPAD_BUTTON_LAST               GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

#define NG_GAMEPAD_BUTTON_CROSS               GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define NG_GAMEPAD_BUTTON_CIRCLE               GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define NG_GAMEPAD_BUTTON_SQUARE               GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define NG_GAMEPAD_BUTTON_TRIANGLE               GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y
		  /*! @} */

		  /*! @defgroup gamepad_axes Gamepad axes
		   *  @brief Gamepad axes.
		   *
		   *  See @ref gamepad for how these are used.
		   *
		   *  @ingroup input
		   *  @{ */
#define NG_GAMEPAD_AXIS_LEFT_X               GLFW_GAMEPAD_AXIS_LEFT_X
#define NG_GAMEPAD_AXIS_LEFT_Y               GLFW_GAMEPAD_AXIS_LEFT_Y
#define NG_GAMEPAD_AXIS_RIGHT_X               GLFW_GAMEPAD_AXIS_RIGHT_X
#define NG_GAMEPAD_AXIS_RIGHT_Y               GLFW_GAMEPAD_AXIS_RIGHT_Y
#define NG_GAMEPAD_AXIS_LEFT_TRIGGER               GLFW_GAMEPAD_AXIS_LEFT_TRIGGER
#define NG_GAMEPAD_AXIS_RIGHT_TRIGGER               GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
#define NG_GAMEPAD_AXIS_LAST               GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
		   /*! @} */
}
}