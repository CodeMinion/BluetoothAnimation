Halloween 2015 Project
- Animating Halloween Prop via Bluetooth and companion Android app.

Date: Halloween 2015

Status: Initial Version.

Allows controlling the eyes are mouth of 
a simple animatronic via Bluetooth Android App.

Command Format:
Length 3 Bytes + Terminator Byte (\n or \r)
Byte # 2: Motor ID ( 1 for Eyes, 2 for Mouth)
Byte # 1: Motion X Component (Between 0-200)
Byte # 0: Motion Y Component (Between 0-200) 

Android App: AniMote
Link: https://play.google.com/store/apps/details?id=codeminion.animmote