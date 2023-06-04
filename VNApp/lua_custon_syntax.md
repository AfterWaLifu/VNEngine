##Lua custom syntax

_____________________


####Game functions

```
say(STRING)		---> changing text in say-box
who(STRING)		---> changing text in who-box
wait(NUMBER)	---> pauses app for NUMBER of milliseconds
stop()			---> pauses lua-reader as if it say, but w/out changing text
choose(STRING, TABLE)	---> creates a var named STRING with result of choosing,
						---> variants to choose are in a table as strings 2o
```

_____________________

####Widget functions

Located in Widget namespace

```
Widget.Add(TABLE)			---> adds a widget on a screen, created with info in a TABLE
Widget.Update(TABLE)		---> updates existing widget with info existing in a TABLE
Widget.GetState(TYPE, NAME)	---> returns TABLE of widget of needed TYPE with a NAME as name
Widget.Remove(TYPE, NAME)	---> removes needed widget from the screen
Widget.GetTextboxValue(NAME)---> returns NAME'd textbox value
```

_____________________

Everything else is located in a Game namespace

####State machine functions

```
Game.Save(NUMBER)		---> saves game into a NUMBER slot
Game.Load(NUMBER)		---> loads game from a NUMBER slot
Game.ToMenu(STRING)		---> draws a menu with function called STRING
Game.ToGame()			---> starting a game and loading a 'game' screen
Game.IsThereReading()	---> returns true/false if there is a reading state on a stack
Game.WipeStates()		---> clears all the states
```

####Audioplayer functions

```
Game.AddAudio(FILENAME, KEY)	---> add to game an FILENAME file with a key KEY
Game.DeleteAudio(KEY)			---> removes from game a KEY audio
Game.PlayMusic(KEY)             ---> plays on repeat a KEY audio
Game.PauseMusic()               ---> pauses a music
Game.StopMysic()                ---> completely stops music (if started will play from the very beging)
Game.PlaySound(KEY)             ---> plays a KEY audio once
Game.StopSound()                ---> stops playing sound
Game.SetMusicVolume(FLOAT)      ---> sets a volume of music on a FLOAT (num between 0.0 and 1.0)
Game.SetSoundVolume(FLOAT)      ---> sets a volume of sound on a FLOAT (num between 0.0 and 1.0)
Game.GetMusicVolume()   	    ---> returns a music volume
Game.GetSoundVolume()   	    ---> returns a sound volume
Game.Mute()                     ---> mutes all the sources of sound
Game.Unmute()                   ---> unmutes them
Game.IsMuted()                  ---> returns true if muted and false if not
```

####Graphic functions

```
Game.AddPicture(FILENAME, KEY, ROWS,COLLUMNS)---> adds a pic FILENAME to game with a name KEY, thinking in has exactly ROWS and COLLUMNS (1/1 default)
Game.DelPicture(KEY)				---> removes KEY pic
Game.SetBackgroundColor(COLOR)      ---> sets a COLOR as a background
Game.SetBackgroundPicture(KEY)      ---> sets background pic KEY
Game.SetUsingBackgroundPic(BOOL)    ---> turning backpic if true, else uses color
Game.GetBackgroundColor()           ---> returns back COLOR
Game.GetBackgroundPicture()         ---> returns back pic KEY
Game.GetDrawingBackPicture()        ---> returns true if uses pic for bcgr
Game.Draw(KEY, DESTINATION, TILE)   ---> draws a KEY tile TYLE (0 default) at DESTINATION, returns primary ID of drawn pic
Game.Draw(KEY, DESTINATION, ROW, COLLUMN)---> same, but with r/c (0/0 default)
Game.DrawLeft(KEY, TILE)			---> draws KEY tile TYLE (0 default) on the left 1/4 of the window
Game.DrawLeft(KEY, ROW, COLLUMN)    ---> draws KEY tile r/c (0/0 default) on the left 1/4 of the window
Game.DrawCenter(KEY, TILE)          ---> draws KEY tile TYLE (0 default) on the center of the window
Game.DrawCenter(KEY, ROW, COLLUMN)  ---> draws KEY tile r/c (0/0 default) on the center of the window
Game.DrawRight(KEY, TILE)           ---> draws KEY tile TYLE (0 default) on the right 1/4 of the window
Game.DrawRight(KEY, ROW, COLLUMN)   ---> draws KEY tile r/c (0/0 default) on the ringt 1/4 of the window
Game.StopDrawing(KEY)               ---> stops drawing KEY pic (first of them if more then one on the screen)
Game.StopDrawingById(NUMBER)        ---> stops drawing pic with id NUMBER (insingned 32bit int)
Game.WipeScreen()                   ---> removes from screen everything being drawn including bcgr pic
Game.SetWindowSize(TABLE)           ---> changes window size to TABLE of 2 (x,y)
Game.GetWindowSize()                ---> returns window size as TABLE of 2 (x,y)
```

####Some utilities functions

```
Game.FontAdd(KEY, FILENAME, SIZE)---> adds a font from FILENAME with a size of SIZE and gives it a name KEY
Game.FontDel(KEY)               ---> removing font with a name KEY
Game.FontExist(KEY)             ---> returns true if font with a name KEY already added
Game.SetDefChoiseButton(TABLE)	---> sets a chouse button default style (including size, but not position)
Game.Screenshot()               ---> creates a png screenshot in a app root folder
Game.SetWindowTitle(STRING)		---> changes window title to STRING
Game.Exit()                     ---> shut game down
```

####Used data-types

KEY - string
COLOR - table of four elements between 0 and 255 in RGBA
STRING - string, lol
TILE, ROW, COLLUMN - just numbers
NAME or TYPE - strings, which you used in a TABLE below

TABLE - for widgets - it's a table with:
- type - "text", "button", "textbox"
- name - a name used in a game ("say" and "who" a reserved by engine in-game (menu is ok))
- text - text in a widget
- shown - bool, true if shown and false is hiden
- font - KEY of font
- wrapped - bool, enabling multiline
- aling - string of two chars, first for horiz align (L(eft)-C(enter)-R(ight)) and second for vert (T(op)-C-B(ottom))
- vindent - vertical indent of text
- hindent - horizontal indent of text
- backPic - back image if needed (KEY)
- textColor - COLOR of text
- backColor - COLOR of background
- borderColor - COLOR of border of the widget
- borderActive - true if need to draw border
- backIsShown - true to draw back and false to hide it
BUTTONS ONLY
- borderColorDefault - COLOR of button when out of focus
- borderColorFocus - COLOR of button when being focused
- func - function on button click
TEXTBOXONLY
- currentString - to current value of textbox (or starting value)
- maxChar - max length of string (32 by default)
