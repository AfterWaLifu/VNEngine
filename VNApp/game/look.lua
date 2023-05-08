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
		Widget.Add(table)
		table.name = "load"; table.geometry = {50,250,200,80}; table.text = "Load"
		Widget.Add(table)
		table.name = "settings"; table.geometry = {50,335,200,80}; table.text = "Settings"
		Widget.Add(table)
		table.name = "mainmenu"; table.geometry = {50,460,200,80}; table.text = "Main menu"
		Widget.Add(table)
		table.name = "exit"; table.geometry = {50,545,200,80}; table.text = "Exit"
		Widget.Add(table)
	else
		table = {type="button", name="start", geometry={50,165,200,80},textColor={0,0,0,255},text="Start", align="CC", font="r32"}
		Widget.Add(table)
		table.name = "load"; table.geometry = {50,270,200,80}; table.text = "Load"
		Widget.Add(table)
		table.name = "settings"; table.geometry = {50,375,200,80}; table.text = "Settings"
		Widget.Add(table)
		table.name = "exit"; table.geometry = {50,480,200,80}; table.text = "Exit"
		Widget.Add(table)
	end
end

function save()
	mainmenu()

	label = {type="text",name="label",text="Save",geometry={300,90,920,50}, textColor={255,255,255,255}, align="CT",font="r40"}
	Widget.Add(label)

	saveButton = {type="button", name="save1",geometry={300,170,300,190}, textColor={0,0,0,255},text="YY.MM.DD HH:MM",
					align="CD", font="r20", wrapped=false, vindent=3}
	savePicture = {type="text", name="save1",geometry={330,175,240,160}, textColor={0,0,0,0}, text="",backPic="savePlaceholder"}
	
	saveButton.name = "save1"
	savePicture.name = saveButton.name
	saveButton.geometry = {300,170,300,190}
	savePicture.geometry = { 330, 175, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "save2"
	savePicture.name = saveButton.name
	saveButton.geometry = {610,170,300,190}
	savePicture.geometry = { 640, 175, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "save3"
	savePicture.name = saveButton.name
	saveButton.geometry = {920,170,300,190}
	savePicture.geometry = { 950, 175, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "save4"
	savePicture.name = saveButton.name
	saveButton.geometry = {300,385,300,190}
	savePicture.geometry = { 330, 390, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "save5"
	savePicture.name = saveButton.name
	saveButton.geometry = {610,385,300,190}
	savePicture.geometry = { 640, 390, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "save6"
	savePicture.name = saveButton.name
	saveButton.geometry = {920,385,300,190}
	savePicture.geometry = { 950, 390, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
end

function load()
	mainmenu()

	label = {type="text",name="label",text="Load",geometry={300,90,920,50}, textColor={255,255,255,255}, align="CT",font="r40"}
	Widget.Add(label)
	
	saveButton = {type="button", name="save1",geometry={300,170,300,190}, textColor={0,0,0,255},text="YY.MM.DD HH:MM",
					align="CD", font="r20", wrapped=false, vindent=3}
	savePicture = {type="text", name="save1",geometry={330,175,240,160}, textColor={0,0,0,0}, text="",backPic="savePlaceholder"}
	
	saveButton.name = "load1"
	savePicture.name = saveButton.name
	saveButton.geometry = {300,170,300,190}
	savePicture.geometry = { 330, 175, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "load2"
	savePicture.name = saveButton.name
	saveButton.geometry = {610,170,300,190}
	savePicture.geometry = { 640, 175, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "load3"
	savePicture.name = saveButton.name
	saveButton.geometry = {920,170,300,190}
	savePicture.geometry = { 950, 175, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "load4"
	savePicture.name = saveButton.name
	saveButton.geometry = {300,385,300,190}
	savePicture.geometry = { 330, 390, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "load5"
	savePicture.name = saveButton.name
	saveButton.geometry = {610,385,300,190}
	savePicture.geometry = { 640, 390, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
	
	saveButton.name = "load6"
	savePicture.name = saveButton.name
	saveButton.geometry = {920,385,300,190}
	savePicture.geometry = { 950, 390, 240,160 }
	Widget.Add(saveButton)
	Widget.Add(savePicture)
end

function settings()
	mainmenu()

	label = {type="text",name="label",text="Settings",geometry={300,90,920,50}, textColor={255,255,255,255}, align="CT",font="r40"}
	Widget.Add(label)

	volume = Game.GetMusicVolume()
	number = {type="text",name="musicVolume", geometry={450,170,150,100},text=volume, textColor={255,255,255,255}, align="CC", font="r40"}
	Widget.Add(number)
	volume = Game.GetSoundVolume()
	number.name = "soundVolume"; number.geometry[1] = 920
	Widget.Add(number)
	
	change = {type="button",name="music-",geometry={325,170,100,100},text="-",align="CC",font="r72"}
	Widget.Add(change)
	change.name = "sound-"; change.geometry[1] = 795
	Widget.Add(change)

	change = {type="button",name="music+",geometry={625,170,100,100},text="+",align="CC",font="r60"}
	Widget.Add(change)
	change.name = "sound+"; change.geometry[1] = 1095
	Widget.Add(change)

	applyButton = {type="button", name="musicVolumeApply",geometry={300,300,450,50},text="Apply", align="CC",font="r32"}
	Widget.Add(applyButton)
	applyButton.name = "soundVolumeApply"; applyButton.geometry[1] = 770
	Widget.Add(applyButton)

end

function game()
	windowsize = Game.GetWindowSize()
	saybox = {
		type = "text",
		name = "say",
		geometry = {0,windowsize[2] - 200, windowsize[1],200},
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
		geometry = {0,windowsize[2] - 300, 200,100},
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