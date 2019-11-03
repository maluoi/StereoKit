---
layout: default
title: StereoKitApp.Initialize
description: Initializes StereoKit window, default resources, systems, etc. Set settings before calling this function, if defaults need changed!
---
# [StereoKitApp]({{site.url}}/Pages/Reference/StereoKitApp.html).Initialize
<div class='signature' markdown='1'>
static Boolean Initialize(string name, Runtime runtimePreference, Boolean fallback)
</div>

## Parameters

|  |  |
|--|--|
|string name|Name of the application, this shows up an the top of the Win32 window, and is submitted to OpenXR. OpenXR caps this at 128 characters.|
|Runtime runtimePreference|Which runtime should we try to load?|
|Boolean fallback|If the preferred runtime fails, should we fall back to flatscreen?|
|RETURNS: Boolean|Returns true if all systems are successfully initialized!|


## Description
Initializes StereoKit window, default resources, systems, etc. Set settings before calling this function, if defaults need changed!


## Examples

Here, we're doing a window!
With lots of explanation.
and lines
```csharp
UI.WindowBegin("Main", ref window_pose, new Vec2(24, 0) * Units.cm2m);

```
Maybe we'll break it up into sections?
```csharp
// Regular comment
UI.Button("Testing!\nok"); UI.SameLine();
UI.Button("Another");
UI.HSlider("slider",  ref slider1, 0, 1, 0.2f, 72 * Units.mm2m); UI.SameLine();
UI.HSlider("slider2", ref slider2, 0, 1, 0,    72 * Units.mm2m);
if (UI.Button("Press me!"))
    UI.Button("DYANMIC BUTTON!!");

UI.WindowEnd();
```
That's all folks!
Whaddya think?
