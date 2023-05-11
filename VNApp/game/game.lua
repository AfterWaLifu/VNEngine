-- this file is bad with comments so do it shorly not --[[]]

Game.SetBackgroundColor({0,0,0,255})

Game.AddPicture("red.png","r", 1,1)
Game.AddPicture("green.png", "g", 1,1)
Game.AddPicture("background.png", "b",1,1)

Game.AddAudio("alarmclock.mp3","clock")

Game.SetBackgroundPicture("b")
Game.Draw("g", {500,300,256,256},0)

who("Зелёный")
say("abcdef")
say("раз раз раз")

Game.StopDrawing("g")
Game.Draw("g", {200,300,300,300},0)
Game.Draw("r", {700,300,300,300},0)
who("Красный")
say("а теперь")

Game.PlaySound("clock")
say("будильник делает бррр")

who("Зелёный")
Game.StopSound()
say("не делает")

who("Красный")
say("уходим")

who("Зелёный")
say("согласен")

Game.StopDrawing("r")
Game.StopDrawing("g")
who("Рассказчик")
say("ВСЁ")

-- Game.Exit()