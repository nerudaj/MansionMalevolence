# UI Guide

## Themes

TGUI supports theme loadable theme files that can apply a unified look to all your widgets. Some example theme files from the TGUI repo are already placed inside `assets/ui-themes` and automatically loaded upon start of the application.

To apply the theme to the application, you need to invoke call such as this one:

```cpp
gui.setTheme(resmgr.get<tgui::Theme::Ptr>("Black.txt"));
```

This line is prepared for you in the constructor of the `DependencyContainer`, just uncomment it and use whatever name of the theme you want. Under debug build, there is a dropdown under `Options->Video` that lets you quickly swap between loaded themes and check out how your app looks under each of them.

## UI Design principles

To make the UI-building code as simple as possible, there is a bunch of UI builders placed inside `lib/include/gui/builders`. The core principle of UI within this project is that each widget is placed at [0%, 0%] of the parent and takes up [100%, 100%] of the parent size. The motivation here is that you should not deal with these two properties of each widget - instead create a layout that dictates position and size, whereas each widget just deals with the logic (and its own rendering).

Layouting is abstracted through the builders. While `WidgetBuilder` only creates individual widgets, other builders let you to design the layout of each screen, layout of a table with widgets, layout of a list of buttons, etc.

Second core principle is DPI awareness. There is a class called `Sizers`, which returns the base DPI-aware font size and base DPI-aware height of a container with a single line of text. There is then a concept of H2 header (1.5x times of base height) and H1 header (2x times of base height). You can further change the app-wide scaling by calling `Sizers::setScale`. There is a slider under `Options->Video` to adjust this value.
