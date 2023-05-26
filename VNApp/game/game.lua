-- this file is bad with comments so do it shorly not --[[]]

Game.SetBackgroundColor({0,0,0,255})

Game.AddPicture("red.png","r", 1,1)
Game.AddPicture("green.png", "g", 1,1)
Game.AddPicture("background.png", "b",1,1)

Game.AddAudio("alarmclock.mp3","clock")

Game.SetBackgroundPicture("b")
Game.DrawCenter("g")

who("Зелёный")
say("abcdef")
say("раз раз раз")

Game.StopDrawing("g")
Game.DrawLeft("g")
Game.DrawRight("r")
who("Красный")
say("а теперь")

Game.PlaySound("clock")
say("будильник делает бррр")

who("Зелёный")
Game.StopSound()
say("не делает")

who("Красный")
say("уходим")

q = {"yes", "no"}
choose("result", q)

who("Зелёный")
say("согласен")

Game.StopDrawing("r")
Game.StopDrawing("g")

who("Рассказчик")
say("ВСЁ")

Game.DelPicture("r")
Game.DelPicture("g")
Game.DelPicture("b")