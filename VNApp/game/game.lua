--[[
	game entry point script for reader
	
	it will be different lua_state from loading
	so it's ok to use any lua-non-conflict names etc
]]

Game.SetBackgroundColor({0,0,0,255})

Game.AddPicture("masyunya.png","m", 1,1)
Game.AddPicture("back.jpg", "b",1,1)

Game.Draw("m", {500,300,256,256},0)
Game.SetBackgroundPicture("b")

who("Масюня")
say("you will die soon")
say("goodbye")

Game.Exit()