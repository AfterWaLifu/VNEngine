--[[
	how does menu (all of them) screens and play screen looks
	THIRD TO LOAD
]]

--[[
	Interface HOW-TO
	creation func's syntax

	table = {}
	table.type = "text"/"button"/"textbox"	// what kind of widget
	table.name = "string"			//codename of widget in programm
	table.shown = bool				// shown if true
	table.geometry = {x,y,w,h}		// without names, just an array-like
	table.textColor = {r,g,b,a}		// same here
	table.text = "string"			// what's inside
	table.wrapped = bool			// true if wrapped
	table.align	= "string"			// look at allign definition at widgets (L(eft)/C/R(ight) + T(op)/C/B(ottom))
	table.vindent = NUM				// vertical indent
	table.hindent = NUM				// horizontal indent
	table.font = "string"			// key of font for manager
	table.backPic = "string"		// pic code for texture manager
	table.backColor = {r,g,b,a}		// same as text color
	table.borderColor = {r,g,b,a}	// same
	table.borderActive = bool		//
	table.borderFocusColor = {r,g,b,a}	// as any color here
	table.maxCharQuantity = NUM		// for textbox

	Widget.Add(table)
	Widget.Update("widget type", "codename", {table like above but w/out name, type})
	Widget.GetState("widget type", "codename")
	Widget.Remove("widget type", "codename")

	Widget.GetButtonClicked("codename")
	Widget.GetButtonFocused("codename")
	Widget.GetTextboxValue("codename")
]]

menu = {
	{type="text", name="title", geometry={50,50,500,100},textColor={255,255,255,255},text="VNApp", align="LT"},
	{type="button", name="start", geometry={50,105,200,100},textColor={0,0,0,255},text="Start", align="CC"},
	{type="button", name="load", geometry={50,210,200,100},textColor={0,0,0,255},text="Load", align="CC"},
	{type="button", name="sets", geometry={50,315,200,100},textColor={0,0,0,255},text="Settings", align="CC"},
	{type="button", name="exit", geometry={50,420,200,100},textColor={0,0,0,255},text="Exit", align="CC"}
}

game = {}