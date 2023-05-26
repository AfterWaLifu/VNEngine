-- this file is bad with comments so do it shorly not --[[]]

Game.SetBackgroundColor({0,0,0,255})

Game.AddPicture("red.png","r", 1,1)
Game.AddPicture("green.png", "g", 1,1)
Game.AddPicture("gr.png", "gr", 1,2)
Game.AddPicture("background.png", "b",1,1)

Game.AddAudio("alarmclock.mp3","clock")

Game.SetBackgroundPicture("b")
Game.DrawCenter("gr", 0)

who("Зелёный")
say("abcdef")
say("раз раз раз")

Game.StopDrawing("gr")
Game.DrawLeft("gr", 0)
Game.DrawRight("gr", 1)
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

Game.StopDrawing("gr")
Game.StopDrawing("gr")

who("Рассказчик")
say("ВСЁ")

Game.DelPicture("r")
Game.DelPicture("g")
Game.DelPicture("b")