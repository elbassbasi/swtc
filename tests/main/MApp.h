//
// Created by azeddine on 27/08/2020.
//

#ifndef SWT_MAPP_H
#define SWT_MAPP_H

#include "MFrame.h"

typedef w_theme* (*_w_toolkit_custom_create_theme)(const char* name);
class MApp : public WApp {
private:
    WToolkit* custom;
    _w_toolkit_custom_create_theme w_toolkit_custom_create_theme;
    WImageList imagelist16;
    WImageList imagelist32;
    WImage image16[IMAGE_LAST];
    WImage image32[IMAGE_LAST];
public:
    MApp(int args, char **argv);
    ~MApp();
    static MApp* Get(){
        return (MApp*)WApp::Get();
    }
    WImageList* GetImageList16();
    WImageList* GetImageList32();
    WImage* GetImage16(int index);
    WImage* GetImage32(int index);
    WToolkit* GetCustomToolkit();
    WTheme* SetCustomTheme(WTheme* theme);
    WTheme* CreateCustomTheme(const char* name);
    WFrame* NewCustomShell();
    WFrame* NewPlatformShell();
private:
    void LoadCustomToolkit();
};



#endif //SWT_MAPP_H
