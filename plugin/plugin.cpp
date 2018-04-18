#include <XPLMPlugin.h>
#include <cstring>
#include <string>
#include <memory>

#include <GL/glew.h>

#include <XPLMGraphics.h>
#include <XPLMDisplay.h>

#include <dataref.h>
#include <command.h>

class FlyBlind {
public:
  FlyBlind() {
    XPLMRegisterDrawCallback(cb, xplm_Phase_Airports, 1, this);
    XPLMRegisterDrawCallback(cb, xplm_Phase_Terrain, 1, this);
    XPLMRegisterDrawCallback(cb, xplm_Phase_Vectors, 1, this);
  }
  ~FlyBlind() {
    XPLMUnregisterDrawCallback(cb, xplm_Phase_Airports, 1, this);
    XPLMUnregisterDrawCallback(cb, xplm_Phase_Terrain, 1, this);
    XPLMUnregisterDrawCallback(cb, xplm_Phase_Vectors, 1, this);
  }
  XPLMDrawCallback_f cb{[](int, int, void* us_ptr) {
    auto us = reinterpret_cast<FlyBlind*>(us_ptr);
    if (us->draw)
      return 1;
    glClearColor(
        us->outside_r * 0.75f, us->outside_g * 0.75f, us->outside_b * 0.75f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
  }};
  bool draw{true};
  PPL::Command cmd{"Albair/FlyBlind/toggle",
                   "Make the world go away",
                   [this](PPL::Command::Phase phase) {
                     if (phase == PPL::Command::Phase::Begin)
                       draw = !draw;
                     return PPL::Command::Outcome::Halt;
                   }};
  PPL::DataRef<float> outside_r{"sim/graphics/misc/outside_light_level_r"};
  PPL::DataRef<float> outside_g{"sim/graphics/misc/outside_light_level_g"};
  PPL::DataRef<float> outside_b{"sim/graphics/misc/outside_light_level_b"};
};

static std::unique_ptr<FlyBlind> flyblind;

PLUGIN_API int XPluginStart(char* out_name, char* out_sig, char* out_desc) {
  std::string name{"FlyBlind"};
  strcpy(out_name, name.c_str());
  strcpy(out_sig, name.c_str());
  std::string desc{"Disables external drawing by command"};
  strcpy(out_desc, desc.c_str());

  flyblind = std::make_unique<FlyBlind>();
  return 1;
}

PLUGIN_API void XPluginStop() {}
PLUGIN_API int XPluginEnable() {
  return 1;
}
PLUGIN_API void XPluginDisable() {}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, long, void*) {}
