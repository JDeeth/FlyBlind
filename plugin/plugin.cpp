/// (c) Jack Deeth 2018 - see attached LICENSE and README.md

#include <XPLMPlugin.h>

#include <cstring>
#include <memory>
#include <string>

#include <GL/glew.h>

#include <XPLMDisplay.h>
#include <XPLMGraphics.h>

#include <command.h>
#include <dataref.h>

class StopSceneryRendering {
  // When created, stops most of X-Plane's scenery from being rendered. Instead
  // of drawing scenery, it draws a solid colour based on the ambient light.
  // When destroyed, restores normal X-Plane scenery rendering.
  static int clear_(int, int, void*);
  static PPL::DataRef<float> outside_r_;
  static PPL::DataRef<float> outside_g_;
  static PPL::DataRef<float> outside_b_;

public:
  StopSceneryRendering() {
    // disabling xplm_Phase_FirstScene hides lights, but causes glitches
    XPLMRegisterDrawCallback(clear_, xplm_Phase_Objects, 1, nullptr);
    XPLMRegisterDrawCallback(clear_, xplm_Phase_Airports, 1, nullptr);
    XPLMRegisterDrawCallback(clear_, xplm_Phase_Terrain, 1, nullptr);
    XPLMRegisterDrawCallback(clear_, xplm_Phase_Vectors, 1, nullptr);
  }
  ~StopSceneryRendering() {
    XPLMUnregisterDrawCallback(clear_, xplm_Phase_Objects, 1, nullptr);
    XPLMUnregisterDrawCallback(clear_, xplm_Phase_Airports, 1, nullptr);
    XPLMUnregisterDrawCallback(clear_, xplm_Phase_Terrain, 1, nullptr);
    XPLMUnregisterDrawCallback(clear_, xplm_Phase_Vectors, 1, nullptr);
  }
};

int StopSceneryRendering::clear_(int, int, void*) {
  glClearColor(outside_r_ * 0.75f, outside_g_ * 0.75f, outside_b_ * 0.75f, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  return 0;
}

PPL::DataRef<float> StopSceneryRendering::outside_r_{
    "sim/graphics/misc/outside_light_level_r"};
PPL::DataRef<float> StopSceneryRendering::outside_g_{
    "sim/graphics/misc/outside_light_level_g"};
PPL::DataRef<float> StopSceneryRendering::outside_b_{
    "sim/graphics/misc/outside_light_level_b"};

class FlyBlind {
  // When the command is triggered, either creates or destroys a
  // StopSceneryRendering object stored in the unique_ptr
  std::unique_ptr<StopSceneryRendering> hide_world_{nullptr};
  PPL::Command toggle_{
      "Albair/FlyBlind/toggle",  // <<<< change this to rename command
      "Make the world go away",
      [this](PPL::Command::Phase phase) {
        if (phase == PPL::Command::Phase::Begin) {
          if (hide_world_ == nullptr)
            hide_world_ = std::make_unique<StopSceneryRendering>();
          else
            hide_world_.reset();
        }
        return PPL::Command::Outcome::Halt;
      }};
};

static std::unique_ptr<FlyBlind> flyblind;

PLUGIN_API int XPluginStart(char* out_name, char* out_sig, char* out_desc) {
  strcpy(out_name, "FlyBlind");
  strcpy(out_sig, "FlyBlind");
  strcpy(out_desc, "Disables external drawing by command");

  flyblind = std::make_unique<FlyBlind>();

  return 1;
}

PLUGIN_API void XPluginStop() {}
PLUGIN_API int XPluginEnable() {
  return 1;
}
PLUGIN_API void XPluginDisable() {}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, long, void*) {}
