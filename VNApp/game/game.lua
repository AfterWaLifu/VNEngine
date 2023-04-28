--[[
	game entry point script for reader
	
	it will be different lua_state from loading
	so it's ok to use any lua-non-conflict names etc
]]

--[[
	VNEngine syntax for Lua
	
	character = {"name", red,green,blue}	aka name + color to write it
	say *STRING*	to write into box
	who *STRING*	who is saying (empty for empty box ofcs)
	
	appears *char table* *STRING*	to draw char at LEFT MIDDLE CENTER
	appears *char table* *float [-1.0;2.0]*		to draw char at custom place at the screen
	
	draw *STRING key* *INT x* *INT y*	what to draw and where for pictures
	draw *STRING key* *float %x* *float %y*		same but realated to % of screen not pixels

	background *STRING key*		for picture
	background *r* *g* *b*		for color
]]
