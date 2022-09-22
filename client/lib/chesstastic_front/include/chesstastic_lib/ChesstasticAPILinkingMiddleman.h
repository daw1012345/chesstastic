//
// Created by Dawid Kulikowski on 21/10/2021.
//

#ifndef CHESSTASTIC_CHESSTASTICAPILINKINGMIDDLEMAN_H
#define CHESSTASTIC_CHESSTASTICAPILINKINGMIDDLEMAN_H

#include "ChesstasticAPIService.h"
#include <QObject>

class ChesstasticAPILinkingMiddleman : public QObject {
Q_OBJECT
public:
    ChesstasticAPILinkingMiddleman(std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService>);

    std::string start();

signals:

    void linked(QString);

private:
    const int TOKEN_BLOCK_NUM = 4;
    const int MIN_BLOCK_LEN = 3;
    void run(std::string);

    std::shared_ptr<ChesstasticNetworking::ChesstasticAPIService> apiService;

    std::string random_token();
    std::string random_token_from_file(std::ifstream&, int);
};


#endif //CHESSTASTIC_CHESSTASTICAPILINKINGMIDDLEMAN_H
