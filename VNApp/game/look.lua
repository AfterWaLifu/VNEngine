--[[
	how does menu (all of them) screens and play screen looks
	THIRD TO LOAD
]]

function mainmenu()
	Game.SetBackgroundColor({200,100,100,255})
	table = {type="text", name="title", geometry={65,65,500,100},textColor={255,255,255,255},text="VNApp", align="LT", font="r60"}
	Widget.Add(table)
	table = {type="button", name="start", geometry={50,165,200,100},textColor={0,0,0,255},text="Start", align="CC", font="r32"}
	Widget.Add(table)
	table.name = "load"; table.geometry = {50,270,200,100}; table.text = "Load"
	Widget.Add(table)
	table.name = "sets"; table.geometry = {50,375,200,100}; table.text = "Settings"
	Widget.Add(table)
	table.name = "exit"; table.geometry = {50,480,200,100}; table.text = "Exit"
	Widget.Add(table)
end