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
Game.DrawLeft("g")
Game.DrawRight("r")
who("Красный")
say("а теперь")

Game.PlaySound("clock")
say("будильник делает бррр")

who("Зелёный")
Game.StopSound()
say("не делает")
goto finale

who("Красный")
say("уходим")

q = {"yes", "no"}
choose("result", q)

::yes::

who("Зелёный")
say("согласен")

Game.StopDrawing("g")
Game.StopDrawing("r")

::no::

who("Зелёный")
say("niht")

Game.StopDrawing("g")
Game.StopDrawing("r")
Game.DrawCenter("g")

::finale::

who("Рассказчик")
say("ВСЁ")

Game.DelPicture("r")
Game.DelPicture("g")
Game.DelPicture("b")