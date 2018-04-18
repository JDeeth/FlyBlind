#include <XPLMPlugin.h>
#include <cstring>
#include <string>

static XPLMDrawCallback_f draw_cb;

PLUGIN_API int XPluginStart(char* out_name, char* out_sig, char* out_desc) {
  std::string name{"FlyBlind"};
  strcpy(out_name, name.c_str());
  strcpy(out_sig, name.c_str());
  std::string desc{"Disables external drawing by command"};
  strcpy(out_desc, desc.c_str());

  draw_cb = [](int, int, void*) {
    glClearColor(0.15, 0.15, 0.17, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
  };
  XPLMRegisterDrawCallback(draw_cb, xplm_Phase_Airports, 1, nullptr);
  XPLMRegisterDrawCallback(draw_cb, xplm_Phase_Terrain, 1, nullptr);
  XPLMRegisterDrawCallback(draw_cb, xplm_Phase_Vectors, 1, nullptr);
  return 1;
}

PLUGIN_API void XPluginStop() {
  XPLMUnregisterDrawCallback(draw_cb, xplm_Phase_Airports, 1, nullptr);
  XPLMUnregisterDrawCallback(draw_cb, xplm_Phase_Terrain, 1, nullptr);
  XPLMUnregisterDrawCallback(draw_cb, xplm_Phase_Vectors, 1, nullptr);
}
PLUGIN_API int XPluginEnable() {
  return 1;
}
PLUGIN_API void XPluginDisable() {}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, long, void*) {}
