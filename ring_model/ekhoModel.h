//
// Created by norse on 17-4-19.
//

#ifndef CAR_EYE_EKHOMODEL_H
#define CAR_EYE_EKHOMODEL_H

#include "list"
class ekhoModel {
public:
    std::list ThreadPool;
    void speakAny(char *msg);
    void speak(char *msg);
    void stopSpeak();
private:
    creatSpeakThread(char msg);

};


#endif //CAR_EYE_EKHOMODEL_H
