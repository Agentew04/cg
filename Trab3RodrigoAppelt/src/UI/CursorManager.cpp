#include <windows.h>
#include <map>
#include <iostream>

#include "CursorManager.h"

std::map<CursorManager::CursorType, bool> CursorManager::loaded;
std::map<CursorManager::CursorType, HCURSOR> CursorManager::cursors;
CursorManager::CursorType CursorManager::currentType = CursorManager::CursorType::DEFAULT;
bool CursorManager::hasMouseSet = false;

void CursorManager::loadCursor(CursorType type){
    if(loaded[type]){
        return;
    }
    HCURSOR cursor = NULL;

    LPCTSTR cursorName;
    // esse codigo vem do T1, usado p/ mouse customizado

    // if(type == CursorType::ROTATE){
    //     cursor = (HCURSOR)LoadImageA(
    //         nullptr,
    //         ".\\Trab3RodrigoAppelt\\images\\rotate-cursor-icon.cur",
    //         IMAGE_CURSOR,
    //         0,
    //         0,
    //         LR_LOADFROMFILE
    //     );
    // }else{
    cursorName = cursorTypeToString(type);
    cursor = LoadCursor(NULL, cursorName);
    // }

    if(cursor == nullptr){
        std::cout << "Error loading cursor <" << cursorName << ">: " << GetLastError() << std::endl;
        return;
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
    hasMouseSet = true;
    if(currentType == type){
        return;
    }
    currentType = type;
}

void CursorManager::applyCursor(){
    loadCursor(currentType);
    if(!hasMouseSet){
        loadCursor(CursorType::DEFAULT);
    }
    HCURSOR cursor = cursors[hasMouseSet ? currentType : CursorType::DEFAULT];
    SetCursor(cursor);
}

void CursorManager::startFrame(){
    hasMouseSet = false;
}

void CursorManager::freeResources(){
    for(auto it = cursors.begin(); it != cursors.end(); it++){
        if(it->second != nullptr){
            DestroyCursor(it->second);
            loaded[it->first] = false;
        }
    }
}
