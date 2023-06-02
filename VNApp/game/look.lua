--[[
	how does menu (all of them) screens and play screen looks
	
	every function is a state of the screen, it's being called to create one
]]

function mainmenu()
	Game.SetBackgroundColor({0,0,0,255})
	Game.SetBackgroundPicture("menuBack")
	reading = Game.IsThereReading()

	-- VNApp text above
	table = {type="text", name="title", geometry={65,65,500,100},textColor={255,255,255,255},text="VNApp", align="LT", font="r60"}
	Widget.Add(table)

	if (reading) then
		table = {type="button", name="save", geometry={50,165,200,80},textColor={0,0,0,255},text="Save", align="CC", font="r32"}
		table.func = function() Game.ToMenu("save") end
		Widget.Add(table)
		table.name = "load"; table.geometry = {50,250,200,80}; table.text = "Load"
		table.func = function() Game.ToMenu("load") end
		Widget.Add(table)
		table.name = "settings"; table.geometry = {50,335,200,80}; table.text = "Settings"
		table.func = function() Game.ToMenu("settings") end
		Widget.Add(table)
		table.name = "mainmenu"; table.geometry = {50,460,200,80}; table.text = "Main menu"
		table.func = function() Game.WipeStates(); Game.ToMenu("mainmenu") end
		Widget.Add(table)
		table.name = "exit"; table.geometry = {50,545,200,80}; table.text = "Exit"
		table.func = Game.Exit
		Widget.Add(table)
	else
		table = {type="button", name="start", geometry={50,165,200,80},textColor={0,0,0,255},text="Start", align="CC", font="r32"}
		table.func = function() Game.ToGame() end
		Widget.Add(table)
		table.name = "load"; table.geometry = {50,270,200,80}; table.text = "Load"
		table.func = function() Game.ToMenu("load") end
		Widget.Add(table)
		table.name = "settings"; table.geometry = {50,375,200,80}; table.text = "Settings"
		table.func = function() Game.ToMenu("settings") end
		Widget.Add(table)
		table.name = "exit"; table.geometry = {50,480,200,80}; table.text = "Exit"; table.func = Game.Exit
		Widget.Add(table)
	end

	Game.PlayMusic("mainmenu")
end

function save()
	mainmenu()

	label = {type="text",name="label",text="Save",geometry={300,90,920,50}, textColor={255,255,255,255}, align="CT",font="r40"}
	Widget.Add(label)

	saveButton = {type="button", name="save1",geometry={300,170,300,190}, textColor={0,0,0,255},text="YY.MM.DD HH:MM",
					align="CB", font="r20", wrapped=false, vindent=3}
	savePicture = {type="text", name="save1",geometry={330,175,240,160},
		textColor={0,0,0,0}, text="",backPic="savePlaceholder",backIsShown=true}
	
	for i = 1,6 do
		saveButton.name = "save"..i
		saveButton.text = ""..i
		savePicture.name = saveButton.name
		saveButton.geometry = {300 + (310 * ((i-1)%3)),170 + (215 * math.floor(i/4)),300,190}
		savePicture.geometry = { saveButton.geometry[1]+30, saveButton.geometry[2]+5, 240,160 }
		if file_exists("game/images/savescreens/"..i..".png") then
			savePicture.backPic="savescreennumber"..i
			Game.AddPicture("savescreens/"..i..".png","savescreennumber"..i,1,1)
		else 
			savePicture.backPic="savePlaceholder"
		end
		saveButton.func = function()
				Game.Save(i)
				Game.StopDrawing("savescreennumber"..i)
				Game.DelPicture("savescreennumber"..i)
				Game.AddPicture("savescreens/"..i..".png","savescreennumber"..i,1,1)
				Widget.Update({type="text",name="save"..i,backPic="savescreennumber"..i })
			end
		Widget.Add(saveButton)
		Widget.Add(savePicture)
	end
end

