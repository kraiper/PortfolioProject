#ifndef _RAWINPUT
#define _RAWINPUT
#include "stdafx.h"
#include "Camera.h"
#include "ButtonInput.h"

extern Camera* Cam;
extern ButtonInput* buttonInput;

//register keyboard mouse as input devices!
bool RegisterInputDevices( HWND &hWnd )
{
	RAWINPUTDEVICE inputDevices[2];
        
	//adds mouse and allow legacy messages
	inputDevices[0].usUsagePage = 0x01; 
	inputDevices[0].usUsage = 0x02; 
	inputDevices[0].dwFlags = 0;   
	inputDevices[0].hwndTarget = 0;

	//adds keyboard and allow legacy messages
	inputDevices[1].usUsagePage = 0x01; 
	inputDevices[1].usUsage = 0x06; 
	inputDevices[1].dwFlags = 0;   
	inputDevices[1].hwndTarget = 0;

	if ( RegisterRawInputDevices(inputDevices, 2, sizeof(inputDevices[0]) ) == FALSE ) 
	{
		return false;
	}

	return true;
}

void inline HandleRawInput( HWND &hWnd, HRAWINPUT &lParam )
{
	//get raw input data buffer size
	UINT dbSize;
	GetRawInputData( lParam, RID_INPUT, NULL, &dbSize,sizeof(RAWINPUTHEADER) );
    

	//allocate memory for raw input data and get data
	BYTE* buffer = new BYTE[dbSize];    
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &dbSize, sizeof(RAWINPUTHEADER) );
	RAWINPUT* raw = (RAWINPUT*)buffer;
	
	// Handle Keyboard Input
	//---------------------------------------------------------------------------
	if (raw->header.dwType == RIM_TYPEKEYBOARD) 
	{
		switch( raw->data.keyboard.Message )
		{
			//key up
			case WM_KEYUP : 
				switch ( raw->data.keyboard.VKey )
				{
					case 'W' : Cam->setMovementToggle( 0, 0 );
						buttonInput->SetPressed(W, false);
					break;

					case 'S' : Cam->setMovementToggle( 1, 0 );
						buttonInput->SetPressed(S, false);
					break;

					case 'A' : Cam->setMovementToggle( 2, 0 );
						buttonInput->SetPressed(A, false);
					break;

					case 'D' : Cam->setMovementToggle( 3, 0 );
						buttonInput->SetPressed(D, false);
					break;

					case VK_SPACE: Cam->setMovementToggle(4, 0);
					break;

					case VK_SHIFT: Cam->setMovementToggle(5, 0);
					break;

					case 'M' : buttonInput->SetMPressed(false);
					break;
					
					case 'N' : buttonInput->SetPressed(N, false);
					break;
					
					case 'B' : buttonInput->SetBPressed(false);
					break;

					case 'V' : buttonInput->SetVPressed(false);
					break;

					case 'C' : buttonInput->SetCPressed(false);
					break;

					case 'R': buttonInput->SetPressed(R, false);
					break;

					case 'F': buttonInput->SetPressed(F, false);
					break;

					case 'T': buttonInput->SetPressed(T, false);
					break;

					case 'G': buttonInput->SetPressed(G, false);
					break;

					case 'Y': buttonInput->SetPressed(Y, false);
					break;

					case 'H': buttonInput->SetPressed(H, false);
					break;
				}
			break;

			//key down
			case WM_KEYDOWN : 
				switch ( raw->data.keyboard.VKey )
				{
					case VK_ESCAPE : PostQuitMessage(0);
					break;

					case 'W' : Cam->setMovementToggle( 0, 1 );
						buttonInput->SetPressed(W,true);
					break;

					case 'S' : Cam->setMovementToggle( 1, -1 );
						buttonInput->SetPressed(S, true);
					break;

					case 'A' : Cam->setMovementToggle( 2, -1 );
						buttonInput->SetPressed(A, true);
					break;

					case 'D' : Cam->setMovementToggle( 3, 1 );
						buttonInput->SetPressed(D, true);
					break;

					case VK_SPACE: Cam->setMovementToggle(4, 1);
					break;

					case VK_SHIFT: Cam->setMovementToggle(5, -1);
					break;

					case 'M' : buttonInput->SetMPressed(true);
					break;

					case 'N' : buttonInput->SetPressed(N, true);
					break;
					
					case 'B' : buttonInput->SetBPressed(true);
					break;

					case 'V' : buttonInput->SetVPressed(true);
					break;

					case 'C' : buttonInput->SetCPressed(true);
					break;		

					case 'R': buttonInput->SetPressed(R, true);
					break;

					case 'F': buttonInput->SetPressed(F, true);
					break;

					case 'T': buttonInput->SetPressed(T, true);
					break;

					case 'G': buttonInput->SetPressed(G, true);
					break;

					case 'Y': buttonInput->SetPressed(Y, true);
					break;

					case 'H': buttonInput->SetPressed(H, true);
					break;
				}
			break;
		}
	}
	
	// Handle Mouse Input
	//---------------------------------------------------------------------------
	else if (raw->header.dwType == RIM_TYPEMOUSE) 
	{
		//mouse camera control
		Cam->adjustHeadingPitch( 0.0025f * raw->data.mouse.lLastX, 0.0025f * raw->data.mouse.lLastY );				
		
	}

	//free allocated memory
	delete[] buffer;

}

#endif