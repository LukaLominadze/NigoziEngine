#pragma once

#include "ngpch.h"

#include "Nigozi/core/Assert.h"
#include "Nigozi/core/Log.h"

#include "Nigozi/core/Environment.h"
#include "Nigozi/core/SubProcess.h"

#include "Nigozi/Benchmarking/Timer.h"

#include "Nigozi/layers/Layer.h"

#include "Nigozi/core/Application.h"
#include "Nigozi/core/InputKeyMap.h"
#include "Nigozi/core/Input.h"

#include "Nigozi/glcore/Renderer2D.h"
#include "Nigozi/glcore/Shader.h"
#include "Nigozi/glcore/Texture.h"
#include "Nigozi/glcore/OrthographicCamera.h"
#include "Nigozi/glcore/OrthographicCameraController.h"
#include "Nigozi/glcore/PerspectiveCamera.h"
#include "Nigozi/glcore/PerspectiveCameraController.h"
#include "Nigozi/glcore/ParticleSystem.h"
#include "Nigozi/glcore/FrameBuffer.h"

#include "Nigozi/audio/AudioEngine.h"
#include "Nigozi/audio/AudioGroup.h"
#include "Nigozi/audio/Audio.h"

#include "Nigozi/events/Event.h"
#include "Nigozi/events/ApplicationEvent.h"
#include "Nigozi/events/KeyEvent.h"
#include "Nigozi/events/MouseEvent.h"

#include "Nigozi/utils/StringUtils.h"
#include "Nigozi/utils/FileDialogue.h"

#include "Nigozi/scene/SceneTree.h"
#include "Nigozi/scene/Entity.h"
#include "Nigozi/scene/Component.h"

#include "Nigozi/scripts/ScriptEngine.h"
#include "Nigozi/scripts/ScriptGlue.h"

#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "project/Project.h"

#include "Nigozi/core/EntryPoint.h"
