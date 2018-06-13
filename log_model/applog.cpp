//
// Created by Summer-V on 17-6-11.
//

#include <iostream>
#include "applog.h"

void applog::printError(std::string msg) {
    std::cout<<"[Error]   "<<msg<<std::endl;
}
void applog::printWarring(std::string msg) {
    std::cout<<"[Warring] "<<msg<<std::endl;
}
void applog::printInfo(std::string msg) {
    std::cout<<"[Info]    "<<msg<<std::endl;
}
void applog::printExit(std::string msg) {
    std::cout<<"[Exit]    "<<msg<<std::endl;
}
void applog::printSuccess(std::string msg) {
    std::cout<<"[Success] "<<msg<<std::endl;
}
void applog::printWaiting(std::string msg) {
    std::cout<<"[Waiting] "<<msg<<std::endl;
}
void applog::printLoading(std::string msg) {
    std::cout<<"[Loading] "<<msg<<std::endl;
}
void applog::logError(std::string msg) {
    std::cout<<"[Error]   "<<msg<<std::endl;
}
void applog::logWarring(std::string msg) {
    std::cout<<"[Warring] "<<msg<<std::endl;
}
void applog::logInfo(std::string msg) {
    std::cout<<"[Info]    "<<msg<<std::endl;
}
void applog::logExit(std::string msg) {
    std::cout<<"[Exit]    "<<msg<<std::endl;
}
void applog::logSuccess(std::string msg) {
    std::cout<<"[Success] "<<msg<<std::endl;
}
void applog::logWaiting(std::string msg) {
    std::cout<<"[Waiting] "<<msg<<std::endl;
}
void applog::logLoading(std::string msg) {
    std::cout<<"[Loading] "<<msg<<std::endl;
}