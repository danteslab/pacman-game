#pragma once
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

class Application;

class Interface {
public:
  explicit Interface(Application *app);
  virtual ~Interface();

  virtual void draw() const = 0;
  virtual void update() = 0;

protected:
  Application *app;
};

#endif // !INTERFACE_HPP
