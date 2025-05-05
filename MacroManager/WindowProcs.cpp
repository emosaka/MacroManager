#include "WindowProcs.h"
#include "Macro.h"
#include <string>


BOOL CALLBACK resize_enum_child_proc(HWND hWnd, LPARAM lParam)
{
    int control_id = GetDlgCtrlID(hWnd);

    if (control_id == TOP_GUI)
    {
        RECT main_window_dims = { 0 };
        GetClientRect(main_window_handle, &main_window_dims);
        SetWindowPos(hWnd, HWND_TOP,
            0,                           //  x position 
            0,                          //  y position 
            main_window_dims.right,           //  width
            main_window_dims.bottom / 2,     //  height
            0);

    }

    if (control_id == REC_BUTTON)
    {
		HWND top_gui_handle = GetParent(hWnd);
		RECT top_gui_dims = { 0 };
		GetClientRect(top_gui_handle, &top_gui_dims);
		SetWindowPos(hWnd, HWND_TOP,
			top_gui_dims.right / 4,      //  x position 
			top_gui_dims.bottom / 4,   //  y position 
			top_gui_dims.right / 8,           //  width
			top_gui_dims.bottom / 2,    //  height
			0);
    }

	if (control_id == STOPCODE_BUTTON)
	{
		HWND top_gui_handle = GetParent(hWnd);
		RECT top_gui_dims = { 0 };
		GetClientRect(top_gui_handle, &top_gui_dims);
		SetWindowPos(hWnd, HWND_TOP,
			top_gui_dims.right * 9 / 32,               //  x position 
			top_gui_dims.bottom * 25 / 32,        //  y position 
			top_gui_dims.right * 2 / 32,           //  width
			top_gui_dims.bottom * 6 / 32,     //  height
			0);
	}

    if (control_id == PLAY_BUTTON)
    {
        HWND top_gui_handle = GetParent(hWnd);
        RECT top_gui_dims = { 0 };
        GetClientRect(top_gui_handle, &top_gui_dims);
		SetWindowPos(hWnd, HWND_TOP,
            top_gui_dims.right / 16,      //  x position 
			top_gui_dims.bottom / 4,      //  y position 
			top_gui_dims.right / 8,       //  width
			top_gui_dims.bottom / 2,      //  height
            0);
    }

	if (control_id == NEW_MACRO_BUTTON)
	{
		HWND top_gui_handle = GetParent(hWnd);
		RECT top_gui_dims = { 0 };
		GetClientRect(top_gui_handle, &top_gui_dims);
		SetWindowPos(hWnd, HWND_TOP,
			top_gui_dims.right * 35 / 64,                          //  x position 
			top_gui_dims.bottom * 9 / 32,                         //  y position 
			top_gui_dims.right / 16,           //  width
			top_gui_dims.bottom * 4 / 32,     //  height
			0);
	}

	if (control_id == ACTIVATE_BUTTON)
	{
		HWND top_gui_handle = GetParent(hWnd);
		RECT top_gui_dims = { 0 };
		GetClientRect(top_gui_handle, &top_gui_dims);
		SetWindowPos(hWnd, HWND_TOP,
			top_gui_dims.right * 35 / 64,                          //  x position 
			top_gui_dims.bottom * 15 / 32,                         //  y position 
			top_gui_dims.right / 16,           //  width
			top_gui_dims.bottom * 4 / 32,     //  height
			0);
	}



	if (control_id == MACRO_LISTBOX)
	{
		HWND top_gui_handle = GetParent(hWnd);
		RECT top_gui_dims = { 0 };
		GetClientRect(top_gui_handle, &top_gui_dims);
		SetWindowPos(hWnd, HWND_TOP,
			top_gui_dims.right * 10 / 16,     //  x position 
			top_gui_dims.bottom / 4,       //  y position 
			top_gui_dims.right * 5 / 16,    //  width
			top_gui_dims.bottom / 2,     //  height
			0);


	}

	if (control_id == BOT_GUI)
	{
		RECT main_window_dims = { 0 };
		GetClientRect(main_window_handle, &main_window_dims);
		SetWindowPos(hWnd, HWND_TOP,
			0,                          //  x position 
            main_window_dims.bottom / 2,      //  y position 
            main_window_dims.right,          //  width
            main_window_dims.bottom / 2,    //  height
			0);
	}


    return TRUE;
}

LRESULT CALLBACK main_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;


    switch (uMsg) {

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZING:
        DefWindowProc(hWnd, uMsg, wParam, lParam);
        EnumChildWindows(hWnd, &resize_enum_child_proc, NULL);
        break;

    default:

        result = DefWindowProc(hWnd, uMsg, wParam, lParam);

        break;
    }
    return result;
}


LRESULT CALLBACK top_gui_proc(HWND top_gui_handle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = FALSE;

    switch (uMsg) {

    case WM_COMMAND:

        result = top_gui_control_command(top_gui_handle, wParam, lParam);
        break;

    case WM_NOTIFY:

        result = top_gui_control_notification(top_gui_handle, wParam, lParam);
        break;

    case WM_LBUTTONDOWN:
        SetFocus(top_gui_handle);
        break;

    default:
        result = DefWindowProc(top_gui_handle, uMsg, wParam, lParam);
        break;
    }
    return result;
}

