Macro manager is a macro app made using the windows api in c++, the project is currently in alpha and probably has alot of bugs.


Current feature are:
  recording and playback of keypresses,
  assigning hotkeys to macros,
  saving/loading macros.


In the future i plan to improve the ui, add support for mouse clicks and movement, a macro viewer/editor, and various other playback options.



to use:

  Create a new macro

  Give it a name by double clicking it on the macro list
  
  Set a hotkey for it by double clicking its "hotkey" column
  
  Set a "stop recording" key under the "Record" button
  
  Click Record and record your macro, ending with the "stop recording" key

  Either click Play, or check the macro and click on "Activate Macros" to use the hotkey


When a macro is saved, a save folder is created in the same directory as the exe, when the program starts all macros in the save folder are loaded.
  
