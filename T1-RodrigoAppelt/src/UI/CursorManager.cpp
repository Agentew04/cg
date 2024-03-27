#include <windows.h>
#include <map>
#include <iostream>

#include "CursorManager.h"

std::map<CursorManager::CursorType, bool> CursorManager::loaded;
std::map<CursorManager::CursorType, HANDLE> CursorManager::cursors;

void CursorManager::loadCursor(CursorType type){
    if(loaded[type]){
        return;
    }
    HANDLE cursor = NULL;

    if(type != CursorType::ROTATE){
        LPCTSTR cursorName = cursorTypeToString(type);
        cursor = LoadCursor(NULL, cursorName);
        if(cursor == nullptr){
            std::cout << "Error loading cursor <" << cursorName << ">: " << GetLastError() << std::endl;
            return;
        }
    }else{
        HANDLE image = LoadImageW(NULL, // hInst
            L"./T1-RodrigoAppelt/images/rotate-cursor-icon.cur", // name
            IMAGE_CURSOR, // type
            0, // cx
            0, // cy
            LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED// fuload
        );
        if (image == nullptr){
            std::cout << "Error loading cursor <rotate>: " << GetLastError() << std::endl;
            return;
        }
    }

    cursors[type] = cursor;
    loaded[type] = true;
}

LPCTSTR CursorManager::cursorTypeToString(CursorType type){
    switch(type){
        case CursorType::DEFAULT:
            return IDC_ARROW;
        case CursorType::CLICKABLE:
            return IDC_HAND;
        case CursorType::MOVE:
            return IDC_SIZEALL;
        case CursorType::LOADING:
            return IDC_WAIT;
        case CursorType::RESIZE_HORIZONTAL:
            return IDC_SIZEWE;
        case CursorType::RESIZE_VERTICAL:
            return IDC_SIZENS;
        case CursorType::RESIZE_DIAGONAL_1:
            return IDC_SIZENESW;
        case CursorType::RESIZE_DIAGONAL_2:
            return IDC_SIZENWSE;
        default:
            return IDC_NO; // to signal the user that something went wrong
            break;
    }
}

void CursorManager::setCursor(CursorType type){
    loadCursor(type);
    SetCursor((HCURSOR)cursors[type]);
}

void CursorManager::freeResources(){
    for(auto it = cursors.begin(); it != cursors.end(); it++){
        if(it->second != nullptr){
            DestroyCursor((HCURSOR)it->second);
            loaded[it->first] = false;
        }
    }
}