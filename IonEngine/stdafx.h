#pragma once

#include "targetver.h"

#include <Windows.h>

#include <deque>
#include <string>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <gl/GL.h>

#include "../Common/Common.h"
#include "../IonScript/IonScript.h"

#include "../SDL2-2.0.3/include/SDL.h"
#include "../SDL2-2.0.3/include/SDL_syswm.h"

#pragma warning(push)
#pragma warning(disable:4100)

#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_sandbox_win.h>

#pragma warning(pop)