function load()
	mainmenu()

	label = {type="text",name="label",text="Load",geometry={300,90,920,50}, textColor={255,255,255,255}, align="CT",font="r40"}
	Widget.Add(label)
	
	saveButton = {type="button", name="save1",geometry={300,170,300,190}, textColor={0,0,0,255},text="YY.MM.DD HH:MM",
					align="CB", font="r20", wrapped=false, vindent=3}
	savePicture = {type="text", name="save1",geometry={330,175,240,160}, textColor={0,0,0,0},
		text="",backPic="savePlaceholder", backIsShown=true}
	
		for i = 1,6 do
		saveButton.name = "load"..i
		saveButton.text = ""..i
		savePicture.name = saveButton.name
		saveButton.geometry = {300 + (310 * ((i-1)%3)),170 + (215 * math.floor(i/4)),300,190}
		savePicture.geometry = { saveButton.geometry[1]+30, saveButton.geometry[2]+5, 240,160 }
		if file_exists("game/images/savescreens/"..i..".png") then
			savePicture.backPic="savescreennumber"..i
			Game.AddPicture("savescreens/"..i..".png","savescreennumber"..i,1,1)
		else 
			savePicture.backPic="savePlaceholder"
		end
		saveButton.func = function() Game.Load(i) end
		Widget.Add(saveButton)
		Widget.Add(savePicture)
	end
end

function settings()
	mainmenu()

	label = {type="text",name="label",text="Settings",geometry={300,90,920,50}, textColor={255,255,255,255}, align="CT",font="r40"}
	Widget.Add(label)

	volume = Game.GetMusicVolume() * 100
	number = {type="text",name="musicVolume", geometry={450,170,150,100},text=math.floor(volume), textColor={255,255,255,255}, align="CC", font="r40"}
	Widget.Add(number)
	volume = Game.GetSoundVolume() * 100
	number.name = "soundVolume"; number.geometry[1] = 920; number.text = math.floor(volume)
	Widget.Add(number)
	
	function cm(c)
		volume = Game.GetMusicVolume() + c
		volume = math.floor((volume + 0.001)*100)/100
		if volume > 1.0 then volume = 1.0 
		elseif volume < 0 then volume = 0 end
		Game.SetMusicVolume(volume)
		Widget.Update({type="text",name="musicVolume",text=math.floor(volume*100)})
	end
	function sm(c)
		volume = Game.GetSoundVolume() + c
		volume = math.floor((volume + 0.001)*100)/100
		if volume > 1.0 then volume = 1.0 
		elseif volume < 0 then volume = 0 end
		Game.SetSoundVolume(volume)
		Widget.Update({type="text",name="soundVolume",text=math.floor(volume*100)})
	end
	change = {type="button",name="music-",geometry={325,170,100,100},text="-",align="CC",font="r72"}
	change.func = function() cm(-0.05) end
	Widget.Add(change)
	change.name = "sound-"; change.geometry[1] = 795
	change.func = function() sm(-0.05) end
	Widget.Add(change)

	change = {type="button",name="music+",geometry={625,170,100,100},text="+",align="CC",font="r60"}
	change.func = function() cm(0.05) end
	Widget.Add(change)
	change.name = "sound+"; change.geometry[1] = 1095
	change.func = function() sm(0.05) end
	Widget.Add(change)

	label.name = "musicLabel"
	label.font = "r32"
	label.text = "Music"
	label.geometry = {300,300,450,100}
	Widget.Add(label)
	label.name = "soundLabel"
	label.text = "Sound"
	label.geometry[1] = 770
	Widget.Add(label)


	function muting()
		Game.Mute()
		Widget.Update({type="button",name="muteButton",text="Unmute", func=unmuting})
	end
	function unmuting()
		Game.Unmute()
		Widget.Update({type="button",name="muteButton",text="Mute", func=muting})
	end
	muteButton = {type="button",name="muteButton",geometry={625,370,270,50},font="r32",text="Mute", align="CC"}
	muteButton.func = muting
	muted = Game.IsMuted()
	if (muted) then
		muteButton.text = "Unmute"
		muteButton.func = unmuting
	end
	Widget.Add(muteButton)
end

function game()
	windowsize = Game.GetWindowSize()
	saybox = {
		type = "text",
		name = "say",
		geometry = {0,520, 1280,200},
		text = "A",
		align = "LT",
		font = "r20",
		wrapped = true,
		vindent = 35,
		hindent = 45,
		backColor = {200,200,255,128},
		borderColor = {0,0,0,128},
		borderActive = true
	}
	whobox = {
		type = "text",
		name = "who",
		geometry = {0,420, 200,100},
		text = "AA",
		align = "CC",
		font = "r32",
		wrapped = false,
		vindent = 0,
		hindent = 0,
		backColor = {200,200,255,128},
		borderColor = {255,255,255,128},
		borderActive = true
	}
	Widget.Add(saybox)
	Widget.Add(whobox)
end

-- used somewhere
function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end
