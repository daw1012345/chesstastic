//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSTASTICAPIMIDDLEMAN_H
#define CHESSTASTIC_CHESSTASTICAPIMIDDLEMAN_H

#include "ChesstasticAPIMiddleman.h"
#include <QObject>
#include <chesstastic_chess/ChessBoard.h>
#include "ChesstasticAPIService.h"

class ChesstasticAPIMiddleman : public QObject {
Q_OBJECT
public:
    ChesstasticAPIMiddleman(std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> service) : apiservice(
            service) {}

    void start_poll() {
        std::thread([=] {
            poll();
        }).detach();
    }

signals:

    void new_game(int);

private:
    std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> apiservice;

    void poll();
};


#endif //CHESSTASTIC_CHESSTASTICAPIMIDDLEMAN_H
