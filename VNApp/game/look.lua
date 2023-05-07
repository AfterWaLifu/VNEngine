--[[
	how does menu (all of them) screens and play screen looks
	
	every function is a state of the screen, it's being called to create one
]]

function mainmenu()
	Game.SetBackgroundColor({250,125,125,255})
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
	table = {type="text", name="placeholder", geometry={565,365,300,100},text="save screen", font="r60"}
	Widget.Add(table)
end

function load()
	mainmenu()
	table = {type="text", name="placeholder", geometry={565,365,300,100},text="Load screen", font="r60"}
	Widget.Add(table)
end

function settings()
	mainmenu()
	table = {type="text", name="placeholder", geometry={565,365,300,100},text="Settings", font="r60"}
	Widget.Add(table)
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