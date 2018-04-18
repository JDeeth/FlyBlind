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
  static PPL::DataRef<float> outside_r_;
  static PPL::DataRef<float> outside_g_;
  static PPL::DataRef<float> outside_b_;
  static int clear_(int, int, void*);

  class MakeBlank {
  public:
    MakeBlank() {
      XPLMRegisterDrawCallback(clear_, xplm_Phase_Airports, 1, nullptr);
      XPLMRegisterDrawCallback(clear_, xplm_Phase_Terrain, 1, nullptr);
      XPLMRegisterDrawCallback(clear_, xplm_Phase_Vectors, 1, nullptr);
    }
    ~MakeBlank() {
      XPLMUnregisterDrawCallback(clear_, xplm_Phase_Airports, 1, nullptr);
      XPLMUnregisterDrawCallback(clear_, xplm_Phase_Terrain, 1, nullptr);
      XPLMUnregisterDrawCallback(clear_, xplm_Phase_Vectors, 1, nullptr);
    }
  };

  std::unique_ptr<MakeBlank> blank_{nullptr};

  PPL::Command toggle_{"Albair/FlyBlind/toggle",
                       "Make the world go away",
                       [this](PPL::Command::Phase phase) {
                         if (phase == PPL::Command::Phase::Begin) {
                           if (blank_ == nullptr)
                             blank_ = std::make_unique<MakeBlank>();
                           else
                             blank_.reset();
                         }
                         return PPL::Command::Outcome::Halt;
                       }};
};

PPL::DataRef<float> FlyBlind::outside_r_{
    "sim/graphics/misc/outside_light_level_r"};
PPL::DataRef<float> FlyBlind::outside_g_{
    "sim/graphics/misc/outside_light_level_g"};
PPL::DataRef<float> FlyBlind::outside_b_{
    "sim/graphics/misc/outside_light_level_b"};

int FlyBlind::clear_(int, int, void*) {
  glClearColor(outside_r_ * 0.75f, outside_g_ * 0.75f, outside_b_ * 0.75f, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  return 0;
}

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