LRESULT top_gui_control_command(HWND top_gui_handle, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    HWND listview_handle = GetDlgItem(top_gui_handle, MACRO_LISTBOX);
    int LB_index = ListView_GetSelectionMark(listview_handle);
    int ML_index = -1;

    if (LB_index >= 0) {

		LVITEM macro_item = { 0 };
		macro_item.iItem = LB_index;
		macro_item.mask = LVIF_PARAM;

        ListView_GetItem(listview_handle, &macro_item);
        ML_index = macro_item.lParam;
    }

	int id = LOWORD(wParam);
	int code = HIWORD(wParam);




    if (id == REC_BUTTON && code == BN_CLICKED)
    {
        SetFocus(top_gui_handle);
        if (ML_index >= 0) {



            Macro::macro_list[ML_index]->read_macro();
        }
    }

    if (id == PLAY_BUTTON && code == BN_CLICKED)
    {
        SetFocus(top_gui_handle);
        if (ML_index >= 0) 
        {
            Macro::macro_list[ML_index]->play();
        }
    }

    if (id == NEW_MACRO_BUTTON && code == BN_CLICKED)
    {
        SetFocus(top_gui_handle);
        ML_index = Macro::create_macro(); 
        Macro::macro_list[ML_index]->add_to_listview();
    }

    if (id == ACTIVATE_BUTTON && code == BN_CLICKED)
    {
        SetFocus(top_gui_handle);

        if (!Macro::active)
        {
            Macro::active = TRUE;
			HWND activate_button_handle = GetDlgItem(top_gui_handle, ACTIVATE_BUTTON);
			SetWindowTextW(activate_button_handle, L"Deactivate Macros");

            HWND listview_handle = GetDlgItem(top_gui_handle, MACRO_LISTBOX);
            int count = ListView_GetItemCount(listview_handle);

            HANDLE event_handle = NULL;
            HANDLE thread_handle = NULL;
            int active_macro_index = -1;

            for (int LB_index = 0; LB_index < count; LB_index++)
            {
                LVITEM macro_item = { 0 };
                macro_item.iItem = LB_index;
                macro_item.mask = LVIF_PARAM;
                ListView_GetItem(listview_handle, &macro_item);
                ML_index = macro_item.lParam;

                if (
                    ListView_GetCheckState(listview_handle, LB_index) &&
                    Macro::macro_list[ML_index]->is_finalized() &&
                    Macro::macro_list[ML_index]->hotkey.ki.wScan != NULL
                    )
                {
                    Macro::macro_list[ML_index]->activate_macro();
                }
            }
            Macro::listen_hotkeys();
        }
        else //macros are active, deactivate
        {
            Macro::active = FALSE;
			HWND activate_button_handle = GetDlgItem(top_gui_handle, ACTIVATE_BUTTON);
			SetWindowTextW(activate_button_handle, L"Activate Macros");

            for (int i=0; i < Macro::active_macro_thread_handles.size() ; i++)
            {
                SetEvent(Macro::active_macro_event_handles[i]);
                CloseHandle(Macro::active_macro_thread_handles[i]);
            }
            Macro::active_macro_thread_handles.clear();
            Macro::active_macro_event_handles.clear();
            Macro::active_macro_ML_indexes.clear();
            Macro::active_macro_hotkeys.clear();
        }
    }

    if (id == SAVE_BUTTON && code == BN_CLICKED)
    {
        SetFocus(top_gui_handle);

		if (ML_index >= 0)
		{
			Macro::macro_list[ML_index]->save_macro();
		}
    }

    if (id == STOPCODE_BUTTON && code == STN_DBLCLK)
    {
        if (Macro::read_stopcode())
        {
            SetWindowTextW((HWND)lParam, L"reading...");
        }
    }

    return result;
}

LRESULT top_gui_control_notification(HWND top_gui_handle, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = FALSE;


    NMHDR* NM_header = (NMHDR*) lParam;
    HWND control_handle = NM_header->hwndFrom;
    UINT_PTR id = NM_header->idFrom;
    UINT code = NM_header->code;
    

    NMITEMACTIVATE* item_act = (NMITEMACTIVATE*)lParam;
    NMLVDISPINFO* disp_info = (NMLVDISPINFO*)lParam;


	if (id == MACRO_LISTBOX && code == NM_DBLCLK) // use item_act
	{
		if (item_act->iItem >= 0)
		{
            if (item_act->iSubItem == 0) // name column clicked
            {
                ListView_EditLabel(control_handle, item_act->iItem);
            }
            else if (item_act->iSubItem == 1) // hotkey column clicked
            {
				LVITEM macro_item = { 0 };
				macro_item.iItem = item_act->iItem;
				macro_item.mask = LVIF_PARAM;

				ListView_GetItem(control_handle, &macro_item);
				int ML_index = macro_item.lParam;

                Macro::macro_list[ML_index]->read_hotkey();
            }
		}
	}

    if (id == MACRO_LISTBOX && code == LVN_ENDLABELEDIT)
    {
        LPWSTR new_macro_name = disp_info->item.pszText;
        if (new_macro_name != NULL)
        {
            if (wcslen(new_macro_name) < 64)
            {
                Macro::macro_list[disp_info->item.lParam]->macro_name = new_macro_name;

                result = TRUE; 
            }
        }
    }


    return result;
}


LRESULT CALLBACK overlay_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont = {0};

	switch (uMsg)
	{
	case WM_CREATE:
	{

		hFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial");
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);


		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, hFont);


		RECT rc;
		GetClientRect(hWnd, &rc);

        std::wstring window_text(32, L'\0');

        GetWindowTextW(hWnd, &window_text[0], 32);


         
		DrawText(hdc, window_text.c_str(), -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_ERASEBKGND:

		return 1;

	case WM_DESTROY:
		DeleteObject(hFont);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

