---
title: Ue Plugin Only Display One
date: 2024.02.21
categories: [Game]
tags: [game/ue]
---

## Identical commands from different plugins display only one

I have two different plugins registered like this

```cpp
// plugin one
UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
{
	FToolMenuSection& Section = Menu->FindOrAddSection("Meshy");
	Section.AddMenuEntryWithCommandList(FMeshyCommands::Get().OpenPluginWindow, PluginCommands);
}
```
```cpp
//plugin two
UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
{
	FToolMenuSection& Section = Menu->FindOrAddSection("Meshy");
	Section.AddMenuEntryWithCommandList(FTestAddonCommands::Get().OpenPluginWindow, PluginCommands);
}
```

Both plugins register an `OpenPluginWindow` command, even though they originate from different classes, UE will only display the one that is registered later.

Upon debuging, I discovered that there is an attribute `Blocks` within the `Section` variable which stores all menu entries and identifies them by their `Name`. However, both plugin commands are named "OpenPluginWindow". Consequently, only one of them will be displayed.

## Solution

Knowing the reason behind this, we simply need to change the id name of the command.

The name of the command is set by `FMeshyCommands::Register();`

```cpp
void FMeshyCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Meshy", "Bring up Meshy window", EUserInterfaceActionType::Button, FInputChord());
}
```

Then check the implementation of `UI_COMMAND`

```cpp
SLATE_API void MakeUICommand_InternalUseOnly(FBindingContext* This, TSharedPtr< FUICommandInfo >& OutCommand, const TCHAR* InSubNamespace, const TCHAR* InCommandName, const TCHAR* InCommandNameUnderscoreTooltip, const ANSICHAR* DotCommandName, const TCHAR* FriendlyName, const TCHAR* InDescription, const EUserInterfaceActionType CommandType, const FInputChord& InDefaultChord, const FInputChord& InAlternateDefaultChord = FInputChord());

#define UI_COMMAND( CommandId, FriendlyName, InDescription, CommandType, InDefaultChord, ... ) \
	MakeUICommand_InternalUseOnly( this, CommandId, TEXT(LOCTEXT_NAMESPACE), TEXT(#CommandId), TEXT(#CommandId) TEXT("_ToolTip"), "." #CommandId, TEXT(FriendlyName), TEXT(InDescription), CommandType, InDefaultChord, ## __VA_ARGS__ );
```

We find that `InCommandName` is the fourth parameter of `MakeUICommand_InternalUseOnly` set as `TEXT(#CommandId)` in `UI_COMMAND`, with `OpenPluginWindow` specified in the call. That's why both commands received the same name.

There is another macro to register a command that can prevent this issue. With yhis command can we specify name ourselves.

```cpp
#define UI_COMMAND_EXT( BindingContext, OutUICommandInfo, CommandIdName, FriendlyName, InDescription, CommandType, InDefaultChord, ... ) \
	MakeUICommand_InternalUseOnly( BindingContext, OutUICommandInfo, TEXT(LOCTEXT_NAMESPACE), TEXT(CommandIdName), TEXT(CommandIdName) TEXT("_ToolTip"), "." CommandIdName, TEXT(FriendlyName), TEXT(InDescription), CommandType, InDefaultChord, ## __VA_ARGS__ );
```