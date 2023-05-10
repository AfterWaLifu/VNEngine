# VNEngine
Creating an Visual Novel Engine in C++
______________________________________

For development vcpkg (with global settings) is used
All the libs (as posible) is x64 and static

Libs needed:
*	SDL2
*	(for Windows it includes ALL the libs used by SDL2, because of static linking)
*	SDL_image (including JPG PNG TIF WEBP libs)
*	SDL_ttf (fonts to draw text)
*	OpenAL - audio api
*	sndfile - small lib to read audio files
*	lua - script language
*	LuaBridge - C++ lua wrapper
*	jsoncpp