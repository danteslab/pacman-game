#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

class Game;
class Keyboard;

class Application {
public:
  Application();
  ~Application();

  void init(int argc, char **argv);
  void loop();
  void quit();

  Keyboard *getKeyboard() const;

  typedef struct {
    int width;
    int height;
    int fps;
    int scale;
    std::string title;
  } Settings_t;

  Settings_t settings;

private:
  bool exit_;
  Game *game_;
  Keyboard *keyboard_;
};

#endif // !APPLICATION_HPP
