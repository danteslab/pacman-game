#include "Application.hpp"

int main() {
  Application app;

  app.init(0, nullptr);
  app.loop();
  app.quit();

  return 0;
}
