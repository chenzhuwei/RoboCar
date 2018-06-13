//
// Created by norse on 17-6-6.
//

#ifndef CAR_EYE_SUBWINDOW_H
#define CAR_EYE_SUBWINDOW_H


class subWindow {
public:
    char *title;
    char button[];
    subWindow(char *title);
    ~subWindow();
};


#endif //CAR_EYE_SUBWINDOW_H
