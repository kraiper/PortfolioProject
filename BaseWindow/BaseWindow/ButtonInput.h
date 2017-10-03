#pragma once

#include "stdafx.h" 

enum ValidButtons {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};

class ButtonInput
{
private:
	bool pressStatus[50];
	bool lastStatus[50];
	bool mPressed, nPressed, bPressed, vPressed, cPressed, ismPressed, isnPressed, isbPressed, isvPressed, iscPressed;

public:
	void SetMPressed(bool pressed) { if (pressed) { if (ismPressed == false) { ismPressed = true; } } else { ismPressed = false; mPressed = !mPressed; } }
	void SetNPressed(bool pressed) { if (pressed) { if (isnPressed == false) { nPressed = !nPressed; }isnPressed = true; } else { isnPressed = false; } }
	void SetBPressed(bool pressed) { if (pressed) { if (isbPressed == false) { bPressed = !bPressed; }isbPressed = true; } else { isbPressed = false; } }
	void SetVPressed(bool pressed) { if (pressed) { if (isvPressed == false) { vPressed = !vPressed; }isvPressed = true; } else { isvPressed = false; } }
	void SetCPressed(bool pressed) { if (pressed) { if (iscPressed == false) { cPressed = !cPressed; }iscPressed = true; } else { iscPressed = false; } }
	bool GetMPressed() { if (ismPressed && !mPressed) { mPressed = !mPressed; return true; } return false; }
	bool GetNPressed() { return nPressed; }
	bool GetBPressed() { return bPressed; }
	bool GetIsVPressed() { return vPressed; }
	bool GetIsCPressed() { return cPressed; }

	void SetPressed(ValidButtons _button, bool _pressed) { if (lastStatus[_button] != _pressed) { pressStatus[_button] = _pressed; lastStatus[_button] = _pressed; } else { pressStatus[_button] = !_pressed; } }

	bool GetPressed(ValidButtons _button) { return pressStatus[_button]; }

};

