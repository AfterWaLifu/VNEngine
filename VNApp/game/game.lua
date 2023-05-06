-- this file is bad with comments so do it shorly not --[[]]

Game.SetBackgroundColor({0,0,0,255})

Game.AddPicture("masyunya.png","m", 1,1)
Game.AddPicture("back.jpg", "b",1,1)

Game.Draw("m", {500,300,256,256},0)
Game.SetBackgroundPicture("b")

who("Масюня")
say("you will die soon")
say("goodbye")

Game.Exit()