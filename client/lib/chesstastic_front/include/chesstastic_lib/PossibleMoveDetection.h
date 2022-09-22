//
// Created by Dawid Kulikowski on 09/11/2021.
//

#ifndef CHESSTASTIC_POSSIBLEMOVEDETECTION_H
#define CHESSTASTIC_POSSIBLEMOVEDETECTION_H


struct PossibleMoveDetection {
    int disfavor_score;
    int idx;

    PossibleMoveDetection(int idx, int score = 0): idx(idx), disfavor_score(score) {}

    void disfavor_more(int severity = 1) {
        disfavor_score += 1 * severity;
    }
};


#endif //CHESSTASTIC_POSSIBLEMOVEDETECTION_H
